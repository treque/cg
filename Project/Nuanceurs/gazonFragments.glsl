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

struct Mat
{
        vec4 Ambient; 
        vec4 Diffuse;
        vec4 Specular;
        vec4 Exponent;
        float Shininess;
};

uniform Light Lights[2]; // 0:ponctuelle  1:dir
uniform Mat Material;

// Déclaration des variables passées par le nuanceur de sommets:
in vec3 colorOut;
in vec3 normal;
in vec3 obs;
in vec3 lightDir[2];

// Déclaration des variables de sortie
out vec4 fragColor;

float attenuation = 1.0;

vec4 calculerReflexion( in int j, in vec3 L, in vec3 N, in vec3 O ) // pour la lumière j
{
    vec4 coul = vec4(0.0);

    // calcul de la composante ambiante pour la source de lumière
    coul += Material.Ambient * vec4(Lights[j].Ambient, 1);

    // calcul de l'éclairage seulement si le produit scalaire est positif
    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calcul de la composante diffuse
        coul += attenuation * Material.Diffuse * vec4(Lights[j].Diffuse, 1) * NdotL;

        float spec = max( 0.0, dot( reflect( -L, N ), O ) );
        coul += attenuation * Material.Specular * vec4(Lights[j].Specular, 1) * ( ( spec == 0.0 ) ? 0.0 : pow( spec, Material.Shininess ) );
    }

    return( coul );
}

void main () {
    vec3 N = normalize(normal);
    vec3 O = normalize(obs);

    vec4 coul = vec4(0, 0, 0, 0.5);

    for ( int j = 0 ; j < 2 ; ++j )
    {
        vec3 L = normalize(lightDir[j]);
        coul += calculerReflexion(j, L, N, O);
    }
	fragColor = coul;
}