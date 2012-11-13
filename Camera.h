#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle): center(center){
        w = direction;
        u = Vector3f::cross(w, up);
        v = Vector3f::cross(u, w);
        u.normalize();
        v.normalize();
        w.normalize();
        w *= 1 / tan(angle / 2);
	}

	virtual Ray generateRay( const Vector2f& point){
		Vector3f x = point[0] * u + point[1] * v + w;
		x.normalize();
		return Ray(center, x);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
    Vector3f center;
    Vector3f u, v, w;
};

#endif //CAMERA_H
