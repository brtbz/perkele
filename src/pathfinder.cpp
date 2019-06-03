bool OpenSetIsEmpty()
{
	return open_set_count == 0 ? true : false;
}

bool ClosedSetIsEmpty()
{
	return closed_set_count == 0 ? true : false;
}

bool OpenSetMapIndicesIsEmpty()
{
	return open_set_map_indices_count == 0 ? true : false;
}

int32_t AddOpenSetMapIndicesLeaf(int32_t map_index)
{
	if ( open_set_map_indices_write_head >= OPEN_SET_MAX_SIZE )
	{
		return -1;
	}
	if ( OpenSetMapIndicesIsEmpty() )
	{
		open_set_map_indices[ open_set_map_indices_root_index ].map_index = map_index;
		open_set_map_indices[ open_set_map_indices_root_index ].parent = -1;
		open_set_map_indices[ open_set_map_indices_root_index ].left_child = -1;
		open_set_map_indices[ open_set_map_indices_root_index ].right_child = -1;

		open_set_map_indices_write_head++;
		open_set_map_indices_count++;
	}
	else
	{
		bool place_for_new_node_found = false;
		int32_t read_head = open_set_map_indices_root_index;

		while (!place_for_new_node_found)
		{
			if ( map_index > open_set_map_indices[read_head].map_index )
			{
				if ( open_set_map_indices[read_head].right_child == -1 )
				{
					open_set_map_indices[ open_set_map_indices_write_head ].map_index = map_index;
					open_set_map_indices[ open_set_map_indices_write_head ].parent = read_head;
					open_set_map_indices[ open_set_map_indices_write_head ].left_child = -1;
					open_set_map_indices[ open_set_map_indices_write_head ].right_child = -1;
					open_set_map_indices[ read_head ].right_child = open_set_map_indices_write_head;

					open_set_map_indices_write_head++;
					open_set_map_indices_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = open_set_map_indices[ read_head ].right_child;
				}
			}
			else // new key equal or less
			{
				if ( open_set_map_indices[read_head].left_child == -1 )
				{
					open_set_map_indices[ open_set_map_indices_write_head ].map_index = map_index;
					open_set_map_indices[ open_set_map_indices_write_head ].parent = read_head;
					open_set_map_indices[ open_set_map_indices_write_head ].left_child = -1;
					open_set_map_indices[ open_set_map_indices_write_head ].right_child = -1;
					open_set_map_indices[ read_head ].left_child = open_set_map_indices_write_head;

					open_set_map_indices_write_head++;
					open_set_map_indices_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = open_set_map_indices[ read_head ].left_child;
				}
			}
		}
	}
	return 0;
}

