# Note for Linux

`echo $PATH`         default path for command devided by :

`pg1 2> pg2`    write stderr from pg1 to pg2

`mkdir d` 
`stat d`      this shows Linksnumber 2, one is d, one is . inside d
`ln -s d soft`   create sofelink `soft` to d     ln is not allowed to hard link a dir


------------------------------------------------------------
## Encryption and SSH secure shell
>Cipher example
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

### lab2





-------------------------------------------------------------
## Linking
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
