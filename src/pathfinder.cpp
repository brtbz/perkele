bool OpenSetIsEmpty()
{
	return open_set_count == 0 ? true : false;
}

bool ClosedSetIsEmpty()
{
	return closed_set_count == 0 ? true : false;
}

int32_t AddOpenSetLeaf( int32_t f_score, int32_t g_score, int32_t map_index )
{
	if ( open_set_write_head >= OPEN_SET_MAX_SIZE )
	{
		return -1;
	}
	if ( OpenSetIsEmpty() )
	{
		open_set[ open_set_root_index ].f_score = f_score;
		open_set[ open_set_root_index ].g_score = g_score;
		open_set[ open_set_root_index ].map_index = map_index;
		open_set[ open_set_root_index ].parent = -1;
		open_set[ open_set_root_index ].left_child = -1;
		open_set[ open_set_root_index ].right_child = -1;

		open_set_write_head++;
		open_set_count++;
	}
	else
	{
		bool place_for_new_node_found = false;
		int32_t read_head = open_set_root_index;

		while (!place_for_new_node_found)
		{
			if ( f_score > open_set[read_head].f_score )
			{
				if ( open_set[read_head].right_child == -1 )
				{
					open_set[ open_set_write_head ].f_score = f_score;
					open_set[ open_set_write_head ].g_score = g_score;
					open_set[ open_set_write_head ].map_index = map_index;
					open_set[ open_set_write_head ].parent = read_head;
					open_set[ open_set_write_head ].left_child = -1;
					open_set[ open_set_write_head ].right_child = -1;
					open_set[ read_head ].right_child = open_set_write_head;

					open_set_write_head++;
					open_set_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = open_set[ read_head ].right_child;
				}
			}
			else // new key equal or less
			{
				if ( open_set[read_head].left_child == -1 )
				{
					open_set[ open_set_write_head ].f_score = f_score;
					open_set[ open_set_write_head ].g_score = g_score;
					open_set[ open_set_write_head ].map_index = map_index;
					open_set[ open_set_write_head ].parent = read_head;
					open_set[ open_set_write_head ].left_child = -1;
					open_set[ open_set_write_head ].right_child = -1;
					open_set[ read_head ].left_child = open_set_write_head;

					open_set_write_head++;
					open_set_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = open_set[ read_head ].left_child;
				}
			}
		}
	}
	return 0;
}

int32_t AddClosedSetLeaf(int32_t map_index)
{
	if ( closed_set_write_head >= CLOSED_SET_MAX_SIZE )
	{
		return -1;
	}
	if ( ClosedSetIsEmpty() )
	{
		closed_set[ open_set_root_index ].map_index = map_index;
		closed_set[ open_set_root_index ].parent = -1;
		closed_set[ open_set_root_index ].left_child = -1;
		closed_set[ open_set_root_index ].right_child = -1;

		closed_set_write_head++;
		closed_set_count++;
	}
	else
	{
		bool place_for_new_node_found = false;
		int32_t read_head = closed_set_root_index;

		while (!place_for_new_node_found)
		{
			if ( map_index > closed_set[read_head].map_index )
			{
				if ( closed_set[read_head].right_child == -1 )
				{
					closed_set[ closed_set_write_head ].map_index = map_index;
					closed_set[ closed_set_write_head ].parent = read_head;
					closed_set[ closed_set_write_head ].left_child = -1;
					closed_set[ closed_set_write_head ].right_child = -1;
					closed_set[ read_head ].right_child = closed_set_write_head;

					closed_set_write_head++;
					closed_set_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = closed_set[ read_head ].right_child;
				}
			}
			else // new key equal or less
			{
				if ( closed_set[read_head].left_child == -1 )
				{
					closed_set[ closed_set_write_head ].map_index = map_index;
					closed_set[ closed_set_write_head ].parent = read_head;
					closed_set[ closed_set_write_head ].left_child = -1;
					closed_set[ closed_set_write_head ].right_child = -1;
					closed_set[ read_head ].left_child = closed_set_write_head;

					closed_set_write_head++;
					closed_set_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = closed_set[ read_head ].left_child;
				}
			}
		}
	}
	return 0;
}

