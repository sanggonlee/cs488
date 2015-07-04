#ifndef CS488_A4_HPP
#define CS488_A4_HPP

#include <string>
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"

extern bool useMirrorEffect;

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               );

//Colour raytrace(Ray r, SceneNode* node, int depth);
Colour raytrace(Ray r, std::set<Primitive*> objects, const std::list<Light*>& lights, const Colour &ambient, bool &hit, int depth);

#endif
