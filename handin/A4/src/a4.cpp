#include "a4.hpp"
#include "image.hpp"
#include <math.h>
#include <set>
#include <limits>
#include "mesh.hpp"

#define MAXIMUM_DEPTH 2
#define EPSILON 1e-6


void findIntersectScene(Ray &ray, std::set<Primitive*> objects, Primitive* &obj, double &dist, Vector3D &n, Matrix4x4 &trans) {
	for (std::set<Primitive*>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
		//std::cout << "iterating finding intersect scene"<< std::endl;
		//Primitive * object;
		double d;
		Matrix4x4 tempTrans;
		switch ((*it)->type) {
			case Primitive::NH_SPHERE:
				d = dynamic_cast<NonhierSphere*>(*it)->computeIntersection(ray, tempTrans);
				break;
			case Primitive::NH_BOX:
				d = dynamic_cast<NonhierBox*>(*it)->computeIntersection(ray, tempTrans);
				break;
			case Primitive::MESH:
				d = dynamic_cast<Mesh*>(*it)->computeIntersection(ray, n);
				break;
			default:
				break;
		}
		if (d >= 0 && d < dist) {
			dist = d;
			obj = *it;
			trans = tempTrans;
		}
	}
#if HIER
	ray.origin = trans.invert() * ray.origin;
	ray.direction = trans.invert() * ray.direction;
	ray.direction.normalize();
#endif
}

