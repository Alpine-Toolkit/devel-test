#version 440

layout(location = 0) in float v_t;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    vec4 color;
    float qt_Opacity;
    float size;
    float spread;
};

#define PI 3.14159265358979323846

void main(void)
{
  // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/smoothstep.xhtml
  // smoothstep — perform Hermite interpolation between two values
  // smoothstep (edge0, edge1, x)
  // performs smooth Hermite interpolation between 0 and 1 when edge0 < x < edge1.
  // This is useful in cases where a threshold function with a smooth transition is desired.

  // https://realtimemike.wordpress.com/2012/02/07/anti-aliasing-and-edge-lines/

  // t in [0, 1]
  // t =   0  sin(0)   = 0  tt = spread
  // t = 1/2  sin(π/2) = 1  tt = 1
  // t =   1  sin(π)   = 0  tt = spread
  // interpolation in [0,1] when spread < sin(t * PI) < 1
  float tt = smoothstep(spread, 1.0, sin(v_t * PI));
  fragColor = color * qt_Opacity * tt;
}
