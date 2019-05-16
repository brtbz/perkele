#version 330

in vec3 position;
in vec2 uv_coord;

out vec2 frag_uv_coord;

void main()
{
	gl_Position = vec4(position.x * 0.5f * 1.0f, position.y * 0.5f * 1.2f, position.z * 0.5f, 1.0f);
	frag_uv_coord = uv_coord;
}
