#version 330 core
out vec4 FragColor;

uniform sampler2D u_texture;
uniform float u_blend;
uniform vec3 u_color;

in vec2 v_TexCoord;

void main() {
    vec4 texColor = texture(u_texture, v_TexCoord);
    vec4 solidColor = vec4(u_color, 1.0);
    FragColor = mix(solidColor, texColor, u_blend);
}
