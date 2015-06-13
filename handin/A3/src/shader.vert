#version 330 

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightSource;

in vec3 vert;
in vec3 normal;

out vec3 varyingNormal;
out vec3 varyingLightDirection;
out vec3 varyingViewerDirection;

void main()
{	
	vec4 eyeVertex = mvMatrix * vec4(vert, 1.0);
	eyeVertex /= eyeVertex.w;
	
	varyingNormal = normalMatrix * normal;
	varyingLightDirection = lightSource - eyeVertex.xyz;
	varyingViewerDirection = -eyeVertex.xyz;

    gl_Position = mvpMatrix * vec4(vert, 1.0);
    // gl_Position = ftransform();
}
