#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include <QGLShaderProgram>
#include "algebra.hpp"

class Material {
public:
  virtual ~Material();
  virtual void apply_gl(QGLShaderProgram* program) const = 0;

protected:
  Material()
  {
  }
private:
  QGLShaderProgram mProgram;
};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
  virtual ~PhongMaterial();

  virtual void apply_gl(QGLShaderProgram* program) const;

private:
  Colour m_kd;
  Colour m_ks;

  double m_shininess;
};


#endif
