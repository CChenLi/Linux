# Linux Enviroment Experiment
*This project is based on CS35L UCLA. The main purpose of this project is to construct enviroment under Linux 
system, practice working under Linux. The whole project is divided into 10 section. Each section has a specific
topic. Focusing on topic, explore methods finish certain task. There is a lab log for each task.*

----------------
## Content
* [Encryption and SSH](#Encryption-and-SSH-secure-shell)
* [Linking](#Linking)
* [Git](#Git)

`echo $PATH`         default path for command devided by :

`pg1 2> pg2`    write stderr from pg1 to pg2

`mkdir d` 
`stat d`      this shows Linksnumber 2, one is d, one is . inside d
`ln -s d soft`   create sofelink `soft` to d     ln is not allowed to hard link a dir

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
-
	
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

git merge <branch>     joins brance into current checked-out branch
merge conflicts, two branch have different copy of the same file

git rebase <branch>
git checkout feature     rebase feature onto master
git rebase master		 
git checkout master      after rebase, get master to lattest commit
git merge feature

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
