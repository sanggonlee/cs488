#version 330 

uniform bool drawingTrackball;
uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightSource;
uniform mat4 jointRotateMatrix;
uniform vec3 origin;

in vec3 vert;
in vec3 normal;

out vec3 varyingNormal;
out vec3 varyingLightDirection;
out vec3 varyingViewerDirection;

void main()
{	
	if (!drawingTrackball) {
		mat4 transOrigMat = mat4(
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	1,	0,
			-origin[0],	-origin[1],	-origin[2],	1
		);
		mat4 InvTransOrigMat = mat4(
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	1,	0,
			vert[0],	vert[1],	vert[2],	1
		);

		vec4 eyeVertex = mvMatrix * inverse(transOrigMat) * jointRotateMatrix * transOrigMat* vec4(vert, 1.0);
		eyeVertex /= eyeVertex.w;
	
		varyingNormal = normalMatrix * normal;
		varyingLightDirection = lightSource - eyeVertex.xyz;
		varyingViewerDirection = -eyeVertex.xyz;


		//gl_Position = mvpMatrix * jointRotateMatrix * vec4(vert, 1.0);
		gl_Position = mvpMatrix * inverse(transOrigMat) * jointRotateMatrix * transOrigMat * vec4(vert, 1.0);
		// gl_Position = ftransform();
    } else {
    	gl_Position = mvpMatrix * vec4(vert, 1.0);
    }
}
