#version 460 core
out vec4 FragColor;

in vec3 ourColor; 
//in vec2 TexCoord; 

uniform sampler2D ourTexture;

void main() 
{
    // vec3 ourColor = vec3(0.0f, 1.0f, 1.0f);
    FragColor = vec4(ourColor, 1.0f);
}
