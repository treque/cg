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

// Déclaration des variables passées par le nuanceur de sommets:
in vec3 colorOut;
vec3 normal;

uniform Light Lights[3];
uniform Mat Material;
uniform int pointLightOn;
uniform int spotLightOn;
uniform int dirLightOn;

vec3 Light0HV;
vec3 Light1HV;
vec3 Light2HV;

vec3 fragLight0Vect;
vec3 fragLight1Vect;
vec3 fragLight2Vect;

uniform	mat4 M;
uniform	mat4 V;
uniform	mat4 P;
uniform	mat4 MV;
uniform	mat4 MVP;
uniform	mat3 N;
uniform float Time;

// Déclaration des variables de sortie
layout(location = 0) out vec4 fragColor;

// Accumulateurs pour les facteurs d'éclairage
vec4 Ambient;
vec4 Diffuse;
vec4 specular;


// Calcule la spécularité d'une source lumineuse
vec4 lightSpec(in int i, in vec3 normal, in vec3 halfVector, in float shininess)
{
    // À compléter
    float NdotH = max(0.0, dot(normal, halfVector));
    return  vec4(Lights[i].Specular * pow(NdotH, shininess), 1.0);
}

void pointLight(in vec3 lightVect)
{
   float nDotVP;       // Produit scalaire entre VP et la normale
   float attenuation;  // facteur d'atténuation calculé
   float d;            // distance entre lumière et fragment
   vec3  VP;           // Vecteur lumière

   // Calculer vecteur lumière
   VP = lightVect;

   // Calculer distance à la lumière
   d = length(VP);

   // Normaliser VP
   VP = normalize(VP);

   // Calculer l'atténuation due à la distance
   attenuation = 1.0 / (Lights[0].Attenuation.x +
       Lights[0].Attenuation.y * d +
       Lights[0].Attenuation.z * d * d);

   nDotVP = max(0.0, dot(normal, VP));

   // Calculer les contributions ambiantes et diffuses
   Ambient  += vec4(Lights[0].Ambient, 1.0) * attenuation;
   Diffuse  += vec4(Lights[0].Diffuse, 1.0) * nDotVP * attenuation;
   specular +=  lightSpec(0, normal, Light0HV, Material.Shininess);
}


// Calcul pour une lumière directionnelle
void directionalLight(in vec3 lightVect)
{
   float nDotVP;         // Produit scalaire entre VP et la normale

   nDotVP = max(0.0, dot(normal, normalize(vec3 (-Lights[2].Position))));

   // Calculer les contributions ambiantes et diffuses
   Ambient  += vec4(Lights[2].Ambient,1.0);
   Diffuse  += vec4(Lights[2].Diffuse,1.0) * nDotVP;
   specular +=  lightSpec(2, normal, Light2HV, Material.Shininess);
}


// Calcul pour une lumière "spot"
void spotLight(in vec3 lightVect)
{
   float nDotVP;            // Produit scalaire entre VP et la normale
   float spotDot;           // Cosinus angle entre VP et direction spot
   float spotAttenuation;   // Facteur d'atténuation du spot
   float attenuation;       // Facteur d'atténuation du à la distance
   float lightToSurfaceAngle;// 
   float d;                 // Distance à la lumière
   vec3  VP;                // Vecteur lumière

   // Calculer le vecteur Lumière
   VP = lightVect;

   // Calculer la distance à al lumière
   d = length(VP);

   // Normaliser VP
   VP = normalize(VP);

   // Calculer l'atténuation due à la distance
   attenuation = 1.0 / (Lights[1].Attenuation.x +
       Lights[1].Attenuation.y * d +
       Lights[1].Attenuation.z * d * d);

   // Le fragment est-il à l'intérieur du cône de lumière ?
   spotDot = dot(-VP, normalize(Lights[1].SpotDir));
   lightToSurfaceAngle = degrees(acos(spotDot));

   if (lightToSurfaceAngle > Lights[1].SpotCutoff)
   {
       spotAttenuation = 0.0; // en dehors... aucune contribution
   }
   else
   {
       spotAttenuation = pow(spotDot, Lights[1].SpotExp);

   }

   // Combine les atténuation du spot et de la distance
   attenuation *= spotAttenuation;

   nDotVP = max(0.0, dot(normal, VP));

   // Calculer les contributions ambiantes et diffuses
   Ambient  += vec4(Lights[1].Ambient, 1.0) * attenuation;
   Diffuse  += vec4(Lights[1].Diffuse, 1.0) * nDotVP * attenuation;
   specular +=  lightSpec(1, normal, Light1HV, Material.Shininess) * attenuation;
}


vec4 flight()
{
    vec4 color;
    vec3 ecPosition3;

    // Réinitialiser les accumulateurs
    Ambient  = vec4 (0.0);
    Diffuse  = vec4 (0.0);
    specular = vec4(0.0);

    if (pointLightOn == 1) {
        pointLight(fragLight0Vect);
    }

    if (dirLightOn == 1) {
        directionalLight(fragLight2Vect);
    }
    
    if (spotLightOn == 1) {
        spotLight(fragLight1Vect);
    }

    //color = (Ambient * Material.Ambient + Diffuse  * Material.Diffuse);
    color = (Ambient * Material.Ambient + Diffuse  * Material.Diffuse + specular * Material.Specular);
    color = clamp( color, 0.0, 1.0 );
    return color;
}

