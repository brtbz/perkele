#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h" 

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

/*
#define INI_IMPLEMENTATION
#ifndef _WIN32
#define INI_STRNICMP( s1, s2, cnt ) ( __builtin_strncasecmp( s1, s2, cnt ) ) 
#endif
#include "ini.h"
*/

/*
#define JSON_IMPLEMENTATION
#include "mm_json.h"

#define MATH_3D_IMPLEMENTATION
#include "math_3d.h"
*/
