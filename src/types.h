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
	LION_DRAGON = 1,
	UNICORN_RIDERS = 2,
	BALLOON_RIDERS = 3,
	STEAMPHONT = 4,
	ELEPHANTMEN = 5,
	PEASANTS = 6,
	BLACK_DRAGONS = 7,
	ELF_BOWMEN = 8,
	HEAVY_CAVALRY = 9,
	WYVERNS = 10,
	SKELETON_CAVALRY = 11,
	SILVER_DRAGONS = 12,
	SEEKERS = 13,
	FIRE_WIZARD = 14,
	HUNTSMEN = 15,
	SOME_LIGHT_CAV = 16,
	MORTAR = 17,
	PEGASUS_RIDERS = 18,
	BRONZE_DRAGON = 19,
	GREEN_DRAGONS = 20,
	ELF_CHARIOTS = 21,
	CENTAURS = 22,
	TURTLE_DRAGON = 23,
	CENTAUR_KNIGHTS = 24,
	HEALERS = 25,
	MILITIA = 26,
	AUXILIA = 27,
	ASHIGUERS = 28,
	BERSERKERS = 29,
	APPRENTICES = 30,
	CONJURERS = 31,
	WARBAND = 32,
	SWORDSMEN = 33,
	LEGIONARII = 34,
	PIKEMEN = 35,
	LIZARDMEN = 36,
	SAMURAI = 37,
	LIONMEN = 38,
	WEREBEARS = 39,
	MONKS_OF_MEO = 40,
	GOBLINS = 41,
	ORCS = 42,
	GOBLIN_ARCHERS = 43,
	SERPENT_RIDERS = 44,
	WOLF_RIDERS = 45,
	DISEASE_THOWERS = 46,
	GOBLIN_SKIRMISHERS = 47,
	HARPIES = 48,
	SOME_BEAST_BOMBER = 49,
	SOME_EVIL_BOMBER = 50,
	AIR_WARRIORS = 51,
	SQUIRES = 52,
	LIGHT_CAVALRY = 53,
	ELVEN_KNIGHTS = 54,
	SANDMEN = 55,
	HYDRA = 56,
	HEAVY_SPEARSMEN = 57,
	CHARIOTS = 58,
	GIANT = 59,
	TREEMEN = 60,
	PISTOLIERS = 61,
	MUSKETEERS = 62,
	EARTHMAGE = 63,
	GARGOYLES = 64,
	DRUIDS = 65,
	GRIPPER = 66,
	SOME_DARK_EVIL_SKYHUNTER = 67,
	FLAME_KNIGHTS = 68,
	BARBARIANS = 69,
	NIGHTMARE = 70,
	SOME_MECH_CAV = 71,
	ANOTHER_MECH_CAV = 72,
	FIRE_BEETLES = 73,
	OGRE_GRIFFONS = 74,
	BOAR_RIDERS = 75,
	EAGLE_RIDERS = 76,
	EAGLE_RIDER_LOOKING_SOMETHING = 77,
	DRAGON_RIDERS_OR_SOMETHING = 78,
	OGRE_CENTAURS = 79,
	OGRE_HUNTERS = 80,
	JAVELINMEN = 81,
	SKELETONS = 82,
	RATMEN = 83,
	TROLLS = 84,
	IRON_GUARD = 85,
	LAVAMEN = 86,
	OGRES = 87,
	MUMMY = 88,
	COBRAMEN = 89,
	OGRE_KNIGHTS = 90,
	BRONZEMAN = 91,
	EAGLES = 92,
	PEGASI = 93,
	ORC_LONGBOWS = 94,
	BALLOON_HANGERS = 95,
	SLAVE_ARCHERS = 96,
	FLYING_SNAKES = 97,
	CATAPULTS = 98,
	BALLISTAS = 99,
	SPATTER_GUNS = 100,
	CHAIN_GUNS = 101,
	EARTH_ELEMENTAL = 102,
	WARLORD_MECHANUS = 103,
	CORPORAL = 104,
	SERGEANT = 105,
	LIEUTENANT = 106,
	CAPTAIN = 107,
	GENERAL = 108,
	BORRIC = 109,
	LORIC = 110,
	DREGGO = 111,
	SHADOW_WARRIORS = 112,
	SHANDEL = 113,
	XYPHON = 114,
	HANDOLA = 115,
	SLINGERS = 116,
	STAFF_SLINGERS = 117,
	SCOUTS = 118,
	FORESTERS = 119,
	TRAIL_RUNNERS = 120,
	SKELETON_SLINGERS = 121,
	ORC_BOWMEN = 122,
	SHADOWS = 123,
	LONGBOWMEN = 124,
	FIRE_ARCHERS = 125,
	ROYAL_ARCHERS = 126,
	KRINDEL = 127,
	ORC_SKIRMISHERS = 128,
	SKELETON_ARCHERS = 129,
	STEAM_GUNNERS = 130,
	CATAPHRACTOI = 131,
	POWER_LANCERS = 132,
	PHOENIX_KNIGHTS = 133,
	GHOST_RIDERS = 134,
	ROLLING_MAN = 135,
	SIR_KALADOR = 136,
	MASTODON = 137,
	BOAR_KNIGHTS = 138,
	SHADOW_KNIGHTS = 139,
	JUNS = 140,
	LIGHT_CAV_X = 141,
	LIGHT_CAV_Y = 142,
	BARBARIAN_CAVALRY = 143,
	BEAR_CAVALRY = 144,
	LIGHT_CAV_Z = 145,
	WIND_RIDERS = 146,
	DRAGON_KNIGHTS = 147,
	SILVER_HAWKS = 148,
	PHOENIX_HAWKS = 149,
	SCREAMER = 150,
	STEAM_HAWKS = 151,
	SOME_MECH_GUN = 152,
	VALIRA = 153,
	THAT_GOOD_DRAGON_HERO = 154,
	FLYING_MONKEYS = 155,
	GOATLORD = 156,
	LAVA_DRAGONS = 157,
	SHADOW_DRAGONS = 158,
	ROC_KNIGHTS = 159,
	BICYCLE_RIDERS = 160,
	SOME_MAGIC_BOMBER = 161,
	BRONZE_GLIDERS = 162,
	NAPHTA_DROPPERS = 163,
	SOME_MAGIC_SKYHUNTER_I_GUESS = 164,
	SKY_CASTLE = 165,
	LIGHTNING_BOMBERS = 166,
	SOME_MECH_BOMBER = 167,
	SOME_BOMBER_AGAIN = 168,
	CANNONS = 169,
	PIT_CANNONS = 170,
	MERCURY_GUNS = 171,
	TANK_LOOKING_THING = 172,
	WALL_SLAMMERS = 173,
	BLACK_ICE_THROWERS = 174,
	WITCH_OF_ICETHORN = 175,
	PALE_MARSHAL = 176,
	KRAEKVAR = 177,
	CLAW_THE_ASSASSIN = 178,
	RATLORD = 179,
	FLYING_DEVIL_WOMAN = 180,
	THE_LASHER = 181,
	SKELETON_COMMANDER = 182,
	LEECH_KING = 183,

} ArmyType;

