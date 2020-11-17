#version 430 core

// Déclaration des variables passées par le nuanceur de sommets:
in vec3 colorOut;

// Déclaration des variables de sortie
out vec4 fragColor;

void main () {

    // À changer:
	fragColor = vec4(colorOut, 1.0f);
}