#version 330 core
out vec4 FragColor;
uniform sampler2D u_texture;
uniform float u_blend;
uniform vec3 u_color;
in vec2 v_TexCoord;
in vec3 v_ViewPos;

void main() {
    vec3 faceNormal = normalize(cross(dFdx(v_ViewPos), dFdy(v_ViewPos)));
    vec3 lightDir = normalize(vec3(0.4, 0.6, 1.0));
    float diffuse = max(dot(faceNormal, lightDir), 0.0);
    float shade = 0.15 + 0.85 * diffuse;

    vec4 texColor = texture(u_texture, v_TexCoord);
    vec4 solidColor = vec4(u_color * shade, 1.0);
    vec4 shadedTex = vec4(texColor.rgb * shade, texColor.a);
    FragColor = mix(solidColor, shadedTex, u_blend);
}
