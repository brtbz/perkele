GLuint NewShader(GLenum shader_type, const char *file_path);
GLuint NewProgram(GLuint vertex_shader, GLuint fragment_shader);
GLuint LoadTexture(const char *file_path, int *w, int *h);
GLuint CreateEmptyTexture(int w, int h);
IndexedPaletteImage LoadIndexedBMP(const char *file_path);
void LoadInitialData();
