#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoord;

void main()
{
    TexCoord = (aPos.xy+1)/2;
    TexCoord.y = 1 - TexCoord.y;
    gl_Position = vec4(aPos,1.0);
}