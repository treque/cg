#version 430 core
uniform samplerCube colorMap;

in vec3 fragTexCoord;
out vec4 color;
void main (void) 
{
    color = texture(colorMap, fragTexCoord);
}
