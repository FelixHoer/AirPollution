#version 330

uniform mat4 mvp_matrix;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coordinates_in;
layout(location = 2) in vec3 position;

layout(location = 0) out vec2 texture_coordinates;
layout(location = 1) out float height;

void main(void)
{
  vec4 world_position = vec4(position + vertex_position, 1.0f);
	gl_Position = mvp_matrix * world_position;
  texture_coordinates = texture_coordinates_in;
  height = world_position.y;
}