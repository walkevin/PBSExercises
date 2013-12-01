#version 130
in vec4 position;
//in vec4 normal;
//in vec4 in_Color;

in mat4 modelMat; 			// Transformation matrix from model 
									// coordinates to world coordinates
uniform mat4 ProjectView_mat;		// View/camera projection matrix
out vec4 ex_Color;
                   
void main()
{
	gl_Position =ProjectView_mat * modelMat * position;
	ex_Color = vec4(1.0, 0.0, 0.0, 1.0);
}
