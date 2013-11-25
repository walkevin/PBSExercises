#version 130
in  vec4 in_Position;
in  vec4 in_Color;

uniform mat4 Model_mat; 			// Transformation matrix from model 
									// coordinates to world coordinates
uniform mat4 ProjectView_mat;		// View/camera projection matrix
out vec4 ex_Color;
                   
void main()
{
	gl_Position = Model_mat * ProjectView_mat * in_Position;
	ex_Color = in_Color;
}
