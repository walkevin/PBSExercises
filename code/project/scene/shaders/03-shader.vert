#version 130

vec3 inPosition;
vec3 inColor;

smooth out vec3 theColor;

void main()
{
	gl_Position = vec4(inPosition, 1.0);
	theColor = inColor;
}
