#version 430 core
uniform samplerCube colorMap;

in vec3 fragTexCoord;
out vec4 color;
void main (void) 
{
   // compl�ter le nuanceur ici pour texturer le cube
   color = texture(colorMap, fragTexCoord);
}
