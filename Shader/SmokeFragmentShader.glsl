#version 330

uniform sampler2D texture_image;
uniform float intensity;

layout(location = 0) in vec2 texture_coordinates;
layout(location = 1) in float height;

void main(void)
{
  vec3 texture_value = texture(texture_image, texture_coordinates).x;
  float height_falloff = clamp(2.0f * height, 0.0f, 1.0f);
  vec3 alpha = texture_value* height_falloff * (0.2f + intensity*0.8f) ;
  gl_FragColor = vec4(0.0f, 0.0f, 0.0f, alpha);
}