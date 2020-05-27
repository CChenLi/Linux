# Note for Linux

* [Encryption and SSH](#Encryption-and-SSH-secure-shell)
* [Linking](#Linking)
* [Git](#Git)

`echo $PATH`         default path for command devided by :

`pg1 2> pg2`    write stderr from pg1 to pg2

`mkdir d` 
`stat d`      this shows Linksnumber 2, one is d, one is . inside d
`ln -s d soft`   create sofelink `soft` to d     ln is not allowed to hard link a dir


------------------------------------------------------------
## Encryption and SSH secure shell
[Back to Content Tabble](#note-for-linux)

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

### lab2 starting at lnxsrv09
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
## Linking
[Back to Content Tabble](#note-for-linux)

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
[Back to Content Tabble](#note-for-linux)


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
