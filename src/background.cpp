// Indexed Palette Image
void DrawIPI(int palette_phase, bool phase_shift, float zoom_level)
{
	glBindVertexArray(ipi_vao);
	glUseProgram(ipi_sp);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ipi.texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ipi.palette_texture);

	GLint input_texture_loc = glGetUniformLocation( ipi_sp, "input_texture");
	glUniform1i( input_texture_loc, 0);

	GLint palette_texture_loc = glGetUniformLocation( ipi_sp, "palette_texture");
	glUniform1i( palette_texture_loc, 1);

	GLint time_loc = glGetUniformLocation( ipi_sp, "time");
	glUniform1ui( time_loc, master_timer);

	GLint zoom_loc = glGetUniformLocation( ipi_sp, "zoom_level");
	glUniform1f(zoom_loc, zoom_level);

	GLint phase_shift_loc = glGetUniformLocation( ipi_sp, "phase_shift");
	glUniform1i( phase_shift_loc, phase_shift );

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
	glBindVertexArray(0);
}

void LoadBackground()
{
	ipi = LoadIndexedBMP("data/gfx/peace_by_ironthunder_and_sourav_dhar_ccby30_bigger.bmp");

	GLuint ipi_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/indexed-palette-image-vert.glsl");
	GLuint ipi_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/indexed-palette-image-frag.glsl");
	ipi_sp = NewProgram(ipi_vs, ipi_fs);

	GLfloat vertex_data[] = {
		// Positions         // uv_coord
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  
	};
	glGenVertexArrays(1, &ipi_vao);
	glBindVertexArray(ipi_vao);
	glGenBuffers(1, &ipi_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ipi_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	GLint attrib_loc_position = glGetAttribLocation(ipi_sp, "position");
	glEnableVertexAttribArray(attrib_loc_position);
	glVertexAttribPointer(attrib_loc_position, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0 );

	GLint attrib_loc_uv_coord = glGetAttribLocation(ipi_sp, "uv_coord");
	glEnableVertexAttribArray(attrib_loc_uv_coord);
	glVertexAttribPointer(attrib_loc_uv_coord, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
