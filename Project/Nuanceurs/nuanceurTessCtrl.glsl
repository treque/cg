#version 410

layout(vertices = 3) out;

uniform float TessLevelInner;
uniform float TessLevelOuter;


in vec3 vPosition[];
out vec3 cPosition[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    cPosition[gl_InvocationID] = vPosition[gl_InvocationID];


    if ( gl_InvocationID == 0 )
    {
        gl_TessLevelInner[0] = 2;
        gl_TessLevelOuter[0] = 2;
        gl_TessLevelOuter[1] = 2;
        gl_TessLevelOuter[2] = 2;
    }
}