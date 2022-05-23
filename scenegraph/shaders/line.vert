// Vulkan-compatible GLSL

// For shader code to be written once in Qt applications and libraries, all shaders are expected to
// be written in a single language, which is then compiled into SPIR-V.

// https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.4.60.html
// https://github.com/KhronosGroup/GLSL/blob/master/extensions/khr/GL_KHR_vulkan_glsl.txt

// glslangValidator line.vert

#version 440

// https://www.khronos.org/opengl/wiki/Uniform_(GLSL)
// https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)
// layout(...) @@storage_qualifier=uniform@@ block_name
// {
//   <define members here>
// } instance_name;  // is optional
layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    vec4 color;
    float qt_Opacity;
    float size;
    float spread;
};

// Vertex shader inputs
layout(location = 0) in vec4 position;
layout(location = 1) in float t;

// Vertex shader outputs
layout(location = 0) out float v_t; // vert + t
// https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
// gl_PerVertex defines an interface block for outputs
out gl_PerVertex {
  vec4 gl_Position;
};

void main(void)
{
    vec4 adjusted_position = position;
    // adjusted_position.y += size * t;
    // ??? compensate for max on the vertex position
    adjusted_position.y += size * (t - .5);
    gl_Position = qt_Matrix * adjusted_position;

    v_t = t;
}
