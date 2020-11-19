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
uniform	mat4 M;
uniform	mat4 V;
uniform	mat4 P;
uniform	mat4 MV;
uniform	mat4 MVP;
uniform	mat3 N;

// Déclaration des variables passées par le nuanceur de sommets:
in vec3 normal;
in vec3 obs;
in vec3 lightDir[2];

// Déclaration des variables de sortie
out vec4 fragColor;

float attenuation = 1.0;

vec4 diffuse(in int i, in vec3 L, in vec3 N, in vec3 O)
{
    vec4 coul = vec4(0.0);
    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calcul de la composante diffuse
        coul += attenuation * Material.Diffuse * vec4(Lights[i].Diffuse, 1) * NdotL;
    }
    return coul;
}

vec4 spec(in int i, in vec3 L, in vec3 N, in vec3 O)
{
    float NdotL = max( 0.0, dot( N, L ) );

    if ( NdotL > 0.0 )
    {
        float specCos = max( 0.0, dot( reflect( -L, N ), O ) );
        return vec4(Lights[i].Specular * pow(specCos, Material.Shininess), 1.0);
    }
    return vec4(0.0);
}

vec4 ambient(in int i)
{
    return Material.Ambient * (Lights[i].Ambient, 1);
}

void main () {
    vec3 N = normalize(normal);
    vec3 O = normalize(obs);

    vec4 coul = vec4(0, 0, 0, 1);

    //for ( int j = 0 ; j < 1 ; ++j )
    //{
        vec3 L = normalize(lightDir[0]);
        coul += ambient(0);
        //coul += diffuse(0, L, N, O);
        //coul += spec(0, L, N, O);
    //}
	fragColor = coul;
}