Colour raytrace(Ray ray, std::set<Primitive*> objects, const std::list<Light*>& lights, const Colour &ambient, bool &hit, int depth) {
/*
	if (depth > 3) {
		std::cerr << "raytrace recursing more than 3 depth! returning" << std::endl;
		return;
	}
*/
	Primitive* object = NULL;
	Vector3D N;
	Matrix4x4 transformMatrix;
	double dist = std::numeric_limits<double>::infinity();	// let -1 represent infinity, since distance can never be negative
	findIntersectScene(ray, objects, object, dist, N, transformMatrix);
	
	if (object == NULL && dist == std::numeric_limits<double>::infinity()) {
		// but will the background colour be black?
		hit = false;
		return Colour(0.0, 0.0, 0.0);
	} else if (object == NULL) {
		std::cerr << 
		"Object is null, but distance is not infinity. Something's wrong here.."<<
		std::endl;
	} else if (dist == std::numeric_limits<double>::infinity()) {
		std::cerr <<
		"Object is not null, but distance is infinity. Something's wrong here.."<<
		std::endl;
	}
	hit = true;
	
	Point3D pointAtTime = ray.origin + (dist * ray.direction);
#if HIER
	pointAtTime = transformMatrix * pointAtTime;
#endif
	Colour ret = ambient;
	
	switch (object->type) {
		case Primitive::NH_SPHERE:
			N = dynamic_cast<NonhierSphere*>(object)->computeNormal(pointAtTime);
			break;
		case Primitive::NH_BOX:
			N = dynamic_cast<NonhierBox*>(object)->computeNormal(pointAtTime);
			break;
		case Primitive::MESH:
			//N = dynamic_cast<Mesh*>(object)->computeNormal(pointAtTime);
			break;
		default:
			break;
	}
	//std::cout << "Normal is "<< N << std::endl;

	PhongMaterial* material = dynamic_cast<PhongMaterial*>(object->getMaterial());

	for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); ++it) {
		Vector3D L = (*it)->position - pointAtTime;
		L.normalize();
		//std::cout << "N is "<<N<<", and L is "<<L<<std::endl;

		// compute shadow value
		double shade = 1.0;
		double tdist = L.length();
		L = (1.0/tdist) * L;
		Ray shadowRay(pointAtTime + EPSILON * L, L);
		double shadowDist = std::numeric_limits<double>::infinity();
			// Dummy variables for filling the function. Can refactor to remove these later..
			Primitive *dummyPrim;
			Vector3D dummyVector;
			Matrix4x4 dummyMat;
		findIntersectScene(shadowRay, objects, dummyPrim, shadowDist, dummyVector, dummyMat);
		if (shadowDist != std::numeric_limits<double>::infinity() && shadowDist >= 0) {
			//std::cout << "shadow intersecting with distance "<< dist<< std::endl;
			shade = 0;
		}

		// add diffuse
		double diffuseRefl = 1.0;
		double dot = N.dot(L);
		//std::cout << "-------------------------------------------------"<<std::endl;
		//std::cout << "dot for diffuse is "<< dot << std::endl;
		if (dot > 0) {
			double diff = shade * dot * diffuseRefl;
			//std::cout << "diffuse is "<< diff << ": "<<shade<<", "<<dot<<std::endl;
			//std::cout << "kd is "<<material->getKd()<<", light "<<(*it)->colour<<std::endl;
			ret = ret + (diff * material->getKd() * (*it)->colour);
			//std::cout << "[]col after diffuse is "<<ret<<std::endl;
		}
		
		Vector3D V = ray.direction;
		Vector3D R = L - 2.0f * L.dot(N) * N;
		
		// add specular
		Colour specularRefl = material->getKs();
		dot = V.dot(R);
		if (dot > 0) {
			Colour spec = shade * powf(dot, material->getShininess()) * specularRefl;
			ret = ret + spec * (*it)->colour;
		}
		//std::cout << "-------------------------------------------------"<<std::endl;
	}
	
	// mirror reflection
	if (useMirrorEffect) {
		double mirrorCoef = 0.7;
		Vector3D R = ray.direction - 2.0 * ray.direction.dot(N) * N;
		if (depth <= MAXIMUM_DEPTH) {
			bool dummyBool;
			Colour rcolour = raytrace(Ray(pointAtTime+EPSILON*R, R), 
				objects, lights, ambient, dummyBool, depth+1);
			//std::cout << "mirror is " << rcolour << std::endl;
			ret = ret + mirrorCoef * rcolour * material->getKd();
		}
	}

	//std::cout << "[-----resulting colour is "<< ret<<"-----]" << std::endl;
	//ret = Colour(1.0-2.3*(1.0-ret.R()),1.0-2.3*(1.0-ret.G()),1.0-2.3*(1.0-ret.B()));
	return ret;
}

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
               )
{
  	// Fill in raytracing code here.
/*
  	std::cerr << "Stub: a4_render(" << root << ",\n     "
            << filename << ", " << width << ", " << height << ",\n     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
            << ambient << ",\n     {";

  	for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
  		if (I != lights.begin()) std::cerr << ", ";
    	std::cerr << **I;
  	}
  	std::cerr << "});" << std::endl;
*/	
  	// Collect objects to render
  	std::set<Primitive*> objects;
  	root->traverseAndCollectPrimitives(objects);

  	// Render
  	Vector3D eyeVector = view - eye;
  	eyeVector.normalize();
  	Vector3D rightVector = eyeVector.cross(up);
  	rightVector.normalize();
  	//Vector3D upVector = rightVector.cross(eyeVector);
  	//upVector.normalize();
  	
  	double fovInRadian = M_PI * (fov/2)/180;
  	double aspect = height/width;
  	double halfWidth = tan(fovInRadian);
  	double halfHeight = aspect * halfWidth;
  	double cameraWidth = halfWidth*2;
  	double cameraHeight = halfHeight*2;
  	double pixelWidth = cameraWidth / (width-1);
  	double pixelHeight = cameraHeight / (height-1);
  	
  	Ray ray = Ray(eye, Vector3D(0,0,0));
  	
  	Image img(width, height, 3);
  	Image background;
  	background.loadPng("background.png");
  	
  	for (int y = 0; y < height; y++) {
 		for (int x = 0; x < width; x++) {
 			Vector3D xComp = ((x*pixelWidth)-halfWidth) * rightVector;
 			Vector3D yComp = ((y*pixelHeight)-halfHeight) * up;
 			ray.direction = eyeVector + xComp + yComp;
 			ray.direction.normalize();
 			
 			bool hit;
 			Colour colour = raytrace(ray, objects, lights, ambient, hit, 0);
 			//hit = true; // for testing purpose
			img(x, height-1-y, 0) = hit ? colour.R() : 
				background(x%background.width(), (height-1-y)%background.height(), 0);
			img(x, height-1-y, 1) = hit ? colour.G() : 
				background(x%background.width(), (height-1-y)%background.height(), 1);
			img(x, height-1-y, 2) = hit ? colour.B() : 
				background(x%background.width(), (height-1-y)%background.height(), 2);
 		}
  	}
/*  
  	// For now, just make a sample image.

  	Image img(width, height, 3);

  	for (int y = 0; y < height; y++) {
    	for (int x = 0; x < height; x++) {
		  	// Red: increasing from top to bottom
		  	img(x, y, 0) = (double)y / height;
		  	// Green: increasing from left to right
		  	img(x, y, 1) = (double)x / width;
		  	// Blue: in lower-left and upper-right corners
		  	img(x, y, 2) = ((y < height/2 && x < height/2)
		                 || (y >= height/2 && x >= height/2)) ? 1.0 : 0.0;
    	}
  	}
*/
  	img.savePng(filename);
}