// i want map_index, but these are sorted by f_score. okay?
int32_t PullMapIndexWithLowestFScoreFromOpenSetAndDontForgetItsGScoreEither(int32_t *map_index, int32_t *g_score)
{
	if (OpenSetIsEmpty())
	{
		return -1;
	}
	else
	{
		// find and get map_index
		int32_t read_head = open_set_root_index;
		while( open_set[read_head].left_child != -1 )
		{
			read_head = open_set[read_head].left_child;
		}
		*map_index = open_set[read_head].map_index;
		*g_score = open_set[read_head].g_score;

		// rearrange

		// am i root?
		if (open_set[read_head].parent == -1)
		{
			if (open_set[read_head].right_child != -1)
			{
				open_set[ open_set[read_head].right_child ].parent = -1;
				open_set_root_index = open_set[read_head].right_child;
			}
		}
		// not root, but got right child?
		else if ( open_set[read_head].right_child != -1 )
		{
			open_set[ open_set[ read_head ].right_child ].parent = open_set[ read_head ].parent;
			open_set[ open_set[ read_head ].parent ].left_child = open_set[ read_head ].right_child;
		}
		else
		{
			open_set[ open_set[ read_head ].parent ].left_child = -1;
		}

		open_set_count--;
	}
	return 0;
}

bool MapIndexIsInClosedSet(int32_t map_index)
{
	if (ClosedSetIsEmpty())
	{
		return false;
	}
	else
	{
		int32_t read_head = closed_set_root_index;
		bool keep_looking = true;
		while (keep_looking)
		{
			if ( closed_set[read_head].map_index == map_index )
			{
				return true;
			}
			if ( map_index < closed_set[read_head].map_index )
			{
				closed_set[read_head].left_child != -1 ? read_head = closed_set[read_head].left_child : keep_looking = false;
			}
			else // if ( map_index > closed_set[read_head].map_index )
			{
				closed_set[read_head].right_child != -1 ? read_head = closed_set[read_head].right_child : keep_looking = false;
			}
		}
	}
	return false;
}

void ANALAIS(int32_t map_index, int32_t accumulated_g_score, ivec2 goal_hex)
{
	//int32_t h_score;
	//int32_t g_score;
	//int32_t f_score;
	int32_t neighbor;
	if ( map_nodes[ map_index ].edge_north != -1 )
	{
		neighbor = map_edges[ map_nodes[map_index].edge_north ].end_node_index;
		if (!MapIndexIsInClosedSet(neighbor))
		{
			ivec2 neighbor_hex = { map_nodes[neighbor].x, map_nodes[neighbor].y };
			int32_t h_score = CalculateHexDistance(neighbor_hex, goal_hex);
			int32_t g_score = accumulated_g_score + map_edges[ map_nodes[map_index].edge_north ].cost;
			int32_t f_score = g_score + h_score;
			printf("NEIGHBOR %d X%d Y%d H_SCORE: %d G_SCORE:%d F_SCORE:%d\n", neighbor, neighbor_hex.x, neighbor_hex.y, h_score, g_score, f_score);
			AddOpenSetLeaf( f_score, g_score, neighbor );
		}
	}

	if ( map_nodes[ map_index ].edge_northeast != -1 )
	{
		neighbor = map_edges[ map_nodes[map_index].edge_northeast ].end_node_index;
		if (!MapIndexIsInClosedSet(neighbor))
		{
			ivec2 neighbor_hex = { map_nodes[neighbor].x, map_nodes[neighbor].y };
			int32_t h_score = CalculateHexDistance(neighbor_hex, goal_hex);
			int32_t g_score = accumulated_g_score + map_edges[ map_nodes[map_index].edge_northeast ].cost;
			int32_t f_score = g_score + h_score;
			AddOpenSetLeaf( f_score, g_score, neighbor );
		}
	}

	if ( map_nodes[ map_index ].edge_southeast != -1 )
	{
		neighbor = map_edges[ map_nodes[map_index].edge_southeast ].end_node_index;
		if (!MapIndexIsInClosedSet(neighbor))
		{
			ivec2 neighbor_hex = { map_nodes[neighbor].x, map_nodes[neighbor].y };
			int32_t h_score = CalculateHexDistance(neighbor_hex, goal_hex);
			int32_t g_score = accumulated_g_score + map_edges[ map_nodes[map_index].edge_southeast ].cost;
			int32_t f_score = g_score + h_score;
			AddOpenSetLeaf( f_score, g_score, neighbor );
		}
	}

	if ( map_nodes[ map_index ].edge_south != -1 )
	{
		neighbor = map_edges[ map_nodes[map_index].edge_south ].end_node_index;
		if (!MapIndexIsInClosedSet(neighbor))
		{
			ivec2 neighbor_hex = { map_nodes[neighbor].x, map_nodes[neighbor].y };
			int32_t h_score = CalculateHexDistance(neighbor_hex, goal_hex);
			int32_t g_score = accumulated_g_score + map_edges[ map_nodes[map_index].edge_south ].cost;
			int32_t f_score = g_score + h_score;
			AddOpenSetLeaf( f_score, g_score, neighbor );
		}
	}

	if ( map_nodes[ map_index ].edge_southwest != -1 )
	{
		neighbor = map_edges[ map_nodes[map_index].edge_southwest ].end_node_index;
		if (!MapIndexIsInClosedSet(neighbor))
		{
			ivec2 neighbor_hex = { map_nodes[neighbor].x, map_nodes[neighbor].y };
			int32_t h_score = CalculateHexDistance(neighbor_hex, goal_hex);
			int32_t g_score = accumulated_g_score + map_edges[ map_nodes[map_index].edge_southwest ].cost;
			int32_t f_score = g_score + h_score;
			AddOpenSetLeaf( f_score, g_score, neighbor );
		}
	}

	if ( map_nodes[ map_index ].edge_northwest != -1 )
	{
		neighbor = map_edges[ map_nodes[map_index].edge_northwest ].end_node_index;
		if (!MapIndexIsInClosedSet(neighbor))
		{
			ivec2 neighbor_hex = { map_nodes[neighbor].x, map_nodes[neighbor].y };
			int32_t h_score = CalculateHexDistance(neighbor_hex, goal_hex);
			int32_t g_score = accumulated_g_score + map_edges[ map_nodes[map_index].edge_northwest ].cost;
			int32_t f_score = g_score + h_score;
			AddOpenSetLeaf( f_score, g_score, neighbor );
		}
	}

	AddClosedSetLeaf(map_index);
}

