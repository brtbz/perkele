#version 330

in vec2 frag_uv_coord;
out vec4 color;

uniform sampler2D input_texture;

void main()
{
	color = texture(input_texture, frag_uv_coord);
	if ( color.a == 1.0f)
	{
		//color.a = 0.5f;
	}
}