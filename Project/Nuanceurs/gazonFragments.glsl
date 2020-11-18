#version 430 core

// Déclaration des variables passées par le nuanceur de sommets:
in vec3 colorOut;
in vec3 normal;

// Déclaration des variables de sortie
layout(location = 0) out vec4 fragColor;

void main () {

    // À changer:
	fragColor = vec4(normal, 1.0f);
}