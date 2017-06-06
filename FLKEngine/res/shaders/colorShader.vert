#version 330 core
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoord;

out vec3 Color;
out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Color = color;
	Texcoord = vec2(texcoord.x,1.0 - texcoord.y);
	gl_Position = projection * view * model * vec4(position, 1.0);
}
