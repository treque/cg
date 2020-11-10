#version 430 core

// Déclarations des variables passées par tampons (buffers):
layout(location = 0) in vec3 vp;

// Déclaration des variables uniformes :
// ...
uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 MV_N;
uniform mat4 M;

// Déclaration des variables de sorties:
// ...
out vec4 color;

void main () {
    //// Eye-coordinate position of vertex, needed in various calculations
    //vec3 ecPosition3;
    //vec4 ecPosition = MV * vec4(vp,1.0);
    //
    //// Do fixed functionality vertex transform
    //ecPosition3 = (vec3 (ecPosition)) / ecPosition.w;
    //fragNormal = fnormal();
    //ftexgen(); 
    gl_Position = (MVP * vec4(vp,1.0));
}