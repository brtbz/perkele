int32_t f_score(OpenSetLeaf osl)
{
	return osl.g_score + osl.h_score;
}

// lowest score is top priority

int32_t PeekTopPriority(OpenSetLeaf *osl, int32_t size)
{
	int32_t lowest_score = 2147483647;

	int32_t i = 0;
	while ( osl[i].left_child > 1 )
	{
		i = osl[i].left_child;
	}
	lowest_score = f_score(osl[i]);
	// right children are all higher scores(?)
	return lowest_score;
}

int32_t FindPath(int32_t start, int32_t goal)
{
	/*
	procession_of_nodes[0] = start;
	path_map[0].start_node = start;
	path_map[0].g_score = 0;
	*/

	int32_t open_set_count = 0;

	ivec2 start_hex = { map_nodes[start].x, map_nodes[start].y };
	ivec2 goal_hex = { map_nodes[goal].x, map_nodes[goal].y };

	open_set[0].parent = -1;
	open_set[0].left_child = -1;
	open_set[0].right_child = -1;
	open_set[0].g_score = 0;
	open_set[0].h_score = CalculateHexDistance( start_hex, goal_hex );
	open_set[0].map_index = start;
	open_set_count++;

	int32_t temp_score = PeekTopPriority(&open_set[0], 16384);

	//while (open_set_count > 0)
	{

	}

	return temp_score;
}
