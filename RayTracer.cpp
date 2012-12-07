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
    Vector3f v = incoming - 2 * Vector3f::dot(normal, incoming) * normal;
    v.normalize();
    return v;
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
    if(fabs(index_nt) < EPSILON) return 0;
    float d = Vector3f::dot(normal, incoming);
    float x = index_n / index_nt;
    float r = 1 - x * x * (1 - d*d);
    if(r < 0) return 0;
    r = sqrt(r);
    transmitted = x * (incoming - d * normal) - r * normal;
    transmitted.normalize();
    return 1;
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces, bool shadows) : scene(scene), shadows(shadows) {
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

bool RayTracer::is_shaded(Ray &ray, float dist) const {
    Hit hit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
    group->intersect(ray, hit, EPSILON);
    return hit.getT() < dist;
}

Vector3f RayTracer::traceRay(Ray &ray, Hit &hit) const {
    return this->traceRay(ray, camera->getTMin(), m_maxBounces, 1, hit);
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit &hit) const
{
    if(bounces < 0) return Vector3f(0, 0, 0);
    hit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
    if(group->intersect(ray, hit, tmin)) {
        Vector3f p = ray.pointAtParameter(hit.getT());
        // Get ambient color
        Vector3f color = hit.getMaterial()->ambientShade(hit, ambient);
        // Get reflection color
        Vector3f specular = hit.getMaterial()->getSpecularColor();
        // Check if we need to reflect
        if (bounces && (specular[0] || specular[1] || specular[2])) {
            Vector3f n = hit.getNormal();
            bool inside = Vector3f::dot(ray.getDirection(), n) > 0;
            float new_index = hit.getMaterial()->getRefractionIndex();
            if(inside == 1) {
                new_index = 1;
                n = -n;
            }
            // Get reflection color
            Vector3f mirror = mirrorDirection(n, ray.getDirection());
            Ray r = Ray(p, mirror);
            Hit tmp_h;
            Vector3f v = traceRay(r, EPSILON, bounces-1, refr_index, tmp_h);
            Vector3f reflection = Material::pointwiseDot(specular, v);
            // Get refraction color
            Vector3f r_dir;
            if(transmittedDirection(n, ray.getDirection(), refr_index, new_index, r_dir)) {
                r = Ray(p, r_dir);
                Hit tmp_h;
                Vector3f refraction = traceRay(r, EPSILON, bounces-1, new_index, tmp_h);
                refraction = Material::pointwiseDot(refraction, specular);
                // Compute weights
                float r0 = (new_index - refr_index) / (new_index + refr_index);
                r0 *= r0;
                float c;
                if(refr_index <= new_index)
                    c = fabs(Vector3f::dot(n, ray.getDirection()));
                else
                    c = fabs(Vector3f::dot(n, r_dir));
                float r = r0 + pow(1 - c, 5) * (1 - r0);
                color += r * reflection + (1-r) * refraction;
            } else {
                color += reflection;
            }
        }
        for(int k = 0; k < lights.size(); k++) {
            Vector3f dir, col;
            float dist;
            lights[k]->getIllumination(p, dir, col, dist);
            dir.normalize();
            // Check shadow ray
            if(shadows) {
                Ray toLight = Ray(p, dir);
                if(this->is_shaded(toLight, dist)) continue;
            }
            // Add illumination
            color += hit.getMaterial()->Shade(ray, hit, dir, col);
        }
        return color;
    } else {
        return scene->getBackgroundColor(ray.getDirection());
    }
}