vec3 random3(vec3 c) {
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}
//
///* skew constants for 3d simplex functions */
const float F3 =  0.3333333;
const float G3 =  0.1666667;
//
///* 3d simplex noise */
float simplex3d(vec3 p) {
	 /* 1. find current tetrahedron T and it's four vertices */
	 /* s, s+i1, s+i2, s+1.0 - absolute skewed (integer) coordinates of T vertices */
	 /* x, x1, x2, x3 - unskewed coordinates of p relative to each of T vertices*/
	 
	 /* calculate s and x */
	 vec3 s = floor(p + dot(p, vec3(F3)));
	 vec3 x = p - s + dot(s, vec3(G3));
	 
	 /* calculate i1 and i2 */
	 vec3 e = step(vec3(0.0), x - x.yzx);
	 vec3 i1 = e*(1.0 - e.zxy);
	 vec3 i2 = 1.0 - e.zxy*(1.0 - e);
	 	
	 /* x1, x2, x3 */
	 vec3 x1 = x - i1 + G3;
	 vec3 x2 = x - i2 + 2.0*G3;
	 vec3 x3 = x - 1.0 + 3.0*G3;
	 
	 /* 2. find four surflets and store them in d */
	 vec4 w, d;
	 
	 /* calculate surflet weights */
	 w.x = dot(x, x);
	 w.y = dot(x1, x1);
	 w.z = dot(x2, x2);
	 w.w = dot(x3, x3);
	 
	 /* w fades from 0.6 at the center of the surflet to 0.0 at the margin */
	 w = max(0.6 - w, 0.0);
	 
	 /* calculate surflet components */
	 d.x = dot(random3(s), x);
	 d.y = dot(random3(s + i1), x1);
	 d.z = dot(random3(s + i2), x2);
	 d.w = dot(random3(s + 1.0), x3);
	 
	 /* multiply d by w^4 */
	 w *= w;
	 w *= w;
	 d *= w;
	 
	 /* 3. return the sum of the four surflets */
	 return dot(d, vec4(52.0));
}
//
///* const matrices for 3d rotation */
const mat3 rot1 = mat3(-0.37, 0.36, 0.85,-0.14,-0.93, 0.34,0.92, 0.01,0.4);
const mat3 rot2 = mat3(-0.55,-0.39, 0.74, 0.33,-0.91,-0.24,0.77, 0.12,0.63);
const mat3 rot3 = mat3(-0.71, 0.52,-0.47,-0.08,-0.72,-0.68,-0.7,-0.45,0.56);
//
///* directional artifacts can be reduced by rotating each octave */
float simplex3d_fractal(vec3 m) {
    return   0.5333333*simplex3d(m*rot1)
			+0.2666667*simplex3d(2.0*m*rot2)
			+0.1333333*simplex3d(4.0*m*rot3)
			+0.0666667*simplex3d(8.0*m);
}

in vec4 pos;

vec3 getNormal(vec3 ws_p1, vec3 ws_p2, vec3 ws_p3)
{
    vec4 edge1 = normalize((vec4(ws_p1, 2) - vec4(ws_p2, 1)));
    vec4 edge2 = normalize((vec4(ws_p3, 3) - vec4(ws_p2, 2)));
    vec3 e1 = vec3(edge1.x, edge1.y, edge1.z);
    vec3 e2 =  vec3(edge2.x, edge2.y, edge2.z);

    return normalize(cross(e1, e2));
}

vec4 height( vec4 pos )
{
	vec2 p = (M * pos).xz / 500.f;
    vec3 p4 = vec3(p, Time * 0.003);

    float noiseVal;
    noiseVal = simplex3d_fractal(p4 * 20 + 20);
    noiseVal = 0.5 + 0.5 * noiseVal;
	return M * (pos + vec4(0 , noiseVal * 10 , 0 , 0));
}

void main () {

	vec4 posInterpol = height(pos);
	// The 0.1 step need to be reworked, we need to calculate it from the levels
	vec4 posInterpolXP = height(pos + vec4(0.1, 0, 0, 0));
	vec4 posInterpolZP = height(pos + vec4(0, 0, 0.1, 0));

	normal = mat3(V) * getNormal(posInterpolZP.xyz, posInterpol.xyz, posInterpolXP.xyz).xyz;

	vec4 ecPosition = V * posInterpol;
	vec3 ecPosition3;
	ecPosition3 = (vec3 (ecPosition)) / ecPosition.w;
	fragLight0Vect = vec3 (Lights[0].Position) - ecPosition3;
    fragLight1Vect = vec3 (Lights[1].Position) - ecPosition3;
    fragLight2Vect = vec3 (-Lights[2].Position);

	Light0HV = normalize(fragLight0Vect - ecPosition.xyz);
	Light1HV = normalize(fragLight1Vect - ecPosition.xyz);
	Light2HV = normalize(-fragLight2Vect);

    // À changer:
	//fragColor = vec4(normal, 1.0f);
	//fragColor = flight(normal);
    //Ambient  = vec4 (0.0);
    //Diffuse  = vec4 (0.0);
    //specular = vec4(0.0);

    //pointLight(fragLight0Vect);
    //spotLight(fragLight1Vect);
    //directionalLight(fragLight2Vect);
	//fragColor = (Ambient * 1.0f + Diffuse  * 1.0f + specular * 1.0f);
	fragColor = flight();
}