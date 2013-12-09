#version 150
in vec3 normal;
in vec4 position;

out vec3 vs_worldpos;
out vec3 vs_normal;

in mat4 modelMat; 			// Transformation matrix from model 
									// coordinates to world coordinates
uniform mat4 ProjectView_mat;		// View/camera projection matrix
                   
void main()
{
    vec4 position = ProjectView_mat * modelMat * position;
    gl_Position = position;
    vs_worldpos = position.xyz;
    vs_normal = transpose(inverse(mat3(modelMat))) * normal.xyz;
}
