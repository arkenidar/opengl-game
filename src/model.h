#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

typedef struct {
  float array[3];
} vector;
typedef struct {
  vector* array;
  int count;
} vector_array;

typedef struct {
  int array[6];
} triangle;
typedef struct {
  triangle* array;
  int count;
} triangle_array;

typedef struct {
vector_array vertex_positions;
vector_array vertex_normals;
triangle_array mesh;
} model;

#endif // MODEL_H_INCLUDED
