// TIMING
uint64_t program_start_time_ticks;
uint64_t perf_freq = 1;
uint32_t master_timer = 0; //ms


bool game_running = true;
bool show_debug_ui = true;
bool show_settings_window = false;
bool show_main_menu = false;
bool show_memory_window = false;


bool audio_enabled = false;


DelagardiSettings settings_confirmed = { 1280, 720, false, false, true };
DelagardiSettings settings_temp = settings_confirmed;
const char* settings_file_name = "delagardi.ini";
char settings_msg[32] = "moi";



SDL_GLContext gl_context;
SDL_Window *window;
const char* glsl_version = "#version 330";
vec2 viewport_size = {640.0f, 480.0f};



// AUDIO
SDL_mutex* audio_mutex;
cm_Source *music_src;
SDL_AudioDeviceID audio_dev;
Sfx sfx[37] = {};




GLuint delagardie_texture;



// TEXT
//unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[512*512];
stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
GLuint ftex;

GLuint font_sp;
GLuint font_vbo;
GLuint font_vao;
GLuint font_custom_buffer;

#define LETTER_COUNTER_MAX 4096
float strings_buffer_data[6*LETTER_COUNTER_MAX];
uint32_t letter_counter = 0;


float debug_ville_len = 0.0f;
float debug_kalle_len = 0.0f;




// ARMY
GLuint army_sp;
GLuint army_vbo;
GLuint army_vao;
GLuint army_texture;
GLuint unit_data_buffer;
int32_t unit_limit = 1024;
int unit_data_count = 183;
ivec4* unit_data;
bool unit_data_buffer_needs_update = true;


Army test_armies[183];

Army *selected_army = NULL;

// HEX MAP
GLuint hex_sp;
GLuint hex_vbo;
GLuint hex_vao;
GLuint hex_map_texture;
GLuint hex_map_highlight_texture;
GLuint hex_indices_buffer;
GLuint hex_terrain_types_buffer;

// miksei nää oo uint8?
uint32_t map_width = 128;
uint32_t map_height = map_width;
uint32_t map_size = map_width * map_height;
uint32_t *map_data;
int32_t highlighted_hex = -2112;

uint32_t hexes_to_draw_count = 0;
uint32_t *hexes_to_draw_indices = NULL;
uint32_t *hexes_to_draw_terrain_types = NULL;
uint32_t hexes_to_draw_max = map_size;

MapNode *map_nodes;
MapNodeNeighbours *map_node_neighbours;
MapEdge *map_edges;
uint32_t total_edge_count = 0;

GLuint edge_sp;
GLuint edge_vbo;
GLuint edge_vao;
GLuint edge_texture;



#define OPEN_SET_MAX_SIZE 16384
#define CLOSED_SET_MAX_SIZE OPEN_SET_MAX_SIZE
OpenSetLeaf open_set[OPEN_SET_MAX_SIZE];
int32_t open_set_write_head = 0;
int32_t open_set_root_index = 0;
int32_t open_set_count = 0;
ClosedSetLeaf closed_set[CLOSED_SET_MAX_SIZE];
int32_t closed_set_write_head = 0;
int32_t closed_set_root_index = 0;
int32_t closed_set_count = 0;
int32_t *came_along_edges; // this a big ARRAY! array index is same as map index, value is edge's index
int32_t path_edges[512];
uint32_t path_edges_size = 0;
float pathfind_weight_h = 1.5f;
float pathfind_weight_g = 1.0f;
bool draw_path = false;
ivec2 current_path = { 0, 0 };
uint32_t nodes_that_were_in_open_set_debug[OPEN_SET_MAX_SIZE];
int32_t number_of_nodes_that_were_in_open_set_debug;
GLuint hex_debug_overlay_sp;
GLuint hex_debug_overlay_vao;
GLuint hex_debug_overlay_vbo;
GLuint hex_indices_debug_overlay_buffer;

Camera camera;

DumbDebugData dumb_dbg;
char dumb_debug_string[256];
char dumb_debug_string2[256];

//vec2 mouse_pos_screen;
ivec2 mouse_pos_screen = {1, 1};
vec2 mouse_pos_map;

bool mouse_first_move_done = false;

IndexedPaletteImage dqk;
GLuint ipi_sp;
GLuint ipi_vbo;
GLuint ipi_vao;


ImFont *im_font_roboto;
ImFont *im_font_monofonto;
