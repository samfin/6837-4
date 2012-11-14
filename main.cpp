#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax) {
    return 1 - (max(min(depthInput, depthMax), depthMin) - depthMin) / (depthMax - depthMin);
}

struct args {
    char *input;
    int w, h;
    char *output;
    int dmin, dmax;
    char *depth;
};

int parse_int(char *x) {
    int k = 0, i = 0;
    while(x[i]) {
        k = 10 * k + (x[i++] - '0');
    }
    return k;
}

void get_args(int argc, char *argv[], args &args) {
    args.depth = 0;
    for(int i = 1; i < argc; ) {
        if(strcmp(argv[i],"-input")==0) {
            args.input = argv[i+1];
            i += 2;
        } else if(strcmp(argv[i],"-size")==0) {
            args.w = parse_int(argv[i+1]);
            args.h = parse_int(argv[i+2]);
            i += 3;
        } else if(strcmp(argv[i],"-output")==0) {
            args.output = argv[i+1];
            i += 2;
        } else if(strcmp(argv[i],"-depth")==0) {
            args.dmin = parse_int(argv[i+1]);
            args.dmax = parse_int(argv[i+2]);
            args.depth = argv[i+3];
            i += 4;
        } else {
            cout << "Could not recognize argument " << argv[i] << "\n";
            i++;
        }
    }
}

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  for( int argNum = 1; argNum < argc; ++argNum )
    {
      std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
  args x;
  get_args(argc, argv, x);
    
  // First, parse the scene using SceneParser.
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.

  SceneParser *parser = new SceneParser(x.input);
  Image image(x.w, x.h);
  Image depth(x.w, x.h);
  float k_a = 1.0, k_d = 1.0;
  Camera *camera = parser->getCamera();
  Group *group = parser->getGroup();
  vector<Light*> lights;
  for(int i = 0; i < parser->getNumLights(); i++) {
    lights.push_back(parser->getLight(i));
  }
  Vector3f ambient = k_a * parser->getAmbientLight();
  Vector3f background = parser->getBackgroundColor();
  for(int i = 0; i < x.w; i++) {
    for(int j = 0; j < x.h; j++) {
        float x0 = i * 2.0 / (x.w - 1) - 1;
        float x1 = j * 2.0 / (x.w - 1) - 1;
        Ray r = camera->generateRay(Vector2f(x0, x1));
        Hit h;
        bool b = group->intersect(r, h, camera->getTMin());
        if(b) {
            Vector3f color = ambient;
            for(int k = 0; k < lights.size(); k++) {
                Vector3f p = r.pointAtParameter(h.getT());
                Vector3f dir, col;
                float dist;
                lights[k]->getIllumination(p, dir, col, dist);
                color += k_d * h.getMaterial()->Shade(r, h, dir, col);
            }
            image.SetPixel(i, j, color);
            if(x.depth) {
                float f = clampedDepth(h.getT(), x.dmin, x.dmax);
                depth.SetPixel(i, j, Vector3f(f, f, f));
            }
        } else {
            image.SetPixel(i, j, background);
        }
    }
  }
  image.SaveImage(x.output);
  if(x.depth) depth.SaveImage(x.depth);
  delete parser;
 
  ///TODO: below demonstrates how to use the provided Image class
  ///Should be removed when you start
  return 0;
}

