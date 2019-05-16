void LoadFontShadersAndInitGLBuffers()
{
    
    GLuint font_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/font-vert.glsl");
    GLuint font_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/font-frag.glsl");
    font_sp = NewProgram(font_vs, font_fs);

    GLfloat vertex_data[] = {
        // Positions         // uv_coord
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  
    };
    glGenVertexArrays(1, &font_vao);
    glBindVertexArray(font_vao);
    glGenBuffers(1, &font_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, font_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    GLint attrib_loc_position = glGetAttribLocation(font_sp, "position");
    glEnableVertexAttribArray(attrib_loc_position);
    glVertexAttribPointer(attrib_loc_position, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0 );

    GLint attrib_loc_uv_coord = glGetAttribLocation(font_sp, "uv_coord");
    glEnableVertexAttribArray(attrib_loc_uv_coord);
    glVertexAttribPointer(attrib_loc_uv_coord, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);



    glGenBuffers(1, &font_custom_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, font_custom_buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat) * LETTER_COUNTER_MAX, NULL, GL_DYNAMIC_DRAW);

    GLint attrib_loc_char_src_rect = glGetAttribLocation(font_sp, "char_src_rect");
    glEnableVertexAttribArray(attrib_loc_char_src_rect);
    glVertexAttribPointer(attrib_loc_char_src_rect, 4, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0 );

    GLint attrib_loc_char_pos = glGetAttribLocation(font_sp, "char_pos");
    glEnableVertexAttribArray(attrib_loc_char_pos);
    glVertexAttribPointer(attrib_loc_char_pos, 2, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(4*sizeof(GLfloat)));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);

}

void LoadFontAndBakeBitmap()
{
	unsigned char ttf_buffer[1<<20];
	//unsigned char temp_bitmap[512*512];

	const char *file_name = "data/fonts/deutsch.ttf";
	FILE *fp = fopen(file_name, "rb");
	ASSERT(fp != NULL);
   	int bytes_read = fread(ttf_buffer, 1, 1<<20, fp);
   	printf("bytes read: %d\n", bytes_read);

   	int bake_rc = stbtt_BakeFontBitmap(ttf_buffer,0, 72.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
   	// can free ttf_buffer at this point
   	ASSERT(bake_rc != 0);

  	glGenTextures(1, &ftex);
   	glBindTexture(GL_TEXTURE_2D, ftex);
   	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512,512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
   	// can free temp_bitmap at this point


   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void InitFont()
{
	LoadFontAndBakeBitmap();
	LoadFontShadersAndInitGLBuffers();
}

void AddStringToBeDrawn(float x, float y, const char* str, int len)
{
	float curX = x;
	float curY = y;

	ASSERT_BARK(letter_counter + len < LETTER_COUNTER_MAX, "too many total chars in strings!! (OVER LETTER_COUNTER_MAX!)");

	for (int i = 0; i < len; i++)
	{
		int ascii = (unsigned char)str[i];
		ascii -= 32; // offset

		strings_buffer_data[(letter_counter+i)*6] = (float)cdata[ascii].x0;
		strings_buffer_data[(letter_counter+i)*6+1] = (float)cdata[ascii].y0;
		strings_buffer_data[(letter_counter+i)*6+2] = (float)cdata[ascii].x1;
		strings_buffer_data[(letter_counter+i)*6+3] = (float)cdata[ascii].y1;
		strings_buffer_data[(letter_counter+i)*6+4] = curX + cdata[ascii].xoff;
		strings_buffer_data[(letter_counter+i)*6+5] = curY + cdata[ascii].yoff;

		curX += ((float)cdata[ascii].xadvance);
	}

	letter_counter += len;
}

void DrawStrings()
{
	glBindBuffer(GL_ARRAY_BUFFER, font_custom_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * letter_counter * 6, (const GLvoid*)&strings_buffer_data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// draw
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(font_sp);
	glBindVertexArray(font_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ftex);

	GLint uniform_loc_input_texture = glGetUniformLocation( font_sp, "input_texture" );
	glUniform1i( uniform_loc_input_texture, 0 );

    GLint uniform_loc_screen_size = glGetUniformLocation( font_sp, "screen_size" );
    glUniform2f(uniform_loc_screen_size, viewport_size.x, viewport_size.y );

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, letter_counter);
	glBindVertexArray(0);
	glUseProgram(0);

	letter_counter = 0;
}
