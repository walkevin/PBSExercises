#version 130

in vec3 inPosition;
in vec3 inColor;

uniform mat4 Model_mat = mat4(1.0); 			// Transformation matrix from model 
									// coordinates to world coordinates
uniform mat4 ProjectView_mat;		// View/camera projection matrix

out vec3 theColor;
//out vec3 theColor;

void main()
{
	//gl_Position = vec4(inPosition, 1.0);
	gl_Position = Model_mat * ProjectView_mat * vec4(inPosition, 1.0);
	theColor = inColor;
}