int32_t AddOpenSetLeaf( float f_score, int32_t g_score, int32_t map_index, int32_t came_along_edge )
{
	nodes_that_were_in_open_set_debug[number_of_nodes_that_were_in_open_set_debug] = (uint32_t)map_index;
	number_of_nodes_that_were_in_open_set_debug++;

	if ( open_set_write_head >= OPEN_SET_MAX_SIZE )
	{
		return -1;
	}
	if ( OpenSetIsEmpty() )
	{
		open_set[ open_set_root_index ].f_score = f_score;
		open_set[ open_set_root_index ].g_score = g_score;
		open_set[ open_set_root_index ].map_index = map_index;
		open_set[ open_set_root_index ].came_along_edge = came_along_edge;
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
					open_set[ open_set_write_head ].came_along_edge = came_along_edge;
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
					open_set[ open_set_write_head ].came_along_edge = came_along_edge;
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
		closed_set[ closed_set_root_index ].map_index = map_index;
		closed_set[ closed_set_root_index ].parent = -1;
		closed_set[ closed_set_root_index ].left_child = -1;
		closed_set[ closed_set_root_index ].right_child = -1;

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
int32_t PullMapIndexWithLowestFScoreFromOpenSet(int32_t *map_index, int32_t *g_score, int32_t *came_along_edge)
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
		*came_along_edge = open_set[read_head].came_along_edge;

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

bool MapIndexIsInOpenSetMapIndices(int32_t map_index)
{
	if (OpenSetMapIndicesIsEmpty())
	{
		return false;
	}
	else
	{
		int32_t read_head = open_set_map_indices_root_index;
		bool keep_looking = true;
		while (keep_looking)
		{
			if ( open_set_map_indices[read_head].map_index == map_index )
			{
				return true;
			}
			if ( map_index < open_set_map_indices[read_head].map_index )
			{
				open_set_map_indices[read_head].left_child != -1 ? read_head = open_set_map_indices[read_head].left_child : keep_looking = false;
			}
			else // if ( map_index > closed_set[read_head].map_index )
			{
				open_set_map_indices[read_head].right_child != -1 ? read_head = open_set_map_indices[read_head].right_child : keep_looking = false;
			}
		}
	}
	return false;
}

void AnalyzeMapNode(int32_t map_index, int32_t accumulated_g_score, ivec2 goal_hex)
{
	int32_t h_score;
	int32_t g_score;
	float f_score;
	int32_t neighbour;

	for (int i = 0; i < 6; i++)
	{
		if ( map_nodes[ map_index ].edge[i] != -1 )
		{
			neighbour = map_edges[ map_nodes[ map_index ].edge[i] ].end_node_index;
			if (!MapIndexIsInClosedSet(neighbour) && !MapIndexIsInOpenSetMapIndices(neighbour))
			{
				if ( map_nodes[neighbour].terrain == IMPASSABLE || map_nodes[neighbour].occupier != -1 )
				{
					AddClosedSetLeaf(neighbour);
				}
				else
				{
					ivec2 neighbour_hex = { map_nodes[neighbour].x, map_nodes[neighbour].y };
					h_score = CalculateHexDistance(neighbour_hex, goal_hex);
					g_score = accumulated_g_score + map_edges[ map_nodes[map_index].edge[i] ].cost;
					if (neighbour_hex.x == goal_hex.x && neighbour_hex.y == goal_hex.y )
					{
						f_score = 0.0f;
					}
					else
					{
						f_score = pathfind_weight_g * (float)g_score + pathfind_weight_h * (float)h_score;
					}
					AddOpenSetLeaf( f_score, g_score, neighbour, map_nodes[ map_index ].edge[i] );
					AddOpenSetMapIndicesLeaf(neighbour);
				}
			}
		}
	}

	AddClosedSetLeaf(map_index);
}
/*
uint64_t starting1 = SDL_GetPerformanceCounter();
uint64_t stopping1 = SDL_GetPerformanceCounter();
double calculating1 = (double)(stopping1 - starting1) / (double)perf_freq;
printf("This took %.04g\n", calculating1*1000.0);
*/

uint32_t ReconstructPath(int32_t start, int32_t goal)
{
	int path_size = 0;
	int n = 0;


	path_edges[n] = came_along_edges[goal];
	int32_t prev_node = map_edges[ came_along_edges[goal] ].start_node_index;

	while ( prev_node != start && path_size < 512 )
	{
		n++;
		path_size++;
		path_edges[n] = came_along_edges[prev_node];
		prev_node = map_edges[ came_along_edges[prev_node] ].start_node_index;
	}

	return path_size;
}

int32_t FindPath(int32_t start, int32_t goal)
{
	if (goal < 0)
	{
		return -1;
	}
	if ( map_nodes[goal].terrain == IMPASSABLE )
	{
		return -2;
	}
	if ( map_nodes[goal].occupier != -1 )
	{
		return -3;
	}

	uint64_t initiating_pathfinding = SDL_GetPerformanceCounter();

	number_of_nodes_that_were_in_open_set_debug = 0;

	open_set_map_indices_count = 0;
	open_set_map_indices_write_head = 0;
	open_set_map_indices_root_index = 0;

	open_set_write_head = 0;
	open_set_root_index = 0;
	open_set_count = 0;
	closed_set_write_head = 0;
	closed_set_root_index = 0;
	closed_set_count = 0;
	path_edges_size = 0;

	ivec2 start_hex = { map_nodes[start].x, map_nodes[start].y };
	ivec2 goal_hex = { map_nodes[goal].x, map_nodes[goal].y };

	int32_t h_score = CalculateHexDistance(start_hex, goal_hex);
	int32_t g_score = 0;
	float f_score = (float)g_score + pathfind_weight_h * (float) h_score;

	bool goal_found = false;
	int32_t temp_score = 1337;

	AddOpenSetLeaf( f_score, g_score, start, -1 );
	AddOpenSetMapIndicesLeaf(start);

	while (!goal_found && !OpenSetIsEmpty())
	{
		int32_t map_index;
		int32_t accumulated_g_score;
		int32_t came_along_edge;
		PullMapIndexWithLowestFScoreFromOpenSet(&map_index, &accumulated_g_score, &came_along_edge);

		//ivec2 current_hex = { map_nodes[map_index].x, map_nodes[map_index].y };
		//h_score = CalculateHexDistance(current_hex, goal_hex);
		came_along_edges[map_index] = came_along_edge;
		if ( map_index == goal )
		{
			goal_found = true;
			path_edges_size = ReconstructPath(start, goal);
			temp_score = accumulated_g_score;
			break;
		}
		AnalyzeMapNode(map_index, accumulated_g_score, goal_hex);
	}

	uint64_t pathfinding_completed = SDL_GetPerformanceCounter();

	double pathfinding_time = (double)(pathfinding_completed - initiating_pathfinding) / (double)perf_freq;

	snprintf(dumb_debug_string, 256, "Path found in %.04g milliseconds.\n", pathfinding_time*1000.0);

	return temp_score;
}
