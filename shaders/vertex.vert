#version 460 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor; 
//out vec2 TexCoord; 

uniform mat4 model;
uniform mat4 view; 
uniform mat4 proj;

void main() 
{
    ourColor = aColor; 
    //ourColor = vec3(aPos); // interp betweeen verts
    //TexCoord = aTexCoord;
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    // The below is helpful for a rect if we need to test basics
    // gl_Position = vec4(aPos, 0.0f, 0.0f);
}
