#version 330

// uniform vec3 frag_color;

out vec4 finalColor;
uniform vec4 color;

void main()
{
    finalColor = color; //vec4(1.0, 0.0, 0.0, 1.0);
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
