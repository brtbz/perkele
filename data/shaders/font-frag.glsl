#version 330

in vec2 frag_uv_coord;

uniform sampler2D input_texture;

out vec4 color;

void main()
{
    vec4 temp_color = texture(input_texture, frag_uv_coord);
    color = vec4(0.9f, 0.8f, 0.7f, temp_color.r);
}
