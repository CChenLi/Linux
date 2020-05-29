# Linux Enviroment Experiment
*This my class note for CS35L UCLA. The main purpose of this project is to construct enviroment under Linux 
system, practice working under Linux. The whole project is divided into 10 section. Each section has a specific
topic. Focusing on topic, explore methods finish certain task. There is a lab log for each task.*

----------------
## Content
* [Encryption and SSH](#Encryption-and-SSH-secure-shell)
* [Shell Scripts and Regular Expression](#Shell-Scripts-and-Regular-Expression)
* [Linking](#Linking)
* [Git](#Git)

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

### Using Linux with a shell
Shell is a interface where you can interact with Linux, bash, zsh

> File Paths
- Root dir is /
- User's homedir is ~ not a folder, but shell internally expand to home dir
- Absolute path, relative to root, start with /
- Relative path, relative to current dir

> Basic command

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
	
### Essential Linux directories  
Command | Definition
------ | ---------
/bin  | essential binaries (e.g. pwd, bash)
/dev  | devices (and virtual ones like /dev/null)
/home | users' home directories
/tmp  | temporary files
/usr  | user files and binaries (e.g. /usr/bin)

### Links
- Hard links points directly to inode on dist, This means multiple files can map to the same inode
- Symbolic(soft) links point to the path of the file
- Create link
	- Hard: `ln source link`
	- Soft: `ln -s source link`
	
```flow
st=>start: Symbolic link
e=>end: Disk
i=>start: Inode
file=>start: File
hl=>start: Hardlink
st->file->i->e
hl->i->e
```



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
