#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 Color;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 posv4 = vec4(position, 1.0f);
    Color = normal;
    FragPos = position;
    Normal = normal;
    TexCoord = texCoord;
    gl_Position = projection*view*posv4;
}
