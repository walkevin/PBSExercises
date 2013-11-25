#version 130
in  vec4 in_Position;
uniform mat4 Model_mat; 			// Transformation matrix from model 
									// coordinates to world coordinates
uniform mat4 ProjectView_mat;		// View/camera projection matrix
                   
void main()
{
	// Project vertex coordinates to screen
    //gl_Position = ProjectView_mat * Model_mat * in_Position;
    gl_Position = in_Position;
}