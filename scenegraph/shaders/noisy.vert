#version 440

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    vec4 color;
    vec2 texture_size;
    float qt_Opacity;
};

// Vertex shader inputs
layout(location = 0) in vec4 position; // screen position
layout(location = 1) in vec2 texture_coordinate; // [0, 1]

// Vertex shader outputs
layout(location = 0) out vec2 v_texture_coordinate;
layout(location = 1) out vec2 v_shade_coordinate; // [0, 1]

out gl_PerVertex {
  vec4 gl_Position;
};

void main() {
    gl_Position = qt_Matrix * position;
    // Texel = t in [0,1] * texture_size = position / texture_size * texture_size;
    // v_texture_coordinate = position.xy * texture_size; // ???
    v_texture_coordinate = position.xy / texture_size;
    v_shade_coordinate = texture_coordinate;
}
