#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual void walk_gl(bool picking) const = 0;
  /*
  void setViewer(Viewer* viewer) {
  	mViewer = viewer;
  }*/
  //virtual void draw();
private:
  //Viewer* mViewer;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual void walk_gl(bool picking) const;
  //virtual void draw();
};

#endif
