#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>
#include <vector>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:

  Group(){

  }
	
  Group( int num_objects ){
    objects = vector<Object3D*>(num_objects);
  }

  ~Group(){
   
  }

  virtual bool intersect( const Ray& r , Hit& h , float tmin ) {
        bool success = 0;
		for(int i = 0; i < objects.size(); i++) {
		    if(objects[i])
		        success |= objects[i]->intersect(r, h, tmin);
		}
		return success;
   }
	
  void addObject( int index , Object3D* obj ){
    // why do we need an index, it is never used and the vector is private
    // oh well
    objects[index] = obj;
  }

  int getGroupSize(){ 
    return objects.size();
  }

 private:
    vector<Object3D*> objects;
};

#endif
	
