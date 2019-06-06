#version 330

in vec3 position;
in vec2 uv_coord;

out vec2 frag_uv_coord;

uniform float zoom_level;

void main()
{
	//float mult = 1.0f;
	float mult = 3.9f / zoom_level;
	mult = pow(mult, 0.5f);
	//gl_Position = vec4(position.x * mult * 1.0f, position.y * mult * 1.2f, position.z * mult, 1.0f);
	gl_Position = vec4(position.x * mult * 1.0f, position.y * mult * 1.0f, position.z * mult, 1.0f);
	frag_uv_coord = uv_coord;
}
