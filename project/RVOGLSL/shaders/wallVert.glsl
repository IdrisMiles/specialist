#version 440 core
/// @brief the vertex passed in
layout (location = 0) in vec3 inVert;
/// @brief the normal passed in
layout (location = 2) in vec3 inNormal;
/// @brief the in uv
layout (location = 1) in vec2 inUV;

uniform mat4 MVP;

void main()
{
// calculate the vertex position
gl_Position = MVP*vec4(inVert,1.0);

}
