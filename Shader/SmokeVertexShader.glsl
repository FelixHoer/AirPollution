#version 330

uniform mat4 mvp_matrix;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coordinates_in;
layout(location = 2) in vec3 position;

layout(location = 0) out vec2 texture_coordinates;
layout(location = 1) out float height;

void main(void)
{
  vec3 zt = normalize(cross(position, vec3(0,1,0)));
  vec3 yt = normalize(position);
  vec3 xt = normalize(cross(yt, zt));
  
  vec3 offset = vertex_position.x * xt + 
                vertex_position.y * yt + 
                vertex_position.z * zt;
  vec4 world_position = vec4(position + offset, 1.0f);
	gl_Position = mvp_matrix * world_position;
  
  texture_coordinates = texture_coordinates_in;
  
  height = position.y;
}