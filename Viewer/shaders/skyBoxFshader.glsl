#version 330 core

in vec3 fragTexCoords;
uniform samplerCube skybox;
out vec4 frag_color;

void main()
{    
	// 'fragTexCoords' got 3 values because we are rendering a cubemap texture that will behave like a 3 dimensional texture. 
	frag_color = texture(skybox, fragTexCoords);
}