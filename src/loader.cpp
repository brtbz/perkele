GLuint NewShader(GLenum shader_type, const char *file_path)
{
    // GL_VERTEX_SHADER
    // GL_FRAGMENT_SHADER
    GLuint new_shader = glCreateShader(shader_type);

    char *shader_code = NULL;
    FILE *fp = fopen(file_path, "r");

    if (fp != NULL)
    {
        if (fseek(fp, 0L, SEEK_END) == 0)
        {
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }
            shader_code = (char*)malloc(sizeof(char) * (bufsize + 1));
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }
            size_t new_len = fread(shader_code, sizeof(char), bufsize, fp);
            if ( ferror( fp ) != 0 )
            {
                fputs("Error reading file", stderr);
            } else
            {
                shader_code[new_len++] = '\0';
            }
        }
        fclose(fp);
    }

    glShaderSource(new_shader, 1, (const GLchar**)&shader_code, NULL);
    glCompileShader(new_shader);
    int status;
    glGetShaderiv(new_shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char buf[1024];
        glGetShaderInfoLog(new_shader, sizeof(buf), NULL, buf);
        printf("Failed to compile shader:\n%s\n", buf);
    }

    free(shader_code);

    return new_shader;
}

GLuint NewProgram(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint new_program = glCreateProgram();

    glAttachShader(new_program, vertex_shader);
    glAttachShader(new_program, fragment_shader);

    glLinkProgram(new_program);
    int status;
    glGetProgramiv(new_program, GL_LINK_STATUS, &status);
    if (!status)
    {
        char buf[1024];
        glGetProgramInfoLog(new_program, sizeof(buf), NULL, buf);
        printf("Failed to link program:\n%s\n", buf);
    }

    return new_program;
}

GLuint LoadTexture(const char *file_path, int *w, int *h)
{

    GLuint new_texture;
    GLint internal_format = GL_RGBA8; 
    GLenum format = GL_RGBA; // this is supposed to be GL_BGRA in Windows version I guess(?)
    int channels = 4; // so it loads also RGB images as RGBA
    int x,y,n;
    unsigned char *data = stbi_load(file_path, &x, &y, &n, channels);
    printf("%s: %d, %d, %d\n",file_path, x, y, n);

    glGenTextures(1, &new_texture);

    if (w != NULL) { *w = x; }
    if (h != NULL) { *h = y; }

    glBindTexture(GL_TEXTURE_2D, new_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, x, y, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return new_texture;
}

//GLuint LoadIndexedBMP(const char *file_path, int *w, int *h, uint8_t *palette, int palette_size)
IndexedPaletteImage LoadIndexedBMP(const char *file_path)
{
    IndexedPaletteImage img;

    FILE *fp = fopen(file_path, "rb");
    uint8_t *bmp_data;
    uint8_t *pixel_data;
    uint32_t pixel_data_offset;
    uint32_t pixel_data_size;

    int bmp_header_size = 14;
    uint32_t dib_header_size = 40;
    int palette_header_size = 1024;
    int total_header_size = bmp_header_size + dib_header_size + palette_header_size;

    if (fp != NULL)
    {
        fpos_t begin;
        fgetpos(fp, &begin);
        fseek(fp, 0, SEEK_END);
        int file_size = ftell(fp);
        fsetpos(fp, &begin);

        bmp_data = (uint8_t*)malloc(file_size);
        fread(bmp_data, file_size, 1, fp);
        fclose(fp);

        BmpHeader bmp_header = {};
        memcpy(&bmp_header, &bmp_data[0], bmp_header_size + dib_header_size);

        ASSERT(dib_header_size == bmp_header.dib_header_size);
        // *w = bmp_header.width;
        // *h = bmp_header.height;
        img.width = bmp_header.width;
        img.height = bmp_header.height;
        pixel_data_size = bmp_header.byte_size;
        pixel_data_offset = bmp_header.pixel_data_offset;

        // memcpy(palette, &bmp_data[bmp_header_size + dib_header_size], palette_size);
        memcpy(&(img.palette[0]), &bmp_data[bmp_header_size + dib_header_size], 1024);

        pixel_data = (uint8_t*)malloc(pixel_data_size);
        memcpy(pixel_data, &bmp_data[total_header_size], pixel_data_size);

        {
            GLuint new_texture;
            GLint internal_format = GL_R8UI;
            GLenum format = GL_RED_INTEGER;

            glGenTextures(1, &new_texture);
            glBindTexture(GL_TEXTURE_2D, new_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, bmp_header.width, bmp_header.height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
            //glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            img.texture = new_texture;
        }

        {
            GLuint palette_texture;
            GLint internal_format = GL_RGBA8UI;
            GLenum format = GL_RGBA_INTEGER;

            glGenTextures(1, &palette_texture);
            glBindTexture(GL_TEXTURE_2D, palette_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, 256, 1, 0, format, GL_UNSIGNED_BYTE, &bmp_data[bmp_header_size + dib_header_size]);

            glBindTexture(GL_TEXTURE_2D, 0);
            img.palette_texture = palette_texture;
        }

        free(bmp_data);
        free(pixel_data);

        return img;
    }
    else
    {
        fprintf(stderr, "no bmp file!\n");
    }
}

GLuint CreateEmptyTexture(int w, int h)
{
    GLuint new_texture;
    GLint internal_format = GL_RGBA8;
    GLenum format = GL_RGBA;

    glGenTextures(1, &new_texture);

    glBindTexture(GL_TEXTURE_2D, new_texture);
    //glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, w, h);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return new_texture;
}

void LoadInitialData()
{
    if ( LoadMusicAndSounds())
    {
        audio_enabled = true;
    }

    {
        int w, h;
        delagardie_texture = LoadTexture("data/gfx/delagardie.jpg", &w, &h);
    }
    {
        //dqk = LoadIndexedBMP("data/gfx/dqk02b.bmp");
        dqk = LoadIndexedBMP("data/gfx/heretic01.bmp");
    }



#if 1
    {
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
#endif

}
