#ifndef HEADER_9E9A46FFAF03A2EA
#define HEADER_9E9A46FFAF03A2EA

// https://replit.com/@dariocangialosi/parsing-and-starts-with-checking#parse.h

#include <stdio.h>
#include <stdlib.h>
#include "model.h"

char* starts_with(const char* string, const char* what){
  char *string_cursor=(char*)string, *what_cursor=(char*)what;
  for(;;string_cursor++,what_cursor++){
    if(*what_cursor=='\0') return string_cursor;
    if(*what_cursor!='\0' && *string_cursor=='\0') return NULL;
    if(*string_cursor != *what_cursor) return NULL;
  }
  return string_cursor;
}

void test_scanning(){
  puts("test_scanning()\n");
  puts(starts_with("","abc")?"yes":"no"); // NO
  puts(starts_with("abcd","abc")?"yes":"no"); // YES
  puts(starts_with("x","abc")?"yes":"no"); // NO
  puts(starts_with("abc","abc")?"yes":"no"); // YES
  puts("");
}

char* next_int(const char* next,int* out){
  int number=0;
  int sign=1; if(*next=='-'){ sign=-1; next++; }
  for(; *next>='0' && *next<='9'; next++){
    int digit=*next-'0';
    number*=10; number+=digit;
  }
  number*=sign;
  *out=number; return (char*)next;
}
char* next_float(const char* next,float* out){
  float number=0; int dot=-1;
  int sign=1; if(*next=='-'){ sign=-1; next++; }
  for(; *next=='.' || (*next>='0' && *next<='9'); next++){
    if(*next=='.'){ dot=0; continue; } if(dot>=0) dot++;
    int digit=*next-'0';
    number*=10; number+=digit;
  }
  while(0<dot--)number/=10;
  number*=sign;
  *out=number; return (char*)next;
}
void test_numbers(){
  puts("test_numbers()\n");

  int out_int;
  next_int("-1234",&out_int);
  printf("out_int: [[%d]]\n",out_int);

  float out_float;
  next_float("-12.34",&out_float);
  printf("out_float: [[%f]]\n",out_float);

  puts("");
}

/* example for parsing of dot-obj files:
v -0.255580 2.190439 0.895723
vn 0.4948 0.1027 0.8629
f 420//1 14252//2 4979//3
*/
model load_model_obj(const char* file_name){ // "main.c" -> "parse.h" -> "head.obj"

  puts(file_name);

  model loaded;
  loaded.mesh.array=NULL; loaded.mesh.count=0;
  loaded.vertex_positions.array=NULL; loaded.vertex_positions.count=0;
  loaded.vertex_normals.array=NULL; loaded.vertex_normals.count=0;

  FILE* file = fopen(file_name,"r"); char line[1001]; char* next;
  ///int face_data[6]={0}; float vector_data_NO_USE[3]={0};
  while(fgets(line,1001,file)){
    ///printf("%s",line);

    char* prefix; int print=0;

    if( (next=starts_with(line,"v ")) ){
      prefix=(char*)"v ";
      if(print) printf("FOUND: %s%s", prefix, next );

      loaded.vertex_positions.count++;
      loaded.vertex_positions.array=(vector*)realloc(
        loaded.vertex_positions.array, loaded.vertex_positions.count*sizeof(vector));

      float* vector_data=loaded.vertex_positions.array[loaded.vertex_positions.count-1].array;

      next=next_float(next,vector_data+0);
      next=starts_with(next," ");
      next=next_float(next,vector_data+1);
      next=starts_with(next," ");
      next=next_float(next,vector_data+2);

      if(print) printf("[[%s%f %f %f]]\n\n",prefix,
         vector_data[0],vector_data[1],vector_data[2]);

    }
    else
    if( (next=starts_with(line,"vn ")) ){
      prefix=(char*)"vn ";
      if(print) printf("FOUND: %s%s", prefix, next );

      loaded.vertex_normals.count++;
      loaded.vertex_normals.array=(vector*)realloc(
        loaded.vertex_normals.array, loaded.vertex_normals.count*sizeof(vector));

      float* vector_data=loaded.vertex_normals.array[loaded.vertex_normals.count-1].array;

      next=next_float(next,vector_data+0);
      next=starts_with(next," ");
      next=next_float(next,vector_data+1);
      next=starts_with(next," ");
      next=next_float(next,vector_data+2);

      if(print) printf("[[%s%f %f %f]]\n\n",prefix,
         vector_data[0],vector_data[1],vector_data[2]);

    }
    else
    if( (next=starts_with(line,"f ")) ){
      prefix=(char*)"f ";
      if(print) printf("FOUND: %s%s", prefix, next );

      loaded.mesh.count++;
      loaded.mesh.array=(triangle*)realloc(
        loaded.mesh.array, loaded.mesh.count*sizeof(triangle));

      int* face_data=loaded.mesh.array[loaded.mesh.count-1].array;

      next=next_int(next,face_data+0);
      next=starts_with(next,"//");
      next=next_int(next,face_data+1);

      next=starts_with(next," ");
      next=next_int(next,face_data+2);
      next=starts_with(next,"//");
      next=next_int(next,face_data+3);

      next=starts_with(next," ");
      next=next_int(next,face_data+4);
      next=starts_with(next,"//");
      next=next_int(next,face_data+5);

      if(print) printf("[[f %d//%d %d//%d %d//%d]]]\n\n",
         face_data[0],face_data[1],face_data[2],face_data[3],face_data[4],face_data[5]);
    }
  }
  fclose(file);

  puts("loaded!");

  return loaded;
}

void test_obj_file(){
  puts("test_obj_file()\n");
  load_model_obj("parse.h");
  puts("");
}

#endif // header guard
