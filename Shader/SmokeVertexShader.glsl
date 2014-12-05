#version 330
uniform mat4 mvp_matrix;
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec3 vertex_normal;
layout(location = 0) out vec4 color;
void main(void)
{
  gl_Position = mvp_matrix * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0f);
  color = vertex_color;
}