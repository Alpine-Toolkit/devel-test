#version 440

#define PI 3.14159265358979323846

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    vec4 color;
    float qt_Opacity;
    float size;
    float spread;
};

layout(location = 0) in float v_t;

layout(location = 0) out vec4 frag_color;

void main(void)
{
  // ??? Pen width is not constant ???
  // ??? measure the offset

  // See also https://realtimemike.wordpress.com/2012/02/07/anti-aliasing-and-edge-lines

  // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/smoothstep.xhtml
  // smoothstep — perform Hermite interpolation between two values
  // smoothstep (edge0, edge1, x)
  // performs smooth Hermite interpolation between 0 and 1 when edge0 < x < edge1.
  //   ρ = clamp((x - edge0) / (edge1 - edge0), 0, 1)
  //   return ρ * ρ * (3 - 2 * ρ)
  // This is useful in cases where a threshold function with a smooth transition is desired.

  // t in [0, 1]
  // interpolation in [0,1] when spread < sin(t * PI) < 1
  //   ρ = clamp((x - spread) / (1 - spread), 0, 1)
  //   return ρ * ρ * (3 - 2 * ρ)
  // t =   0   x = sin(0)   = 0   ρ = 0   tt = 0
  // t = 1/2   x = sin(π/2) = 1   ρ = 1   tt = 1
  // t =   1   x = sin(π)   = 0   ρ = 0   tt = 0

  // Without sin() : it shows steps on the lower edge
  // float tt = smoothstep(spread, 1.0, v_t);
  float tt = smoothstep(spread, 1.0, sin(v_t * PI));
  frag_color = color * qt_Opacity * tt;
}
