#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
    return Vector3f();
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
    return 0;
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces) : scene(scene) {
  group = scene->getGroup();
  camera = scene->getCamera();
  m_maxBounces = max_bounces;
  for(int i = 0; i < scene->getNumLights(); i++) {
    lights.push_back(scene->getLight(i));
  }
  ambient = scene->getAmbientLight();
  
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay(Ray &ray, Hit &hit) const {
    return this->traceRay(ray, camera->getTMin(), m_maxBounces, 1, hit);
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit &hit) const
{
    if(bounces < 0) return Vector3f(0, 0, 0);
    hit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
    bool b = group->intersect(ray, hit, tmin);
    if(b) {
        Vector3f color = hit.getMaterial()->ambientShade(hit, ambient);
        for(int k = 0; k < lights.size(); k++) {
            Vector3f p = ray.pointAtParameter(hit.getT());
            Vector3f dir, col;
            float dist;
            lights[k]->getIllumination(p, dir, col, dist);
            color += hit.getMaterial()->Shade(ray, hit, dir, col);
        }
        return color;
    } else {
        return scene->getBackgroundColor(ray.getDirection());
    }
}
