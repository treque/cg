#version 410

layout(vertices = 3) out;

uniform float TessLevelInner;
uniform float TessLevelOuter;

uniform vec3 eyePos;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;
uniform mat4 P;

in vec3 vPosition[];
out vec3 cPosition[];
in vec3 wordPos[];

float GetTessLevel(float Distance0)
{

    if (Distance0 <= 50.0) {
        return 10.0;
    }
    else if (Distance0 <= 100.0) {
        return 7.0;
    }
    else {
        return 2.0;
    }
}

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    cPosition[gl_InvocationID] = vPosition[gl_InvocationID];

    if ( gl_InvocationID == 0 )
    {
        vec3 eyeWorldPos = eyePos;

        //vec3 wordPos = (MV * vec4(cPosition[gl_InvocationID], 1)).xyz;
        float EyeToVertexDistance0 = distance(eyeWorldPos, wordPos[0] );
        
        //float EyeToVertexDistance0 = distance(eyeWorldPos, cPosition[0] );
        float outterLevel = GetTessLevel(EyeToVertexDistance0);
        //float EyeToVertexDistance1 = distance(eyeWorldPos, cPosition[1]);
        //float EyeToVertexDistance2 = distance(eyeWorldPos, cPosition[2]);

        gl_TessLevelInner[0] = 2;
        gl_TessLevelOuter[0] = outterLevel;
        gl_TessLevelOuter[1] = outterLevel;
        gl_TessLevelOuter[2] = outterLevel;
    }
}