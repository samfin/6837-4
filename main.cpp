#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include "RayTracer.h"
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
    int bounces;
    bool shadows;
};

int parse_int(char *x) {
    int k = 0, i = 0;
    while(x[i]) {
        k = 10 * k + (x[i++] - '0');
    }
    return k;
}

void get_args(int argc, char *argv[], args &args) {
    // Set default args
    args.depth = 0;
    args.bounces = 4;
    args.shadows = 0;
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
        } else if(strcmp(argv[i],"-bounces")==0) {
            args.bounces = parse_int(argv[i+1]);
            i += 2;
        } else if(strcmp(argv[i],"-shadows")==0) {
            args.shadows = 1;
            i += 1;
        } else {
            cout << "Could not recognize argument " << argv[i] << "\n";
            i++;
        }
    }
}

float random_float() {
    return 0.5 - (float) rand() / RAND_MAX;
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

  SceneParser *scene = new SceneParser(x.input);
  RayTracer *tracer = new RayTracer(scene, x.bounces, x.shadows);
  Camera *camera = scene->getCamera();
  Image image(x.w, x.h);
  Image depth(x.w, x.h);
  /*
  for(int i = 0; i < x.w; i++) {
    for(int j = 0; j < x.h; j++) {
        float x0 = i * 2.0 / (x.w - 1) - 1;
        float x1 = j * 2.0 / (x.h - 1) - 1;
        Ray r = camera->generateRay(Vector2f(x0, x1));
        Hit h;
        Vector3f color = tracer->traceRay(r, h);
        image.SetPixel(i, j, color);
    }
  }
  */
  x.w *= 3;
  x.h *= 3;
  Vector3f ***big_image = new Vector3f**[x.w];
  for(int i = 0; i < x.w; i++) {
    big_image[i] = new Vector3f*[x.h];
    for(int j = 0; j < x.h; j++) {
      big_image[i][j] = new Vector3f[2];
        float x0 = (i + random_float()) * 2.0 / (x.w - 1) - 1;
        float x1 = (j + random_float()) * 2.0 / (x.h - 1) - 1;
        Ray r = camera->generateRay(Vector2f(x0, x1));
        Hit h;
        Vector3f color = tracer->traceRay(r, h);
        big_image[i][j][0] = color;
    }
  }
  float kernel[5] = {0.1201, 0.2339, 0.2931, 0.2339, 0.1201};
  // Horizontal
  for(int i = 0; i < x.w; i++) {
    for(int j = 0; j < x.h; j++) {
      big_image[i][j][1] = Vector3f(0, 0, 0);
      for(int k = 0; k < 4; k++) {
        int j2 = j + k - 2;
        if(j2 < 0 || j2 >= x.h) continue;
        big_image[i][j][1] += kernel[k] * big_image[i][j2][0];
      }
    }
  }
  // Vertical
  for(int j = 0; j < x.h; j++) {
    for(int i = 0; i < x.w; i++) {
      big_image[i][j][0] = Vector3f(0, 0, 0);
      for(int k = 0; k < 4; k++) {
        int i2 = i + k - 2;
        if(i2 < 0 || i2 >= x.w) continue;
        big_image[i][j][0] += kernel[k] * big_image[i2][j][1];
      }
    }
  }
  // Down sample
  x.w /= 3;
  x.h /= 3;
  for(int i = 0; i < x.w; i++) {
    for(int j = 0; j < x.h; j++) {
      Vector3f color = Vector3f(0, 0, 0);
      for(int k = 0; k < 3; k++) {
        for(int k2 = 0; k2 < 3; k2++) {
          color += big_image[3*i + k][3*j + k2][0];
        }
      }
      color *= 1.0 / 9.0;
      image.SetPixel(i, j, color);
    }
  }
  image.SaveImage(x.output);
  delete tracer;
  delete scene;
  return 0;
}

