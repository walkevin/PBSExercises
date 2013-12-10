#version 150
//#version 130
// //=======================================================================
// // Uniforms: Constant over all vertices
// //=======================================================================
// uniform vec4 lightPos = vec4(12.0f, 32.0f, 560.0f, 1.0f);	// Position of light in model coordinates
// uniform mat4 modelMat; 			// Transformation matrix from model 
// 					// coordinates to world coordinates
// uniform mat4 ProjectView_mat;		// View/camera projection matrix
// 
// uniform vec4 color_ambient = vec4(0.1, 0.2, 0.5, 1.0);
// uniform vec4 color_diffuse = vec4(0.2, 0.3, 0.6, 1.0);
// uniform vec4 color_specular =  vec4(1.0, 1.0, 1.0, 1.0);
// uniform float shininess = 77.0f;
// 
// //=======================================================================
// // Attributes: Per vertex data
// //=======================================================================
// in vec4 position;			// Vertex coordinates in model space
// in vec3 normal;				// Vertex normal
// 
// 
// //=======================================================================
// // Varying: Vertex shader output, input to rasterizer
// //=======================================================================
// out vec4 color_out;				// shaded color for this vertex
// 
// 
// //=======================================================================
// // Computes the intensity of the reflected diffuse light for a point
// // at position 'position' with normal vector 'normal', if the light
// // source is located at 'lightPos'.
// //=======================================================================
// float computeDiffuseIntens(in vec3 position, in vec3 normal, in vec3 lightPos)
// {
// 	vec3 vecToLight = normalize(lightPos.xyz - position.xyz);
// 	
// 	float diffuseIntensity = dot(normal, vecToLight);
// 	
// 	diffuseIntensity = clamp(diffuseIntensity, 0.0, 1.0);
// 	
// 	return diffuseIntensity;
// }
// 
// float computeSpecularIntens(in vec3 position, in vec3 normal, in vec3 lightPos)
// {
// 	vec3 light_direction = normalize(lightPos - position);
// 	vec3 half_vector = normalize(light_direction + normalize(position));
// 	float specularIntensity = pow(max(0.0, dot(normal, half_vector)), shininess);
// 	return specularIntensity;
// }
// 
// //=======================================================================
// // Vertex program entry point
// //=======================================================================
// void main(void)
// {
// 	// Do some simple diffuse shading
// 	vec3 normal2 = normalize(normal);
// 	float diffuseIntensity = computeDiffuseIntens(position.xyz, normal2, lightPos.xyz);
// 	float specularIntensity = computeSpecularIntens(position.xyz, normal2, lightPos.xyz);
// 	float ambientIntensity = 0.3;
// 
// 	color_out = min(vec4(1.0), color_ambient + diffuseIntensity * color_diffuse + specularIntensity * color_specular);
// 	
// 	// Project vertex coordinates to screen
// 	gl_Position = ProjectView_mat * modelMat * position;
// }


in vec3 normal;
in vec4 position;
in vec4 color_body;

out vec3 vs_worldpos;
out vec3 vs_normal;
out vec4 color_ambient;
in mat4 modelMat; 			// Transformation matrix from model 
									// coordinates to world coordinates
uniform mat4 ProjectView_mat;		// View/camera projection matrix

void main()
{
    color_ambient = color_body;
    vec4 position = ProjectView_mat * modelMat * position;
    gl_Position = position;
    vs_worldpos = position.xyz;
    vs_normal = transpose(inverse(mat3(modelMat))) * normal.xyz;
}
