#version 410

layout(vertices = 3) out;

uniform float TessLevelInner;
uniform float TessLevelOuter;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if ( gl_InvocationID == 0 )
    {
        gl_TessLevelInner[0] = 2;
        gl_TessLevelInner[1] = 2;
        gl_TessLevelOuter[0] = 2;
        gl_TessLevelOuter[1] = 2;
        gl_TessLevelOuter[2] = 2;
        gl_TessLevelOuter[3] = 2;
    }
}