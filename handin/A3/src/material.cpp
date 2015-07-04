#include "material.hpp"

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl(QGLShaderProgram* program, bool picked) const
{
	program->setUniformValue("ambientColor", 
		QColor(m_kd.R()*255/4, m_kd.G()*255/4, m_kd.B()*255/4));
    program->setUniformValue("diffuseColor", 
    	QColor(m_kd.R()*255, m_kd.G()*255, m_kd.B()*255));
    program->setUniformValue("specularColor", 
		QColor(m_ks.R()*255, m_ks.G()*255, m_ks.B()*255));
		
	if (picked) {
		program->setUniformValue("diffuseColor", 
			QColor(m_kd.R()*255+1, m_kd.G()*255+1, m_kd.B()*255+1));
	}
	
	program->setUniformValue("shininess", (GLfloat)m_shininess);
}