typedef struct Army
{
	int32_t position_hex;
	ArmyType type;
	int16_t movement_points_max;
	int16_t movement_points_current;
} Army;

typedef struct MapNode
{
	int32_t index; // this might be superfluous? it's the same as the index in map_nodes array. Think about it!
	int16_t x;
	int16_t y;

//temp (or maybe not???) // maybe could do somekind of hot/cold-split for this struct later on
	int32_t n_north;
	int32_t n_northeast;
	int32_t n_southeast;
	int32_t n_south;
	int32_t n_southwest;
	int32_t n_northwest;
	int32_t n_count;

// edges
	int32_t edge_north;
	int32_t edge_northeast;
	int32_t edge_southeast;
	int32_t edge_south;
	int32_t edge_southwest;
	int32_t edge_northwest;
} MapNode;

typedef struct MapEdge
{
	int32_t index; // possibly superfluous as well
	int32_t start_node_index;
	int32_t end_node_index;
	int16_t cost;
	int16_t direction; // 0 = north, 1 = northeast, 2 = southeast, 3 = south, 4 = southwest, 5 = northwest
} MapEdge;

#if 0
typedef struct PathStep
{
	int32_t this_node;
	int32_t came_from_node;
	int32_t g_score;
	int32_t f_score;
} PathStep;
#else
typedef struct PathStep
{
	int32_t start_node;
	int32_t edge;
	int32_t g_score;
	int32_t f_score;
} PathStep;
#endif

typedef struct OpenSetLeaf
{
	int32_t parent = -2; // parent -1 is root
	int32_t left_child = -2;
	int32_t right_child = -2;
	int32_t g_score = -2; // key value, sort by this
	int32_t h_score = -2; // HexDistance(map_nodes[value], map_nodes[goal_node])
	// int32_t f_score = g_score + h_score;
	int32_t map_index = -2; // index to map_nodes array
} OpenSetLeaf;

typedef struct ClosedSetLeaf
{
	int32_t parent = -2;
	int32_t left_child = -2;
	int32_t right_child = -2;
	int32_t map_index = -2;
} ClosedSetLeaf;