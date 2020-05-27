#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char * argv[]){
	if(argc != 3){
		fprintf(stderr, "%s\n", "ERROR: usage: ./tr2b.c [from] [to]");
		exit(1);
	}

	const char* from = argv[1];
	const char* to = argv[2];

	unsigned long fromlen = strlen(from);
	unsigned long tolen = strlen(to);

	if(fromlen - tolen){
		fprintf(stderr, "%s\n", "ERROR: length of [from] differ from [to]");
		exit(1);
	}

	// this is at most 127*127 step
	unsigned long i, j;
	for(i = 1; i < fromlen; i++){ //remember to test fromlen = 0
		for(j = 0; j < i; j++){
			if(from[i] == from[j]){
				fprintf(stderr, "%s\n", "ERROR: duplicate in [from]");
			}
		}
	}
	int check, c;
	while(1 == 1){
		check = 0;
		ssize_t EOFcheck = read(STDIN_FILENO, &c, 1);
		if(EOFcheck <= 0) break;
		for(i = 0; i < fromlen; i++){
			if (c == from[i]){
				write(STDOUT_FILENO, to+i, 1);
				check = 1;
				break;
			}
		}
		if(check == 0) write(STDOUT_FILENO, &c, 1);
	}

	return 0;

}