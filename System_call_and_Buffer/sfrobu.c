#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int frobcmpF(char const* a, char const* b){
  char defa, defb;
  while(*a != ' ' && *b != ' '){
    defa = toupper((*a) ^ 42);
    defb = toupper((*b) ^ 42);
    if(defa == defb){
      a+=1;
      b+=1;
    }
    if(defa > defb) return 1;
    if(defa < defb) return -1;
  }
  //reach end of a or b
  if((*a) == ' ' && (*b) == ' ') return 0;
  if((*a) == ' ' && (*b) != ' ') return	-1; //a is prefix of b
  if((*a) != ' ' && (*b) == ' ') return	1;  //b is prefix of a
}

int frobcmp(char const* a, char const* b){
  char defa, defb;
  while(*a != ' ' && *b != ' '){
    defa = (*a) ^ 42;
    defb = (*b) ^ 42;
    if(defa == defb){
      a+=1;
      b+=1;
    }
    if(defa > defb) return 1;
    if(defa < defb) return -1;
  }
  //reach end of a or b
  if((*a) == ' ' && (*b) == ' ') return 0;
  if((*a) == ' ' && (*b) != ' ') return -1; //a is prefix of b
  if((*a) != ' ' && (*b) == ' ') return 1;  //b is prefix of a
}

int sortFuncF(const void* a, const void* b){
    return frobcmpF(*(char const**) a, *(char const**) b);
}

int sortFunc(const void* a, const void* b){
    return frobcmp(*(char const**) a, *(char const**) b);
}

void IOError(){
  fprintf(stderr, "Unable to read");
  exit(1);
}


void memoryError(){
  fprintf(stderr, "Error: allocate memory Failed");
  exit(1);
}

int main(int argc, const char* argv[]){
  int caseignore = 1;
  if(argc==2 && (strcmp(argv[1], "-f") != 0)) caseignore = 0;
  if(argc != 2) caseignore = 0;


  ssize_t ret;
  int isEOF = 0;
  size_t filesize = 8 * 1024; // mind this line ----------------------
  int bufsize = 0;
  char curchar;
  int offset = 0;
  // retrive the size of input file
  struct stat fileData;
  if(fstat(STDIN_FILENO, &fileData) < 0){
    fprintf(stderr, "fstat error");
    exit(1);
  }

  if(S_ISREG(fileData.st_mode) != 0){ // regular file
    filesize = fileData.st_size;
    if(filesize == 0) return 0;
  }     
  char *buffer = (char*) malloc(filesize * sizeof(char));
  char *buffer2;
  // read input into buffer.
  do {
    ret = read(STDIN_FILENO, buffer+offset, filesize/2);
    if(ret < 0) IOError();
    curchar = buffer[ret-1];
    isEOF = (!ret);
    //printf("RET: %zd\n", ret);
    offset += ret;

    if(offset == filesize){ // if 
      filesize *= 2;
      buffer2 = (char*) realloc(buffer, filesize * sizeof(char));
      buffer = buffer2;
    }
  } while(!isEOF);
  buffer[offset] = EOF;

  if(offset == 0){
    return 0;
  }

//-----------------------------------------------------
//printf("%s\n", "Content of buffer:");
int i, j;
//for(i = 0; i < offset; i++){
//  putchar(buffer[i]);
//}
//printf("\noffset: %d\n",  offset);
//-----------------------------------------------------






  // Load content in buffer into 2_D array
  char **lines = (char**) malloc(sizeof(char*));
  char *line = (char*) malloc(sizeof(char));
  lines[0] = line;
  if(lines == NULL || line == NULL){
    memoryError();
  }
  int row = 0; //for melloc, not index
  int col = 0;
  int bias = 0; 


  char letter = buffer[0]; // Adjust later for empty buffer
  char nextchar;

  //build 2-D array
  //build line until encounter space, store line into lines.
  //if next char is EOF, break
  //if next char is not EOF, intialize new line, increment row.
  //If encounter EOF during building line, store line in lines and break
  size_t idx = 0;
  while(1 == 1){
    if(letter != ' ' && letter != EOF){ //keep build current line
      line = realloc(line, (col+1) * sizeof(char));
      if(line == NULL) memoryError();
      //--------------------------
      line[col] = letter;
      col+=1;
      letter = buffer[++idx];
    }
    if(letter == ' ' && letter != EOF){ //\n encountered
      line = realloc(line, (col+1) * sizeof(char));
      if(line == NULL) memoryError();
      line[col] = ' ';
      lines[row] = line;//store current building line
      row +=1;
      nextchar = buffer[++idx];//check if nextchar is EOF
      if(nextchar == EOF){//break if nextchar is EOF
        bias = -1;
        break;
      } else{//create one more row and start to build a new line;
        lines = realloc(lines, (row+1) * sizeof(char*));
        letter = nextchar;
        line = (char*) malloc(sizeof(char));//start a new line
        if(line == NULL || lines == NULL) memoryError();
        col = 0;
      }
    }
    if(letter == EOF){
      line = realloc(line, (col+1) * sizeof(char));
      if(line == NULL) memoryError();
      line[col] = ' ';
      lines[row] = line;
      break;
    }
  }

  //for(i=0; i <= row + bias; i++){
  //  printf("%s\n", lines[i]);
  //}


  if(caseignore==1){
    qsort(lines, row + bias + 1, sizeof(char*),sortFuncF);
  } else{
    qsort(lines, row + bias + 1, sizeof(char*),sortFunc);
  }
  
  //for(i=0; i <= row + bias; i++){
  //  printf("%s\n", lines[i]);
  //}

  int afterSP = 1;
  for(i = 0; i <= row+bias; i++){ 
    j=0;
    while(1==1){
      if(lines[i][j] != ' '){
	afterSP = 0;
	//putchar(lines[i][j]);
	write(1, &lines[i][j],1);
        if(ferror(stdout)){
          fprintf(stderr, "Error: read output Falied");
          exit(1);
        }
      }else{
	if(afterSP == 0) {
	  //putchar(lines[i][j]);
	  write(1, &lines[i][j],1);
	  afterSP = 1;
	}
	  if(ferror(stdout)){
            fprintf(stderr, "Error: read output Falied");
            exit(1);
          }
        break;
      }
      j++;
    }
    free(lines[i]);
  }
  free(lines); 
  free(buffer);
  return 0;
}
