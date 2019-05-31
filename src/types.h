typedef union vec2
{
	struct
	{
		float x;
		float y;
	};
	struct
	{
		float u;
		float v;
	};
} vec2;

typedef struct ivec2
{
	int32_t x;
	int32_t y;
} ivec2;

typedef struct ivec4
{
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t w;
} ivec4;

#pragma pack(push, 1)
typedef struct BmpHeader
{
	uint8_t header_field[2];
	uint32_t byte_size;
	uint8_t filler[4]; // 4 bytes of trash
	uint32_t pixel_data_offset;
	uint32_t dib_header_size; // assuming this is 40
	int32_t width;
	int32_t height;
	uint16_t color_planes; // must be 1
	uint16_t bits_per_pixel;
	uint32_t compression_method;
	uint32_t image_size;
	int32_t horizontal_resolution;
	int32_t vertical_resolution;
	uint32_t number_colors;
	uint32_t important_colors;
} BmpHeader;
#pragma pack(pop)

typedef struct IndexedPaletteImage
{
	uint8_t palette[1024];
	int32_t width;
	int32_t height;
	GLuint texture;
	GLuint palette_texture;
} IndexedPaletteImage;

typedef struct DelagardiSettings
{
	int screen_width;
	int screen_height;
	bool fullscreen;
	bool borderless;
	bool vsync;
} DelagardiSettings;

typedef struct Sfx
{
	int id;
	cm_Source *src;
} Sfx;

struct DumbDebugData
{
	int tile_column_min;
	int tile_column_max;
	int tile_row_min;
	int tile_row_max;
};


typedef enum
{
	GOBLIN_BATHROBE_BLUE = 174,
	GOBLIN_BATHROBE_GREEN = 176,
	GOBLIN_BATHROBE_RED = 177,
} ArmyBaseSprite;

typedef struct Army
{
	int32_t position_hex;
	ArmyBaseSprite base_sprite;
	int16_t movement_points_max;
	int16_t movement_points_current;
	int16_t hits_max;
	int16_t hits_current;
	int16_t strength;
	char name[32];
} Army;

typedef enum 
{
	PASSABLE = 1,
	IMPASSABLE = 2,
} TerrainWalkability;

typedef struct MapNode
{
	int32_t index; // this might be superfluous? it's the same as the index in map_nodes array. Think about it!
	int16_t x;
	int16_t y;
	TerrainWalkability terrain;

	int32_t edge[6]; // 0 north, 1 northeast, 2 southeast, 3 south, 4 southwest, 5 northwest
} MapNode;

typedef struct MapNodeNeighbours // these are only used for determining edges at the moment I think
{
	int32_t n_north;
	int32_t n_northeast;
	int32_t n_southeast;
	int32_t n_south;
	int32_t n_southwest;
	int32_t n_northwest;
	int32_t n_count;
} MapNodeNeighbours;

typedef struct MapEdge
{
	int32_t index; // possibly superfluous as well
	int32_t start_node_index;
	int32_t end_node_index;
	int16_t cost;
	int16_t direction; // 0 = north, 1 = northeast, 2 = southeast, 3 = south, 4 = southwest, 5 = northwest
	// TerrainWalkability terrain;
} MapEdge;

typedef struct OpenSetLeaf
{
	int32_t parent = -2; // parent -1 is root
	int32_t left_child = -2; // index in open_set data structure
	int32_t right_child = -2; // index in open_set data structure
	int32_t g_score = -2; // 
	//int32_t h_score = -2; // HexDistance(map_nodes[map_index], map_nodes[goal_node])
	float f_score = -2.0f; //g_score + h_score; // this is the key actually.
	int32_t map_index = -2; // index to map_nodes array
	int32_t came_along_edge = -2;
} OpenSetLeaf;
// needs quick peek, removal and insertion
// insertion moves write pointer forward
// removal invalidates data, but doesn't move remaining data around
// if root is removed, root pointer is shifted to old root's right child
// root won't need removal if it has any remaining left children

typedef struct ClosedSetLeaf
{
	int32_t parent = -2;
	int32_t left_child = -2;
	int32_t right_child = -2;
	int32_t map_index = -2; // key
} ClosedSetLeaf;
// closed set needs quick peeks and quick insertions
// no individual removals from closed set, just clean wipes
