#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec2 fragTexCoords;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragTexCoords = inTexCoords;
}