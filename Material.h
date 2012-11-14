#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture

class Material
{
public:
	
 Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
  diffuseColor( d_color),specularColor(s_color), shininess(s)
  {
    
  }

  virtual ~Material()
    {

    }

  virtual Vector3f getDiffuseColor() const 
  { 
    return  diffuseColor;
  }
    
  Vector3f ambientShade(const Hit& hit, const Vector3f& ambient) {
    float k_a = 1.0;
    Vector3f d_col = diffuseColor;
    if(hit.hasTex && t.valid()) {
        d_col = t(hit.texCoord[0], hit.texCoord[1]);
    }
    return k_a * Vector3f(ambient[0] * d_col[0], ambient[1] * d_col[1], ambient[2] * d_col[2]);
  }

  Vector3f Shade( const Ray& ray, const Hit& hit,
                  const Vector3f& dirToLight, const Vector3f& lightColor ) {

    float k_d = 1.0, k_s = 1.0;
    Vector3f n = hit.getNormal();
    n.normalize();
    float d = Vector3f::dot(dirToLight, n);
    if(d < 0) d = 0;
    Vector3f d_col = diffuseColor;
    if(hit.hasTex && t.valid()) {
        d_col = t(hit.texCoord[0], hit.texCoord[1]);
    }
    Vector3f diffuse = d * Vector3f(lightColor[0] * d_col[0], lightColor[1] * d_col[1], lightColor[2] * d_col[2]);
    Vector3f r = ray.getDirection();
    r = r - 2 * Vector3f::dot(r, n) * n;
    r.normalize();
    float s = Vector3f::dot(dirToLight, r);
    if(s > 0) s = pow(s, shininess);
    else s = 0;
    Vector3f specular = s * Vector3f(lightColor[0] * specularColor[0], lightColor[1] * specularColor[1], lightColor[2] * specularColor[2]);
    return k_d * diffuse + k_s * specular;
  }

  void loadTexture(const char * filename){
    t.load(filename);
  }
 protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};



#endif // MATERIAL_H
