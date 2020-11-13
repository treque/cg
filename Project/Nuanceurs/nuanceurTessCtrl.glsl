#version 410

layout(vertices = 4) out;

uniform float TessLevelInner;
uniform float TessLevelOuter;
uniform bool debugCustomTessellationLevels;

uniform vec3 eyePos;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;
uniform mat4 P;

in vec3 vPosition[];
out vec3 cPosition[];
out vec3 color[];
in vec3 wordPos[];

float GetTessLevel(float Distance0)
{

    if (Distance0 <= 25.0) {
        return 9.0;
    }
    else if (Distance0 <= 50.0) {
        return 8.0;
    }
    else if (Distance0 <= 75.0) {
        return 7.0;
    }
    else if (Distance0 <= 100.0) {
        return 6.0;
    }
    else if (Distance0 <= 125.0) {
        return 5.0;
    }
    else if (Distance0 <= 150.0) {
        return 4.0;
    }
    else if (Distance0 <= 175.0) {
        return 3.0;
    }
    else if (Distance0 <= 200.0) {
        return 2.0;
    }
    else {
        return 1.0;
    }
}

vec3 GetTessLevelColor(float Distance0)
{

    if (Distance0 <= 25.0) {
        return vec3(1.0, 0.0, 0.0);
    }
    else if (Distance0 <= 50.0) {
        return vec3(0.75, 0.25, 0.0);
    }
    else if (Distance0 <= 75.0) {
        return vec3(0.50, 0.50, 0.0);
    }
    else if (Distance0 <= 100.0) {
        return vec3(0.25, 0.75, 0.0);
    }
    else if (Distance0 <= 125.0) {
        return vec3(0.0, 1.0, 0.0);
    }
    else if (Distance0 <= 150.0) {
        return vec3(0.0, 0.75, 0.25);
    }
    else if (Distance0 <= 175.0) {
        return vec3(0.0, 0.50, 0.50);
    }
    else if (Distance0 <= 200.0) {
        return vec3(0.0, 0.5, 0.75);
    }
    else {
        return vec3(0.0, 0.0, 1.0);
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
        color[gl_InvocationID] = GetTessLevelColor(EyeToVertexDistance0);
        //float EyeToVertexDistance1 = distance(eyeWorldPos, cPosition[1]);
        //float EyeToVertexDistance2 = distance(eyeWorldPos, cPosition[2]);

        gl_TessLevelInner[0] =
        gl_TessLevelInner[1] = debugCustomTessellationLevels ? TessLevelInner : outterLevel ;
        gl_TessLevelOuter[0] =
        gl_TessLevelOuter[1] =
        gl_TessLevelOuter[2] =
        gl_TessLevelOuter[3] = debugCustomTessellationLevels ? TessLevelOuter : outterLevel;
    }
}