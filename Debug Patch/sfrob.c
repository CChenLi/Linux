#include <stdlib.h>
#include <stdio.h>

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
  if((*a) == ' ' && (*b) != ' ') return	-1; //a is prefix of b
  if((*a) != ' ' && (*b) == ' ') return	1;  //b is prefix of a
}

int sortFunc(const void* a, const void* b){
  return frobcmp(*(char const**) a, *(char const**) b);
}



void memoryError(){
  fprintf(stderr, "Error: allocate memory Failed");
  exit(1);
}

int main(){
  char **lines = (char**) malloc(sizeof(char*));
  char *line = (char*) malloc(sizeof(char));
  lines[0] = line;
  if(lines == NULL || line == NULL){
    memoryError();
  }
  int row = 0; //for melloc, not index
  int col = 0;
  int bias = 0; 
  char letter = getchar();
  char nextchar;
  int t = 1;
  if(ferror(stdin)){
    fprintf(stderr, "Error: error indicator for stdin was set");
    exit(1);
  }

  //build 2-D array
  //build line until encounter space, store line into lines.
  //if next char is EOF, break
  //if next char is not EOF, intialize new line, increment row.
  //If encounter EOF during building line, store line in lines and break
  while(t == 1){
    if(letter != ' ' && letter != EOF){ //keep build current line
      line = realloc(line, (col+1) * sizeof(char));
      if(line == NULL) memoryError();
      //--------------------------
      line[col] = letter;
      col+=1;
      letter = getchar();
    }
    if(letter == ' ' && letter != EOF){ //\n encountered
      line = realloc(line, (col+1) * sizeof(char));
      if(line == NULL) memoryError();
      line[col] = ' ';
      lines[row] = line;//store current building line
      row +=1;
      nextchar = getchar();//check if nextchar is EOF
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
  qsort(lines, row + bias + 1, sizeof(char*),sortFunc);
  int i, j;
  //for(i=0; i <= row + bias; i++){
  //  printf("%s\n", lines[i]);
  //}
  for(i = 0; i <= row+bias; i++){ 
    j=0;
    while(t==1){
      if(lines[i][j] != ' '){
	putchar(lines[i][j]);
	if(ferror(stdout)){
	  fprintf(stderr, "Error: read output Falied");
	  exit(1);
	}
      }else{
	putchar(lines[i][j]);
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
  
  return 0;
}
