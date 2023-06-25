#ifndef HEADER_952350C5AED62F44
#define HEADER_952350C5AED62F44

typedef struct vector3_struct{

  float vector3_array[3];

  float& operator[](int index){
    return vector3_array[index];
  }

  vector3_struct(){
    float zero[3]={0,0,0};
    set_value_from(zero);
  }

  struct vector3_struct vsum(struct vector3_struct v1, struct vector3_struct v2){
    struct vector3_struct vr;
    vr[0]=v1[0]+v2[0];
    vr[1]=v1[1]+v2[1];
    vr[2]=v1[2]+v2[2];
    return vr;
  }

  struct vector3_struct operator+(struct vector3_struct other){
    return this->vsum(*this,other);
  }

  void set_value_from(float in[3]){
    vector3_array[0]=in[0];
    vector3_array[1]=in[1];
    vector3_array[2]=in[2];
  }

  vector3_struct(float in[3]){
    set_value_from(in);
  }

  struct vector3_struct vmultiply(struct vector3_struct v1, float factor){
    struct vector3_struct vr;
    vr[0]=v1[0]*factor;
    vr[1]=v1[1]*factor;
    vr[2]=v1[2]*factor;
    return vr;
  }

  struct vector3_struct operator*(float factor){
    return this->vmultiply(*this,factor);
  }

} vector3;
#endif // header guard

