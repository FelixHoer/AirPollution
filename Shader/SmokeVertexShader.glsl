#version 330
uniform mat4 mvp_matrix;
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 position;
void main(void)
{
  vec4 world_position = vec4(position + vertex_position, 1.0f);
	gl_Position = mvp_matrix * world_position;
}