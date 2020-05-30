# Linux Enviroment Experiment
*This my class note for CS35L, instructed by Paul Eggert and Ritam Sarmah, at UCLA. The main purpose of this project is to construct enviroment under Linux 
system, practice working under Linux. The README contains most of the topics. More details are in the log files of each subdirectories.*

----------------
## Content
* [Linux Basic](#linux-basic)
	* [FILE Inode Link](#files)
	* [Shell Command, Enviroment Variable and Emacs](#Using-Linux-with-a-shell)
	* [Emacs GDB Valgrind](#Emacs-basic)
* [Encryption and SSH](#Encryption-and-SSH-secure-shell)
* [Shell Scripts and Regular Expression](#Shell-Scripts-and-Regular-Expression)
* [Patches, Compilation & Makefile](#Patches-Compilation-and-Makefile)
* [C Basic](#c-basic)
* [System Call Programming](#System-Call-Programming)
	* [Kernel and syscall](#kernel)
	* [Buffered vs. Unbuffered I/O](#Buffed-Unbuffered-IO)
	* [System call Commands](#System-call-Commands)
* [Linking](#Linking)
* [Git Management Control](#Git)
* [Acknowledgments](#Acknowledgments)

## Main topics

* Getting to know your system
* SSH setup and use in application
* Shell scripting
* C programming and debugging
* System call programming and debugging
* Dynamic linking
* Basic change management
* Change management speluning and implementation
* Research and development in computing
---------------------------------
## Linux basic
[Back to Content Tabble](#linux-enviroment-experiment)

Everyting in Linux is a file or process  
Files are collection of data  
Process are an instance of an executing program  

### Files
- Organized in a tree of directories  
- Regular files are text, images, PDF, etc  
- Directoreis, executble programs, devices  
- Hidden file start with a dot .ssh .profile

### Metadata and Inodes
- Every file is associated with descriptive info called metadata, including:
	- File type (regular, dir,...)
	- Link count, how many directory entries link to the file
	- Permission,  which users can read, write, or execute a file 
	- File owner
	- File size (in Byte)
	- Time of last access (read/execute), last modification (write), and change to inode
	- Disk address of file contents
- Inodes & directories
	- Every file in the file system is associated with an inode, a data structure that stores its corresponding file's metadata
	- Inodes are stored in an inode table, which is an array with all the files in the filesystem.
	- Each inode has a unique inode number that is used to index into the table
	- A directory is actually a file mapping filenames to inode numbers
	- Inodes and directories work together to store metadata for every file
	

### Links
- Hard links points directly to inode on dist, This means multiple files can map to the same inode
- Symbolic(soft) links point to the path of the file
- Create link
	- Hard: `ln source link`
	- Soft: `ln -s source link`


### Using Linux with a shell
Shell is a interface where you can interact with Linux, bash, zsh

> File Paths
- Root dir is /
- User's homedir is ~ not a folder, but shell internally expand to home dir
- Absolute path, relative to root, start with /
- Relative path, relative to current dir

### Essential Linux directories  
Command | Definition
------ | ---------
/bin  | essential binaries (e.g. pwd, bash)
/dev  | devices (and virtual ones like /dev/null)
/home | users' home directories
/tmp  | temporary files
/usr  | user files and binaries (e.g. /usr/bin)

### Basic command

`echo $PATH`         default path for command devided by :  
`pg1 2> pg2`    write stderr from pg1 to pg2  
`mkdir d`  
`stat d`      this shows Linksnumber 2, one is d, one is . inside d  
`ln -s d soft`   create sofelink `soft` to d     ln is not allowed to hard link a dir  
`C-l` clear terminal screen  
`C-u` delete all text behind cursor  
`ps` list current funning process  
`kill PID` kill process  

> Enviroment variables

`HOME` ~
`PATH`  list of directories, separated by colons, where shell should look for executables   
Assign variable using VAR=...   
Access with dollar sign, e.g. $VAR  

`export` 命令用于设置或显示环境变量。    
在 shell 中执行程序时，shell 会提供一组环境变量。  
export 可新增，修改或删除环境变量，供后续执行的程序使用。export 的效力仅限于该次登陆操作。  
`export PATH=”$PATH:your path1:your path2 ...”`   
**You can export in your ~/.profile file to do it every time you log in**  



### Emacs basic
Action | Command
-------|----------
Exit | C-x C-c
Undo | C-x u
Save | C-x s
Copy | M-w
Cut('kill') | C-w
Paste('Yank') | C-y
Run shell Command | M-! COMMAND
Compile | M-x compile
Open dir | C-x d
Cancel comand | C-g

### GDB

Compile with -g flag
`(gdb)` prompt  
`(gdb) run arg1 --arg2`  run

> Breakpoint

When you reach the breakpoint, the line is not executed yet

`(gdb) break file.c:6`  This sets a breakpoint at line 6 of file.c.
`(gdb) break my_func`  This sets a breakpoint that pauses execution anytime my_func is called.  
`(gdb) break my_func if expression`  Pause at my_func only when expression is true  
`(gdb) info break`  List breakpoints  
`(gdb) delete`  Delete all breakpoints  
`(gdb) delete 3`  Delete 3rd breakpoint  
`(gdb) clear my_function`  Clear breakpoint at my_function  
`(gdb) clear 6`  Clear breakpoint at line 6  
`(gdb) clear file.c:6`  Clear breakpoint at line 6 in file.c

> Once you hit a breakpoint, you can control how to continue execution

`(gdb) continue`  Continue execution until the next breakpoint.  
`(gdb) step [n]`  Step to next [n] line(s) of code. Steps into functions.  
`(gdb) next [n]`  Same as step, but won't step into functions.  
 `<Enter>`   repeats the last command you typed in GDB  
 
 `(gdb) print var`  Prints the value of var.  
 `(gdb) watch var`  Create watchpoint that pauses execution when var value changes

> Analyzing the program

`(gdb) info frame`  Displays info about current stack frame, such as return address and register values.  
`(gdb) info local`  List local variables and values of the function for the current stack frame.  
`(gdb) info args`  List arguments for the current function call.  
`(gdb) list [line/function]`  List 10 centered around line or function  

> Change variable value in current session

`(gdb) print answer = 1` set the value of local variable answer to 1 in current gdb session   
`(gdb) print sizeof(arr)`  run c code with gdb   

### Valgrind
- `valgrind ./file`  
- `valgrind --leak-check=full ./file`  to show more detail  



------------------------------------------------------------
## Encryption and SSH secure shell
[Back to Content Tabble](#linux-enviroment-experiment)

> Cipher example
 - Caesar's    shift
 - AES(symmetric) transform data a number of time key can be up to 128, 192, or 256 bits
 - RSA(asymmetric) combine 2 large prime to generate public key the prime are kept secret in the private key for decrypt use asymmetric to send symmetric key is SSH

### SSH
1. server provide its identity, if client to an unrecognized server,
	ssh show the hostname, IP and unique fingerprint of server's pubkey for 
	verification those will be stored in .ssh in home directory of client, then the 
	server become recognized, pubkey of server stored in `~/.ssh/known_host`
	this fingure is hashed pubkey, used to verity server
2. client ask server to prove its the owner of the public key by encrypthing
	a message with pubkey, server use private key to decripty, verification finish
3. Clien authentication
	- Password(default)
	
	- SSH keys:
		1. client generate keypair, share it with server
		2. server copies pubkey to its `~/.ssh/authorized_keys`
		3. server encrypy challenge with pubkey to client check if client can decrypt
	   Session encryption, client and server agree on a sym key called `session key`

### [lab2](https://github.com/CChenLi/Linux/tree/master/SSH) starting at lnxsrv09
Generate SSH key pair  
- `ssh-keygen` 
- `<enter passphrase>` Then you will see fingerprint 
- `cd .ssh`  
- `id_rsa` and `id_rsa.pub` is created	

> Copy pubkey to server
1. Manually append content of `id_rsa.pub` to `authorized_keys`
2. or `ssh-copy-id username@host`
- `ssh-copy-id username@lnxsrv10`
- `<enter password>`

> Login with SSH key from lnxsrv09 to lnxsrv10
- `ssh username@lnxsrv10`
- By default, SSH uses defualt key `~/.ssh/id_rsa`
- Or, `ssh -i <path-to-private-key> username@lnxsrv10`
- If your key has passphrase, you need to enter it in prompt

> Use ssh-agent a process need to redo everytime
1. Back to lnxsrv09 `exit`
2. ``eval `ssh-agent` `` start process
3. `ssh-add` tell which key to use
4. enter passphrase
5. then you can ssh to server without entering passphrase
forwarded over SSH using ssh -A
You can modify `./profile` or `.bashrc` in `~`to automatically do this 

> Use X11 forwarding
- `ssh -X`
- `xeyes` check success

> Multiple hop
1. set up `ssh-agent`
2. keep use `ssh -A`

### gpg
- tool that provides digital encryption and signing services
- Certifies and timesramps a document
- if the document is subsequently modified, signature will fail

-------------------------------------------------------------
## Shell Scripts and Regular Expression

[Back to Content Tabble](#linux-enviroment-experiment)

### Some commands work with regex
- `tr` translate ro delete **character**
	* `echo "hello world" | tr 'odw' 'aye'` `hella early`
	* `echo "hello world" | tr -d 'lo'` `he wrd`
	* `echo "hello world" | tr -s 'l'` `helo world`
	* `echo "hello world" | tr 'odw' 'o'` `hella aarla`
	* `echo "hello world" | tr 'o' 'aye'` `hella warld` maps o to a
- `sort [FILE]` also from stdin
	* `-u` remove duplicate
	* sort by new line
- `comm FILE1 FILE2` generate output of 3 columns
	* Col1: Lines unique to FILE1
	* Col1: Lines unique to FILE2
	* Col1: Lines common in both file
	* read stdin by pass `-` as filename
	* `-1` supress Col1
- `wc` count lines, words, bytes
	* `-l` only lines
- `locale`
	* print information about current locale enviroment, language, country, character encoding settings
	* Prefixed with `LC_` `LC_COLLATE`-order for comparing and sorting
	* `export LC_ALL='C'` Change all local variable to `C`
	* Locale characer encoding can affect behavior of command like `sort` 
- `grep` Search for lines matching text
	* `-E` use ERE, `-F` matches fixed string instead of regex
	* `ls -l | grep "^d"`
- `sed` reolace text using regex `-E`
	* `sed "s/ORIGINAL/REPLACEMENT/[FLAGS]" [FILENAME]`
	* `sed "s/this/that/" file` replace first `this` with `that`
	* `sed "s/this/that/g" file` replace all
	* `sed -E "s/hi|bye//g" file` remove all `hi` or `bye`
- `find` search for files in a directory
	* `-name` search by name
	* `-type` search by file type
	* `-maxdepth DEPTH` descend into directory to DEPTH
	* `-prune` ignore a directory and files under it
	* `find . -type d -exec echo {} >> matches.txt \;` `\;` completes `exec`
	
### Shell Scripts
[Learn X in Y minutes](https://learnxinyminutes.com/docs/bash/)  
- specify the interpreter path at top of file using **shebang**
	*`#!/usr/bin/bash`
	* OR automatically find bash interpreter in $PATH using `#!/usr/bin/env bash`
- `man test`
```
#!/usr/bin/env bash

echo Hello world! # => Hello world!

# Each command starts on a new line, or after a semicolon:
echo 'This is the first line'; echo 'This is the second line'

first=Bruce    #no space!
last=Wayne
echo $first
name="$first $last"

#parameter expansion ${ }     avoid ambiguity
echo "${name} is Batman"

# Command substitution  $()
dirs=$(find . -maxdepth 1)
echo "My current directory is $(pwd)"

# Arithmetic expansion $(())
a=$((1+3))
b=$(($a+3))    #b = 7

# Built-in variables:
# There are some useful built-in variables, like
echo "Last program's return value: $?"
echo "Script's PID: $$"
echo "Number of arguments passed to script: $#"      #not include program name
echo "All arguments passed to script: $@"
echo "Script's arguments separated into different variables: $1 $2... ${10} ..."

#Quotes
a=pwd
echo '$a'   # $a            single quotes use literal
echo "$a"   # pwd           double quotes expand backticks and $
echo `$a`   # /usr/...      backticks expand and execute as shell command
echo "There are `ls | wc -l` items here."
#There are        3 items here.    space due to wc format

# Conditionals
echo "always executed" || echo "only execute if first fails"
echo "always executed" && echo "only execute if first not fails"

# if statements 	requires space since [ and ] are commands man [
numer=$(($RANDOM % 61 - 30)) between -30 to 30
if [ $number -eq 0 ]; then
	echo "Zero"
elif [ $number -gt 0 ]; then
	echo "Positive"
else 
	echo "Negative"
fi

if [ -d abc ]  #True is abc exist and is a directory

echo "What's your name?"
read Name # Note that we didn't need to declare a new variable
echo Hello, $Name!

# To use && and || with if statements, you need multiple pairs of square brackets:
if [ "$Name" == "Steve" ] && [ "$Age" -eq 15 ]
then
    echo "This will run if $Name is Steve AND $Age is 15."
fi

if [ "$Name" == "Daniya" ] || [ "$Name" == "Zach" ]
then
    echo "This will run if $Name is Daniya OR Zach."
fi

# There is also the `=~` operator, which tests a string against a Regex pattern:
Email=me@example.com
if [[ "$Email" =~ [a-z]+@[a-z]{2,}\.(com|net|org) ]]
then
    echo "Valid email!"
fi

# Bash uses a `case` statement that works similarly to switch in Java and C++:
case "$Variable" in
    #List patterns for the conditions you want to meet
    0) echo "There is a zero.";;  #;; is like break
    1) echo "There is a one.";;
    *) echo "It is not null.";;
esac

for i in {1..3}; do
	echo "$i"
done

for ((i=1; i <=3; i++)); do
	echo "$i"
done

while [ true ]
do
	echo "loop body here..."
	break
done

greeting="hello world"       hello
for word in $greeting; do    world
	echo "${word}"
done			   #IFS is internal Filed separator, default is SP

cell="123-456-789"
IFS='-'
for num in $cell; do    123
	echo ${num}     456
done			789

for i in $(seq 0 ${#greeting}); do    #each line is a character
	echo "${greeting:i:1}"		#IFS='' not work
done

# Arrays
fruit=("apple" "banana" "cherry")
echo ${fruit[0]}     	#apple
echo ${fruit[1]}	#banana
echo ${fruit[@]}	#apple banana cherry
echo ${fruit}		#apple
for fruit in ${fruits[@]}; do
	echo ${fruit}
done

declare -a hw		#declare an array hw
hw[0]="hello"
echo ${hw[@]}		#hello
hw+=("world")		#appending
echo ${hw[@]}		#hello world    you can add 1 to hello

# Function
equal() {
	if [ $1 -eq $2 ]; then     #arguments $1...
		return 0		#exit success
	else
		return 1		
	fi
}
equal 5 9			no return, no x=equal 5 9
echo $?			#1
equal 5 "5"
echo $?			#0 they are equal
```

### Customize Command in ~/./profile
`source filename` source命令通常用于重新执行刚修改的初始化文件，使之立即生效，而不必注销并重新登录。该命令通常用命令“.”来替代  
`./profile` will execute everytime login  
```
# ~/./profile
up(){			#Usage: up 2     cd .. cd ..
	local limit=1
	if [ $# -eq 1 ]; then
		limit=$1
	fi
	for i in $(seq 1 $limit); do
		cd ..
	done
}
```


### [Regular Expression Cheat Sheet](https://www.rexegg.com/regex-quickstart.html)
- to match '-' in Character set [-abc] or [abc-]
- POSIX Bracket Expression
- [ab[:digit:]] is same as [ab0-9]
- Capture groups (abc){3} matches abcabcabc, \# match precious capture group, start from 1 
	`<([A-Z][A-Z0-9]*)\b[^>]*>.*?,/|1>` match HEML tag
- `yes|no` match `yes` or `no`
- `G(oo)+gle` match even number of `o`
- `h(im|er)` him or her
- `\d{3}-\d{3}-\d{4}` 123-456-7890 phone number

> Basic vs. Entended Regular Expressions

- Basic regular expression(BRE): ? + {} () | are nomal, escape as metacharacter
- Extended reuglar expression(ERE)

### Exercise
- match favorite or favourite
	- `favorite|favourite`
	- `favou?rite`
- match variation of Google with 2-7 os
	- `Go{2,7}gle`
- match line where the last word ens in 'st' or 'de' and the line optitional end in punc
	- `^.*(st|de)[\.,';]?$`
	- `[(st)(de)] is invalid`
- match binary string of 00111, 001 even 0 odd 1
	- `^(00)*1(11)*$`
- match line that is exactly an int with an optitional leading + or -
	- `^[+-]?(0|([1-9][[:digit:]]*))$`
- [Regex Test](https://regexr.com/)	

------------------------------------------------------------
## Patches Compilation and Makefile
Back to Content Tabble](#linux-enviroment-experiment)

### Diff and patches
- `diff -u file1 file2`
	- Comapre the difference in files line by line
	- -u output in diff formatc
```
--- path/to/originalfile	last modified data
+++ path/to/modifiedfile	last modified data
@@ -startingline,endingline +startingline,endline @@
- line deleted from original
+ line added to the original
an unchanged line
```
- `patch -p[N] < patch_file`
	- look at patchfile/diff file to see what N to use
	- use N=1 to modify original/file to modifide/file

### Compiled language
- +Fast, -test changes require recompilation, -Less portable. Generated binary
is platform dependent.
- Compilers translate source code into code that a computer can quickly
read and execute
- Source Code → Intermediate Representation → Assembly for Target
Architecture → Binary Executable
- Examples of compiled languages are C, C++, C#, Rust, Haskell, Swift
- source code → preprocessor → compiler -S → assembler -c → linker

### Interpreted language
- -Slower performance due to overhead of interpretation, +Flexible development/ testing, +Platform independent
- An interpreter directly executes source code line by line
- Interprets each line into commands at runtime
- Examples of interpreted languages are Python, JavaScript, Ruby, PHP

### Makefile
```
CC=g++ # compiler
CFLAGS=-Wall -g # compiler flags

# Running `make` (no target) defaults to the first target, conventionally "all"
all: bookstore

bookstore: coffee.o book.o store.o # final binary depends on object files
$(CC) $(CFLAGS) -o bookstore coffee.o book.o bookstore.o

coffee.o: coffee.cpp coffee.h
$(CC) $(CFLAGS) -c coffee.cpp # -c says not to run linker; so just creates .o file

book.o: book.cpp book.h
$(CC) $(CFLAGS) -c book.cpp

store.o: store.cpp book.h coffee.h
$(CC) $(CFLAGS) -c store.cpp

$Makefile for dynamic linking
randmain : randmain.o randcpuid.o
	gcc $(CFLAGS) $^ -ldl -Wl,-rpath,. -o $@

randlibhw.so : randlibhw.c
	gcc $(CFLAGS) $^ -shared -fPIC -o $@

randlibsw.so : randlibsw.c
	gcc $(CFLAGS) $^ -shared -fPIC -o $@

clean: # cleans generated build files
rm -f coffee.o book.o bookstore.o bookstore
```
### Build process
1. ./configure
	- Checks for build dependencies using configure.ac and creates the Makefile. Alternatively, write the Makefile by hand if needed.
	- --prefix=/some/path specifies path for binaries instead of the default path, which one may not have permission to write to.
2. make 
	- Runs the default target, which builds the executables specified by the Makefile
3. make install
	- Copies binaries generated by make to a target install location
	- If ./configure specified --prefix, uses that path



-------------------------------------------------------------
## C basic
[Back to Content Tabble](#linux-enviroment-experiment)

### pointer and reference
```
int *ptr;
int x = 77
ptr = &x;
*ptr++;

void increment(int *num){ *num++; }
int x = 10;
increment(&x);

#function pointer
int add(int x, int y){ return x + y; }
int main(void){
	int (*add_fn_ptr)(int, int) = &add;
	int sum = (*add_fn_ptr)(2, 3);
}

#qsort
int compare(const void * a, const void * b){ //generic pointer type to fit qsort
	return (*(int *)a - *(int *)b);    //cast to int pointer, then defference
}
int main(void){
	int values[] = {40, 10, 100, 90, 20, 25};
	qsort(values, 6, sizeof(int), compare);
	for(int i = 0; i < 6; ++i){
		printf("%d\n", values[i]);
	}
}
```

### Struct
```
struct Point {
	int x;
	int y;
}
struct Point p1 = {1, 2}; //Initialize (ordered)
p1.x = 3;
struct Point p3 = {.x = 5, .y = 6};   //alternative initialize

typedef struct Point {
	int x;
	int y;
} Mypoint;
Mypoint p1 = {1, 2};
```

### Standard IO
- `printf("x is %d", x)` always to stdout
- `fprintf(FILE *fptr, const char * format, ...);`
	- Prints to a file using a file pointer (stdout, stderr, and stdin are defined in <stdio.h>)
- `sprintf(char *str, const char *format, ...);`
	- Prints to a string buffer (i.e. a char array)
- `snprintf(char *str, size_t size, const char *format, ...);`
	- Prints a maximum of size characters to string buffer (safer than sprintf) prevert buffer overflow
```
char buffer[50];
sprintf(buffer, "Jack");
fprintf(stdout, "Hello, %s\n", buffer);
```

- `getchar` `putchar` Functions to read/write a single character from stdin/stdout
```
#include <stdio.h>
// implements cat: copies stdin to stdout
int main(void){
	char c;
	while ((c = getchar()) != EOF) // EOF indicates end of file
		putchar(c);
}
```

### Dynamic memory allocation
- Four function in <stdlib.h>
	- malloc() allocate
	- free()   dellocate
	- calloc() like malloc, but intialize to 0
	- realloc  rellocate previous block with new size
- Use pointer arithmetic or ptr[index] to access elements in block
- malloc/free
	- `ptr = (type *) malloc(number_of_bytes)`
```
char* letters; // Pointer for memory block
int n = 5; // Number of array elements
letters = (char *) malloc(n * sizeof(char));
if (letters == NULL)// Check memory allocated correctly
printf("Error allocating memory\n");
free(letters); // Deallocate
```
- realloc
	- `ptr = realloc(ptr, new_byte_number)`
	- Address migth be different, but content of memory will be preserved
```
int *arr = (int *) malloc(sizeof(int) * 10);
arr = realloc(arr, sizeof(int) * 100);
```
- Important notes
	- malloc/realloc do not initialize memory, will contain garbage
	- Forgetting to free causes a memory leak
	- Double free (freeing same location twice) causes undefined behavior
	- Use after free (access dangling pointer) also causes undefined behavior

------------------------------------------------------------
## System Call Programming
[Back to Content Tabble](#linux-enviroment-experiment)

### Kernel 
- A kernel is a program that serves as the core of the OS
- Has complete control over system and provides services to control CPU, Ememory, processes, disk, I/O
- On computer boot up, the kernel is the first program loaded into memory
- If the kernel crashes, the whole computer crashes too!
- Funtion
	- Memory Management – Keeps track of memory locations, and how much memory is being used to store what
	- Process Management and Scheduling – Determine what processes can use the CPU, when, and for how long
	- Device Drivers – Acts as the interpreter between hardware and software
- Kernel vs. user mode
	- Kernel Mode 0, User Mode 1 
	- CPU has two processor modes: user & kernel mode. A mode bit toggles execution between the two modes.
	- Memory is divided into kernel space and user space, specifying where kernel mode and user mode instructions are executed in
	- Privileged instructions
		- Execute any instruction available in hardware
		- Perform I/O or disk operations
		- Access entire memory space
		- Run other critical tasks
	- All other programs run in user mode, which has fewer privileges
		- Limited access to memory: prevents programs from overwriting each other’s memory
		- Buggy programs won't crash the entire computer (since a crash in kernel mode would)

### System calls
- User programs can use system calls to request the kernel to perform privileged operations on their behalf
- If the OS allows the system call, it causes a trap, which interrupts the user process and switches to kernel mode
- The kernel executes the system call, and afterwards, switches back to user mode to continue execution
- Switching modes for a system call incurs overhead

### Commands set1
- `strace ./program`  Intercepts and prints out system calls to stderr or to an output file
	-  `strace –o strace_output ./my_program`
	-  `strace -c` prints out a more readable summary report
- Use functions in <unistd.h> which are wrappers around system call implementations by glibc
	- `write(1, "Hello world!\n", 14)` write 14 byte to 1(stdout)
- Use syscall() in <sys/syscall.h> with the system call number
	- System call numbers can be viewed in /usr/include/asm/unistd_64.h
	- `syscall(SYS_write, 1, "Hello world!\n", 14)`  SYS_write is #define as 1, the syscall number, 1 is stdout
- Write the assembly instructions

## Buffed Unbuffered IO
- Every system call incurs overhead, requiring a switch from user mode, to kernel mode, and back to user mode
- For I/O like reading or writing to disk, we should buffer operations to reduce system calls and improve performance
- Unbuffered I/O issues a system call for each byte read/written
- Buffered I/O collects as many bytes as possible in a buffer (for read or write) and then makes one system call
- Buffered operations are more efficient, while unbuffered operations are instantaneous

- `time COMMAND` of `time ./PROGRAM`
	- Outputs real, user, and sys timing statistics
	- real: elapsed time as read from a wall clock
	- user: CPU time spent in user-mode code (outside the kernel) within the process
	- sys: CPU time spent in the kernel within the process (i.e., system calls)

### Buffered/unbuffered behavior in Linux
- stdout is usually line buffered by default. We assume there is a large amount of data going through, and it can wait momentarily until a buffer is collected.
	- Line buffered means we output the buffer at an \n character
	- `fflush(File *stream)` — forces a write of all buffered data
- stderr is unbuffered by default. We assume errors are infrequent, but we want to know about them immediately.

### Library calls
- Library calls abstract the implementation of making the system calls
- For example, putchar and printf are library calls that perform privileged operations like printing to the screen using system calls
- Typically more efficient, since they optimize the number of system calls made, e.g., by using buffered I/O
	- fopen() is a library call and uses buffered I/O
	- open() is a system call and uses unbuffered I/O
	
### Types of system calls
- Process control – need to alter process execution of a running program
- File management – create/delete/read/write to files
- Device management – manipulate peripherals
- Information management – retrieve information from OS like time/date
- Communication – create interprocess communication channels

### System call Commands
```
#include <unistd.h>

pid_t getpid(void) // returns the process id of the calling process

int dup(int fd) // duplicates a file descriptor fd

int fstat(int fd, struct stat *buf) // Returns information about the file with the descriptor fd into buf

ssize_t read(int fd, void *buffer, size_t n) // read n number of bytes from file described by fd to buffer

ssize_t write(int fd, void *buffer, size_t n) // write n number of bytes from file described by fd to buffer

int open(const char *pathname, int flags) // given a pathname for a file, returns a file descriptor (int)

int close(int fd) // closes given file descriptor

```
	



-------------------------------------------------------------
## Linking
[Back to Content Tabble](#linux-enviroment-experiment)

`#include "add.h"` 

### Static:  

`gcc -c add.c -o libadd.o`  
`ar rcs libadd.a libadd.o`  
`gcc -c main.c -o main.o`  
`gcc main.o libadd.o`  

### Dynamic:  

`gcc -fPIC -c add.c  
`gcc -shared -Wl,-soname,libadd.so.1 -o libadd.so.1.0 add.o`  
`ln -sf libadd.so.1.0 libadd.so.1`	soname  
`ln -sf libadd.so.1 libadd.so`        link name  
`gcc -c main.c -o main.o`  
`gcc main.o -L. -Wl,-rpath,. -ladd`  
`ldd a.out	show dynamic library included`  

> Dynamic loading: Works with shared library

use `-ldl` flag   
`gcc main.c -ldl -Wl,-rpath,.`
```
/*        main.c        /*  
int main(void){  
	#include <stdio.h>  
	#include <dlfcn.h>  
	void *handle = dlopen("libadd.so", RTLD_LAZY);  
	if(!handle){  
		fputs(dlerror(), stderr);;  
		return 1;
	}
	int (*add)(int, int) = dlsym(handle, "add");
	char *error = dlerror();
	if(error != NULL){
		fputs(error, stderr);
		return 1;
	}
	int sum = add(2, 4);
	printf("%d\n", sum);
	dlclose(handle);
	return 0;
}
```  
`void * dlopen(const char *filename, int flag)` flag:`RTLD_NOW/RTLD_LAZY`  
`void * dlsym(void *handle, char *symbol)`  
`char * dlerror()`  
```
int dlclose(void *handle)
__attribute__((__constructor__))
void to_run_before(void){
	printf("Ran with dlopen\n");
}
__attribute__((__destructor__))
void to_run_after(void){
	printf("Ran with dlclose\n");
}
```


-------------------------------------------------------------
## Git
[Back to Content Tabble](#linux-enviroment-experiment)


	SSH keys with github: week2
setting->SSH and GPG keys->new ssh key->
~/./ssh/id

### Branches:

HEAD point to our currently active head
git checkout <name>    more HEAD to branch
git checkout -b
git branch -vv         show all branches
upstream???

Three way merge
fast-forward merge

git merge <branch>     joins brance into current checked-out branch
merge conflicts, happen in three way merge,two branch have different copy of the same file

git rebase <branch>
git checkout feature     rebase feature onto master
git rebase master		 
git checkout master      after rebase, get master to lattest commit
git merge feature

Referencing commits
we can refer to any single commit butots 40-char SHA-1 hash /check ppt

commit range:
double dot   
triple dot   XOR

Ancestry references
- caret(^) after a commit ref resolves to a parent commit
	 include a number after if get a different parent at a fork history
- tilde(~) after a commit ref resolve to 1st parent of commit
	including a number after it gets older generation in commit history
	
`git format-patch -n <object>`
	* n specifies how many commit should be included in to the patch
	* <object>  can be a single commit or range
	* by default print the patch filename, `--stdout` prints patch contents
	* apply the patch to your working directory using git am < file.patch
									     
.git directory
- `cd .git`
- `ls`
- .git/objects/ store commit,tree,blob as subfolder
- .git/refs   .git/HEAD file is a symbolic ref to our current ref
- .git/indx   a binary ifle that act as the staging area(also called index)
	`git ls-files --stage` 
- .git/log	

Git + Graphs
- directed graph, directed acyclic graph(DAG)
- git commit history can be represented as a DAG

Topological sorting   parent apears before child
- modified depth-first-search

	
### Git Data Model:

commit object point to tree point to two blobs

### Other:

tag: head, a pointer to commits in git history
git tag       			view all tags 
git stash list
git stash apply
git apply
./gitignore 			file that you don't want to track, like intermediate files





### Commands and basic scripting

* Unix wildcards, basic regular expressions
* More advanced commands (e.g., [grep](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/grep.html), [find](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/find.html))
* Pipelines and redirection
* Simple shell scripting
* Idea of interpreted languages

### More scripting, VMs, and construction tools

* Basic of [Python](https://python.org/)
* [Java](http://www.oracle.com/technetwork/java/index.html) as a compromise between interpreted and compiled languages
* Building for source

### Cahnge management basics

* Basic of Makefiles
* [diff](https://www.gnu.org/software/diffutils/) and [patch](https://www.gnu.org/software/patch/)
* retrieving a package to build and install
* Basic version control, e.g.,[Git](https://git-scm.com/)

### Security basics

* Threats, including eavesdropping, tampering, forgery, and denial of service
* Authentication, authorization, and accounting
* Chains of trust
* Firewalls, kernels, and sandboxes
* Intrusion detection
* Backups
* Security polices

## Acknowledgments

* This project is based on CS35L UCLA.