int32_t FindPath(int32_t start, int32_t goal)
{
	uint64_t initiating_pathfinding = SDL_GetPerformanceCounter();

	open_set_write_head = 0;
	open_set_root_index = 0;
	open_set_count = 0;
	closed_set_write_head = 0;
	closed_set_root_index = 0;
	closed_set_count = 0;

	ivec2 start_hex = { map_nodes[start].x, map_nodes[start].y };
	ivec2 goal_hex = { map_nodes[goal].x, map_nodes[goal].y };

	int32_t h_score = CalculateHexDistance(start_hex, goal_hex);
	int32_t g_score = 0;
	int32_t f_score = g_score + h_score;

	bool goal_found = false;
	int32_t temp_score = 1337;


	AddOpenSetLeaf( f_score, g_score, start );

	while (!goal_found && !OpenSetIsEmpty())
	{
		int32_t map_index;
		int32_t accumulated_g_score;
		PullMapIndexWithLowestFScoreFromOpenSetAndDontForgetItsGScoreEither(&map_index, &accumulated_g_score);

		ivec2 current_hex = { map_nodes[map_index].x, map_nodes[map_index].y };
		h_score = CalculateHexDistance(current_hex, goal_hex);
		if ( h_score == 0)
		{
			goal_found = true;
		}
		temp_score = accumulated_g_score;
		printf("-----------LETS ANALAIS\n");
		ANALAIS(map_index, accumulated_g_score, goal_hex);
	}

	uint64_t pathfinding_completed = SDL_GetPerformanceCounter();

	double pathfinding_time = (double)(pathfinding_completed - initiating_pathfinding) / (double)perf_freq;
	//printf("Path found in %.04g milliseconds.\n", pathfinding_time*1000.0);

	snprintf(dumb_debug_string, 256, "Path found in %.04g milliseconds.\n", pathfinding_time*1000.0);

	return temp_score;
}

/*
1. get from open_set entry with top priority (lowest f_score)
2. analyze all its neighbour nodes (or edges) and if they are not found in closed set, then add them to open set
3. add current node to closed set, and remove it from open set
4. goto 1 if open set not empty
*/
