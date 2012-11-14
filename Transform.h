#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform: public Object3D
{
public: 
  Transform(){}
 Transform( const Matrix4f& matrix, Object3D* obj ):o(obj){
    m_inv = matrix.inverse(0, 1e-6);
    m_invt = m_inv.transposed();
  }
  ~Transform(){
  }
  virtual bool intersect( const Ray& r , Hit& h , float tmin){
    Vector3f old_o = r.getOrigin();
    Vector3f old_d = r.getDirection();
    
    Vector4f origin = Vector4f(old_o, 1);
    origin = m_inv * origin;
    Vector3f new_origin = Vector3f(origin[0] / origin[3], origin[1] / origin[3], origin[2] / origin[3]);
    Vector4f dir = Vector4f(old_d, 0);
    dir = m_inv * dir;
    Vector3f new_dir = Vector3f(dir[0], dir[1], dir[2]);
    Ray new_ray = Ray(new_origin, new_dir);
    bool b = o->intersect( new_ray , h , tmin);
    if(b) {
        Vector4f n = Vector4f(h.getNormal(), 0);
        n = m_invt * n;
        Vector3f new_n = Vector3f(n[0], n[1], n[2]);
        new_n.normalize();
        h.set(h.getT(), h.getMaterial(), new_n);
    }
    return b;
  }

 protected:
  Object3D* o; //un-transformed object
  Matrix4f m_inv;
  Matrix4f m_invt;
};

#endif //TRANSFORM_H
