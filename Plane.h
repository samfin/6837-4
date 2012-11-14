#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
using namespace std;
///TODO: Implement Plane representing an infinite plane
///choose your representation , add more fields and fill in the functions
class Plane: public Object3D
{
public:
	Plane(){}
	Plane( const Vector3f& normal , float d , Material* m):Object3D(m), normal(normal), d(d) {
	    
	}
	~Plane(){}
	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		float a = d - Vector3f::dot(normal, r.getOrigin());
		float b = Vector3f::dot(normal, r.getDirection());
		// parallel case sucks, ignore it
		if(fabs(b) < 1e-5) return 0;
		float t = a / b;
		if(t < tmin || t >= h.getT()) return 0;
		h.set(t, material, normal);
		return 1;
	}

protected:
    Vector3f normal;
    float d;
};
#endif //PLANE_H
		

