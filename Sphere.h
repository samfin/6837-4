#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	Sphere(){ 
		//unit ball at the center
		center = Vector3f(0, 0, 0);
		radius = 1;
		material = 0;
	}

	Sphere( Vector3f center , float radius , Material* material ):center(center), radius(radius), Object3D(material){
        
	}
	

	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
        Vector3f x = r.getOrigin() - center;
        Vector3f y = r.getDirection();
        float a = Vector3f::dot(y, y);
        float b = 2 * Vector3f::dot(x, y);
        float c = Vector3f::dot(x, x) - radius*radius;
        float d = b*b-4*a*c;
        if(d < 0) return 0;
        a *= 2;
        b = -b / a;
        d = sqrt(d) / a;
        float t0 = b - d;
        float t1 = b + d;
        float t;
        if(t0 >= tmin)
            t = t0;
        else if(t1 >= tmin)
            t = t1;
        else
            return 0;
        if(t >= h.getT())
            return 0;
        // intersection found
        Vector3f v = r.pointAtParameter(t);
        // cout << v[0] << " " << v[1] << " " << v[2] << "\n";
        Vector3f n = r.pointAtParameter(t) - center;
        n.normalize();
        h.set(t, material, n);
        return 1;
	}

protected:
	Vector3f center;
	float radius;
	Material *material;
};


#endif
