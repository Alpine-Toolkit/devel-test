#version 440

#define PI 3.14159265358979323846

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    vec4 color;
    vec2 texture_size;
    float qt_Opacity;
};

layout(binding = 1) uniform sampler2D qt_Texture;

layout(location = 0) in vec2 v_texture_coordinate;
layout(location = 1) in vec2 v_shade_coordinate;

layout(location = 0) out vec4 frag_color;

void main()
{
  // texture coordinate range in [0,1]
  float noise = texture(qt_Texture, v_texture_coordinate).r;
  vec2 shade_center = vec2(0.5, 0.4);
  float shade = length(shade_center - v_shade_coordinate);
  float color_offset = noise * 0.05 - shade * 0.3;
  vec4 c = vec4(color.xyz + color_offset, color.w);
  frag_color = c * qt_Opacity;
}
