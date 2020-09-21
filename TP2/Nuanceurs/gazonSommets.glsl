#version 430 core
struct Light
{
        vec3 Ambient; 
        vec3 Diffuse;
        vec3 Specular;
        vec4 Position;  // Si .w = 1.0 -> Direction de lumiere directionelle.
        vec3 SpotDir;
        float SpotExp;
        float SpotCutoff;
        vec3 Attenuation; //Constante, Lineraire, Quadratique
};

layout(location = 0) in vec2 vt;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec3 vp;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 MV_N;
uniform mat4 M;
uniform mat4 V;
uniform Light Lights[3]; // 0:ponctuelle  1:spot  2:directionnelle

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragLight0Vect;
out vec3 fragLight1Vect;
out vec3 fragLight2Vect;

void main (void)
{
    vec3 csPosition3;
    vec3 light0Vect;
    vec3 light1Vect;
    vec3 light2Vect;
    vec3 normal_cameraSpace;
    
    // Transformation de la position
    gl_Position = MVP * vec4(vp,1.0);
    vec4 csPosition = MV * vec4 (vp, 1.0);
    csPosition3 = (csPosition.xyz);
    
    //Vecteurs de la surface vers la lumière
    light0Vect = vec3(Lights[0].Position) - csPosition3;
    light1Vect = vec3(Lights[1].Position) - csPosition3;
    light2Vect = vec3(Lights[2].Position) - csPosition3;

    //Normale en référentiel caméra:
    normal_cameraSpace = normalize(MV_N * vn);
    
    //Coordonée de texture:
    // ...
    fragTexCoord = vt;
    fragNormal = normal_cameraSpace;
    fragLight0Vect = light0Vect;
    fragLight1Vect = light1Vect;
    fragLight2Vect = light2Vect;
    
    // Transformation de la position
    gl_Position = MVP * vec4(vp,1.0);
    
}
