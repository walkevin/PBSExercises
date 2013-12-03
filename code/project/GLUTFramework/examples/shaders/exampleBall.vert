#version 130

uniform mat4 model_matrix = mat4(1.0);
uniform mat4 ProjectView_mat;

in vec3 normal;
in vec4 position;

out vec3 vs_worldpos;
out vec3 vs_normal;

void main(void)
{
    vec4 position = ProjectView_mat * model_matrix * position;
    gl_Position = position;
    vs_worldpos = position.xyz;
    vs_normal = mat3(model_matrix) * normal.xyz;
}


/*
#version 130
in  vec4 in_Position;
in  vec4 in_Color;
out vec4 ex_Color;
uniform mat4 ProjectView_mat;		// View/camera projection matrix
void main()
{
	gl_Position = ProjectView_mat * in_Position;
	ex_Color = vec4(1.0f,0.0f,0.0f,1.0f);
}
*/
