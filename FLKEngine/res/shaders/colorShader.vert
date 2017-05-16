#version 330 core
 
in vec3 position;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec2 Texcoord;

uniform mat4 transform;

void main()
{
	Color = color;
	Texcoord = vec2(texcoord.x,1.0 - texcoord.y);
	gl_Position = transform * vec4(position, 1.0);
}
