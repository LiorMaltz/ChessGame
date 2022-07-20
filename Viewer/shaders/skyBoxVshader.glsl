#version 330 core

layout (location = 0) in vec3 pos;

out vec3 fragTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform bool orthoElsePerspective;

void main()
{
    // the model is a cube that will be rendered with the skybox texture
    fragTexCoords = pos;

    vec4 posTemp;
    // no model transformation because we want the background texture to be in the same place.
    if(!orthoElsePerspective)
        posTemp = projection * view * vec4(pos, 1.0);
    else   
        posTemp = (view * vec4(pos, 1.0)) * projection;

    // by giving w value instead of z we make sure the skybox will be rendered on the edge of the view-volume
    gl_Position = posTemp.xyww;
    
}  