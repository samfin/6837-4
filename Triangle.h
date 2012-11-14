#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle: public Object3D
{
public:
	Triangle();
        ///@param a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object3D(m), a(a) {
	    this->b = a - b;
	    this->c = a - c;
        hasTex = false;
	}

	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin){
	    Vector3f rhs = a - ray.getOrigin();
	    Vector3f dir = ray.getDirection();
		Matrix3f m = Matrix3f(b, c, dir, 1);
		bool isSingular;
		m = m.inverse(&isSingular, 1e-5);
		if(isSingular) return 0;
		Vector3f coords = m * rhs;
		if(coords[2] < tmin || coords[2] >= hit.getT()) return 0;
		float alpha = 1 - coords[0] - coords[1];
		if(coords[0] < 0 || coords[1] < 0 || alpha < 0) return 0;
		Vector3f normal = normals[0] * alpha + normals[1] * coords[0] + normals[2] * coords[1];
		hit.set(coords[2], material, normal);
		if(hasTex) {
		    Vector2f u = texCoords[0] * alpha + texCoords[1] * coords[0] + texCoords[2] * coords[1];
		    hit.setTexCoord(u);
		}
		return 1;
	}
	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];
protected:
    Vector3f a, b, c;
};

#endif //TRIANGLE_H
