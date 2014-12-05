#version 330
layout(location = 0) in vec4 color;
void main(void)
{
  gl_FragColor = color;
}