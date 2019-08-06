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

typedef union vec3
{
	struct
	{
		float x;
		float y;
		float z;
	};
	struct
	{
		float r;
		float g;
		float b;
	};
} vec3;

typedef union vec4
{
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};
	struct
	{
		float r;
		float g;
		float b;
		float a;
	};
} vec4;

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

typedef struct PerkeleConfigs
{
	int screen_mode;
	int screen_w;
	int screen_h;
	bool enable_vsync;
	bool enable_music;
	bool enable_sfx;
	float master_gain;
	float music_gain;
	float sfx_gain;
	bool mouse_edge_scroll;
	bool enable_debug_window;
	bool bypass_main_menu;
	bool ignore_move_rules;
	bool pathfind_debug_overlay;
} PerkeleConfigs;

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
	PUDDING_GOLEM = 318,
	BEHOLDER = 366,
	CRUSADER = 401,
	LICH_LORD_BLUE = 497,
	LICH = 511,
	LICH_LORD_GRAY = 528,
	DANCING_SKELETON = 533,
	PIRATE_GIRL = 562,
	FAT_GOBLIN = 567,
	GOBLIN_BATHROBE_PINK = 610,
	RED_KNIGHT = 612,
	SURFER_BOY = 613,
	GOBLIN_GREEN_ARMOR = 621,
	ANGRY_RATMAN = 625,
	GOBLIN_PIRATE = 628,
} ArmyBaseSprite;

typedef enum Faction
{
	FACTION_RED,
	FACTION_BLUE,
	FACTION_GREEN,
	FACTION_GOBLINS,
	FACTION_SURFERS,
	FACTION_COUNT,
} Faction;

typedef struct Army
{
	int32_t index;
	int32_t position_hex;
	ArmyBaseSprite base_sprite;
	int16_t movement;
	int16_t hits_max;
	int16_t hits_current;
	int16_t wounded_soldiers;
	int16_t dead_soldiers;
	int16_t armor;
	int16_t strength;
	int16_t faction;
	int32_t zone_of_control; // maybe this could be bool, but maybe there could be more to zones-of-control than just yes/no
	bool move_done;
	bool action_done;
	bool dead;
	bool draw;
	bool active;
	int attack_sound;
	char name[48];
} Army;

typedef struct ArmyTemplate
{
	ArmyBaseSprite base_sprite;
	int16_t movement;
	int16_t hits_max;
	int16_t armor;
	int16_t strength;

	int attack_sound;
} ArmyTemplate;

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
	int32_t occupier;
	int32_t pathfind_island_id;

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

typedef struct ThickLeaf
{
	int32_t parent = -2; // parent -1 is root
	int32_t left_child = -2; // index in open_set data structure
	int32_t right_child = -2; // index in open_set data structure
	int32_t g_score = -2; // 
	//int32_t h_score = -2; // HexDistance(map_nodes[map_index], map_nodes[goal_node])
	float f_score = -2.0f; //g_score + h_score; // this is the key actually.
	int32_t map_index = -2; // index to map_nodes array
	int32_t came_along_edge = -2;
} ThickLeaf;
// needs quick peek, removal and insertion
// insertion moves write pointer forward
// removal invalidates data, but doesn't move remaining data around
// if root is removed, root pointer is shifted to old root's right child
// root won't need removal if it has any remaining left children

typedef struct SlimLeaf
{
	int32_t parent = -2;
	int32_t left_child = -2;
	int32_t right_child = -2;
	int32_t map_index = -2; // key
} SlimLeaf;
// closed set needs quick peeks and quick insertions
// no individual removals from closed set, just clean wipes

typedef struct Pathfinder
{
	int32_t open_set_write_head = 0;
	int32_t open_set_root_index = 0;
	int32_t open_set_count = 0;

	int32_t open_set_map_indices_write_head = 0;
	int32_t open_set_map_indices_root_index = 0;
	int32_t open_set_map_indices_count = 0;

	int32_t closed_set_write_head = 0;
	int32_t closed_set_root_index = 0;
	int32_t closed_set_count = 0;

	float pathfind_weight_h = 1.5f;
	float pathfind_weight_g = 1.0f;

	ThickLeaf *open_set;
	SlimLeaf *open_set_map_indices;
	SlimLeaf *closed_set;

	// int32_t *came_along_edges;

	int32_t *nodes_that_were_in_open_set_debug;
	int32_t number_of_nodes_that_were_in_open_set_debug;
} Pathfinder;
