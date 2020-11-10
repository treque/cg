#version 430 core

// Déclaration des variables passées par le nuanceur de sommets:
in vec4 color;

// Déclaration des variables de sortie
layout(location = 0) out vec4 fragColor;

void main () {

    // À changer:
	fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}