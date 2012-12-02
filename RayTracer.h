#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <cassert>
#include <vector>
#include "SceneParser.h"
#include "Ray.h"
#include "Hit.h"

class SceneParser;


class RayTracer
{
public:
  
  RayTracer()
  {
      assert( false );
  }

  RayTracer( SceneParser* scene, int max_bounces //more arguments as you need...
  );
  ~RayTracer();
  
  Vector3f traceRay( Ray& ray, float tmin, int bounces, 
                     float refr_index, Hit& hit ) const;
  Vector3f traceRay(Ray& ray, Hit& hit) const;
  bool is_shaded(Ray& ray) const;
private:
  SceneParser* scene;

  int m_maxBounces;
  Group *group;
  Camera *camera;
  vector<Light*> lights;
  Vector3f ambient;
};

#endif // RAY_TRACER_H
