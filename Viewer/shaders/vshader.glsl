#version 330 core


layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool useEnvironmentMap;

// transfer to color shader
out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexCoords;

// trensfer to frag to help calculate canonical textures
out vec3 localVetexPos;




void main()
{
	// This gave us some projection issues, so we tried transposing it
	gl_Position = projection * (vec4(pos, 1.0f) * model * view);
	//gl_Position = projection * transpose(view) * transpose(model) * vec4(pos, 1.0f);
	fragPos = vec3(vec4(pos, 1.0f) * model);


	fragTexCoords = texCoords;
	localVetexPos = pos;
	fragNormal = normal;
	
}