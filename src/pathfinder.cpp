/*
units can pass through other units of the same faction

1. pathfinder needs to know WHO is going to use the path (or maybe just which faction?)
2. if map node has occupier, but factions match, it goes to open set instead
3. reachable nodes set that is made like this, requires another pass where ALL the nodes with ANY occupier are removed
*/

bool OpenSetIsEmpty(Pathfinder *pf)
{
	return pf->open_set_count == 0 ? true : false;
}

bool ClosedSetIsEmpty(Pathfinder *pf)
{
	return pf->closed_set_count == 0 ? true : false;
}

bool OpenSetMapIndicesIsEmpty(Pathfinder *pf)
{
	return pf->open_set_map_indices_count == 0 ? true : false;
}

int32_t AddOpenSetMapIndicesLeaf(Pathfinder *pf, int32_t map_index)
{
	if ( pf->open_set_map_indices_write_head >= OPEN_SET_MAX_SIZE )
	{
		return -1;
	}
	if ( OpenSetMapIndicesIsEmpty(pf) )
	{
		pf->open_set_map_indices[ pf->open_set_map_indices_root_index ].map_index = map_index;
		pf->open_set_map_indices[ pf->open_set_map_indices_root_index ].parent = -1;
		pf->open_set_map_indices[ pf->open_set_map_indices_root_index ].left_child = -1;
		pf->open_set_map_indices[ pf->open_set_map_indices_root_index ].right_child = -1;

		pf->open_set_map_indices_write_head++;
		pf->open_set_map_indices_count++;
	}
	else
	{
		bool place_for_new_node_found = false;
		int32_t read_head = pf->open_set_map_indices_root_index;

		while (!place_for_new_node_found)
		{
			if ( map_index > pf->open_set_map_indices[read_head].map_index )
			{
				if ( pf->open_set_map_indices[read_head].right_child == -1 )
				{
					pf->open_set_map_indices[ pf->open_set_map_indices_write_head ].map_index = map_index;
					pf->open_set_map_indices[ pf->open_set_map_indices_write_head ].parent = read_head;
					pf->open_set_map_indices[ pf->open_set_map_indices_write_head ].left_child = -1;
					pf->open_set_map_indices[ pf->open_set_map_indices_write_head ].right_child = -1;
					pf->open_set_map_indices[ read_head ].right_child = pf->open_set_map_indices_write_head;

					pf->open_set_map_indices_write_head++;
					pf->open_set_map_indices_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = pf->open_set_map_indices[ read_head ].right_child;
				}
			}
			else // new key equal or less
			{
				if ( pf->open_set_map_indices[read_head].left_child == -1 )
				{
					pf->open_set_map_indices[ pf->open_set_map_indices_write_head ].map_index = map_index;
					pf->open_set_map_indices[ pf->open_set_map_indices_write_head ].parent = read_head;
					pf->open_set_map_indices[ pf->open_set_map_indices_write_head ].left_child = -1;
					pf->open_set_map_indices[ pf->open_set_map_indices_write_head ].right_child = -1;
					pf->open_set_map_indices[ read_head ].left_child = pf->open_set_map_indices_write_head;

					pf->open_set_map_indices_write_head++;
					pf->open_set_map_indices_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = pf->open_set_map_indices[ read_head ].left_child;
				}
			}
		}
	}
	return 0;
}

int32_t AddOpenSetLeaf( Pathfinder *pf, float f_score, int32_t g_score, int32_t map_index, int32_t came_along_edge )
{
	pf->nodes_that_were_in_open_set_debug[ pf->number_of_nodes_that_were_in_open_set_debug ] = (uint32_t)map_index;
	pf->number_of_nodes_that_were_in_open_set_debug++;

	if ( pf->open_set_write_head >= OPEN_SET_MAX_SIZE )
	{
		return -1;
	}
	if ( OpenSetIsEmpty(pf) )
	{
		pf->open_set[ pf->open_set_root_index ].f_score = f_score;
		pf->open_set[ pf->open_set_root_index ].g_score = g_score;
		pf->open_set[ pf->open_set_root_index ].map_index = map_index;
		pf->open_set[ pf->open_set_root_index ].came_along_edge = came_along_edge;
		pf->open_set[ pf->open_set_root_index ].parent = -1;
		pf->open_set[ pf->open_set_root_index ].left_child = -1;
		pf->open_set[ pf->open_set_root_index ].right_child = -1;

		pf->open_set_write_head++;
		pf->open_set_count++;
	}
	else
	{
		bool place_for_new_node_found = false;
		int32_t read_head = pf->open_set_root_index;

		while (!place_for_new_node_found)
		{
			if ( f_score > pf->open_set[read_head].f_score )
			{
				if ( pf->open_set[read_head].right_child == -1 )
				{
					pf->open_set[ pf->open_set_write_head ].f_score = f_score;
					pf->open_set[ pf->open_set_write_head ].g_score = g_score;
					pf->open_set[ pf->open_set_write_head ].map_index = map_index;
					pf->open_set[ pf->open_set_write_head ].came_along_edge = came_along_edge;
					pf->open_set[ pf->open_set_write_head ].parent = read_head;
					pf->open_set[ pf->open_set_write_head ].left_child = -1;
					pf->open_set[ pf->open_set_write_head ].right_child = -1;
					pf->open_set[ read_head ].right_child = pf->open_set_write_head;

					pf->open_set_write_head++;
					pf->open_set_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = pf->open_set[ read_head ].right_child;
				}
			}
			else // new key equal or less
			{
				if ( pf->open_set[read_head].left_child == -1 )
				{
					pf->open_set[ pf->open_set_write_head ].f_score = f_score;
					pf->open_set[ pf->open_set_write_head ].g_score = g_score;
					pf->open_set[ pf->open_set_write_head ].map_index = map_index;
					pf->open_set[ pf->open_set_write_head ].came_along_edge = came_along_edge;
					pf->open_set[ pf->open_set_write_head ].parent = read_head;
					pf->open_set[ pf->open_set_write_head ].left_child = -1;
					pf->open_set[ pf->open_set_write_head ].right_child = -1;
					pf->open_set[ read_head ].left_child = pf->open_set_write_head;

					pf->open_set_write_head++;
					pf->open_set_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = pf->open_set[ read_head ].left_child;
				}
			}
		}
	}
	return 0;
}

int32_t AddClosedSetLeaf(Pathfinder *pf, int32_t map_index)
{
	if ( pf->closed_set_write_head >= CLOSED_SET_MAX_SIZE )
	{
		return -1;
	}
	if ( ClosedSetIsEmpty(pf) )
	{
		pf->closed_set[ pf->closed_set_root_index ].map_index = map_index;
		pf->closed_set[ pf->closed_set_root_index ].parent = -1;
		pf->closed_set[ pf->closed_set_root_index ].left_child = -1;
		pf->closed_set[ pf->closed_set_root_index ].right_child = -1;

		pf->closed_set_write_head++;
		pf->closed_set_count++;
	}
	else
	{
		bool place_for_new_node_found = false;
		int32_t read_head = pf->closed_set_root_index;

		while (!place_for_new_node_found)
		{
			if ( map_index > pf->closed_set[read_head].map_index )
			{
				if ( pf->closed_set[read_head].right_child == -1 )
				{
					pf->closed_set[ pf->closed_set_write_head ].map_index = map_index;
					pf->closed_set[ pf->closed_set_write_head ].parent = read_head;
					pf->closed_set[ pf->closed_set_write_head ].left_child = -1;
					pf->closed_set[ pf->closed_set_write_head ].right_child = -1;
					pf->closed_set[ read_head ].right_child = pf->closed_set_write_head;

					pf->closed_set_write_head++;
					pf->closed_set_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = pf->closed_set[ read_head ].right_child;
				}
			}
			else // new key equal or less
			{
				if ( pf->closed_set[read_head].left_child == -1 )
				{
					pf->closed_set[ pf->closed_set_write_head ].map_index = map_index;
					pf->closed_set[ pf->closed_set_write_head ].parent = read_head;
					pf->closed_set[ pf->closed_set_write_head ].left_child = -1;
					pf->closed_set[ pf->closed_set_write_head ].right_child = -1;
					pf->closed_set[ read_head ].left_child = pf->closed_set_write_head;

					pf->closed_set_write_head++;
					pf->closed_set_count++;
					place_for_new_node_found = true;
				}
				else
				{
					read_head = pf->closed_set[ read_head ].left_child;
				}
			}
		}
	}
	return 0;
}

// i want map_index, but these are sorted by f_score. okay?
int32_t PullMapIndexWithLowestFScoreFromOpenSet(Pathfinder *pf, int32_t *map_index, int32_t *g_score, int32_t *came_along_edge)
{
	if (OpenSetIsEmpty(pf))
	{
		return -1;
	}
	else
	{
		// find and get map_index
		int32_t read_head = pf->open_set_root_index;
		while( pf->open_set[read_head].left_child != -1 )
		{
			read_head = pf->open_set[read_head].left_child;
		}
		*map_index = pf->open_set[read_head].map_index;
		*g_score = pf->open_set[read_head].g_score;
		*came_along_edge = pf->open_set[read_head].came_along_edge;

		// rearrange

		// am i root?
		if ( pf->open_set[read_head].parent == -1)
		{
			if ( pf->open_set[read_head].right_child != -1)
			{
				pf->open_set[ pf->open_set[read_head].right_child ].parent = -1;
				pf->open_set_root_index = pf->open_set[read_head].right_child;
			}
		}
		// not root, but got right child?
		else if ( pf->open_set[read_head].right_child != -1 )
		{
			pf->open_set[ pf->open_set[ read_head ].right_child ].parent = pf->open_set[ read_head ].parent;
			pf->open_set[ pf->open_set[ read_head ].parent ].left_child = pf->open_set[ read_head ].right_child;
		}
		else
		{
			pf->open_set[ pf->open_set[ read_head ].parent ].left_child = -1;
		}

		pf->open_set_count--;
	}
	return 0;
}

bool MapIndexIsInClosedSet(Pathfinder *pf, int32_t map_index)
{
	if (ClosedSetIsEmpty(pf))
	{
		return false;
	}
	else
	{
		int32_t read_head = pf->closed_set_root_index;
		bool keep_looking = true;
		while (keep_looking)
		{
			if ( pf->closed_set[read_head].map_index == map_index )
			{
				return true;
			}
			if ( map_index < pf->closed_set[read_head].map_index )
			{
				pf->closed_set[read_head].left_child != -1 ? read_head = pf->closed_set[read_head].left_child : keep_looking = false;
			}
			else // if ( map_index > closed_set[read_head].map_index )
			{
				pf->closed_set[read_head].right_child != -1 ? read_head = pf->closed_set[read_head].right_child : keep_looking = false;
			}
		}
	}
	return false;
}

bool MapIndexIsInOpenSetMapIndices(Pathfinder *pf, int32_t map_index)
{
	if (OpenSetMapIndicesIsEmpty(pf))
	{
		return false;
	}
	else
	{
		int32_t read_head = pf->open_set_map_indices_root_index;
		bool keep_looking = true;
		while (keep_looking)
		{
			if ( pf->open_set_map_indices[read_head].map_index == map_index )
			{
				return true;
			}
			if ( map_index < pf->open_set_map_indices[read_head].map_index )
			{
				pf->open_set_map_indices[read_head].left_child != -1 ? read_head = pf->open_set_map_indices[read_head].left_child : keep_looking = false;
			}
			else // if ( map_index > closed_set[read_head].map_index )
			{
				pf->open_set_map_indices[read_head].right_child != -1 ? read_head = pf->open_set_map_indices[read_head].right_child : keep_looking = false;
			}
		}
	}
	return false;
}

void AnalyzeMapNode(Pathfinder *pf, int32_t map_index, int32_t accumulated_g_score, ivec2 goal_hex, int faction)
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
			if (!MapIndexIsInClosedSet(pf, neighbour) && !MapIndexIsInOpenSetMapIndices(pf, neighbour))
			{
				if ( map_nodes[neighbour].terrain == IMPASSABLE || map_nodes[neighbour].occupier != -1 && test_armies[ map_nodes[neighbour].occupier ].faction != faction )
				{
					AddClosedSetLeaf(pf, neighbour);
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
						f_score = pf->pathfind_weight_g * (float)g_score + pf->pathfind_weight_h * (float)h_score;
					}
					AddOpenSetLeaf( pf, f_score, g_score, neighbour, map_nodes[ map_index ].edge[i] );
					AddOpenSetMapIndicesLeaf(pf, neighbour);
				}
			}
		}
	}
	AddClosedSetLeaf(pf, map_index);
}

void AnalyzeMapNodeForReachableNodes(Pathfinder *pf, int32_t map_index, int32_t accumulated_g_score, int available_movement_points, int faction)
{
	int32_t g_score;
	int32_t neighbour;

	for (int i = 0; i < 6; i++)
	{
		if ( map_nodes[ map_index ].edge[i] != -1 )
		{
			neighbour = map_edges[ map_nodes[ map_index ].edge[i] ].end_node_index;
			if (!MapIndexIsInClosedSet(pf, neighbour) && !MapIndexIsInOpenSetMapIndices(pf, neighbour))
			{
				if ( map_nodes[neighbour].terrain == IMPASSABLE || map_nodes[neighbour].occupier != -1 && test_armies[ map_nodes[neighbour].occupier ].faction != faction )
				{
					AddClosedSetLeaf(pf, neighbour);
				}
				else
				{
					g_score = accumulated_g_score + map_edges[ map_nodes[map_index].edge[i] ].cost;

					if (g_score > available_movement_points)
					{
						AddClosedSetLeaf(pf, neighbour);
					}
					else
					{
						float f_score = (float)g_score;
						AddOpenSetLeaf( pf, f_score, g_score, neighbour, map_nodes[ map_index ].edge[i] );
						AddOpenSetMapIndicesLeaf( pf, neighbour);
					}
				}
			}
		}
	}
	AddClosedSetLeaf( pf, map_index);
}

void ZeroPathfinderCounters(Pathfinder *pf)
{
	pf->open_set_write_head = 0;
	pf->open_set_root_index = 0;
	pf->open_set_count = 0;

	pf->open_set_map_indices_write_head = 0;
	pf->open_set_map_indices_root_index = 0;
	pf->open_set_map_indices_count = 0;

	pf->closed_set_write_head = 0;
	pf->closed_set_root_index = 0;
	pf->closed_set_count = 0;

	pf->number_of_nodes_that_were_in_open_set_debug = 0;
}

/*
uint64_t starting1 = SDL_GetPerformanceCounter();
uint64_t stopping1 = SDL_GetPerformanceCounter();
double calculating1 = (double)(stopping1 - starting1) / (double)perf_freq;
printf("This took %.04g\n", calculating1*1000.0);
*/

// way to show all the hexes that are reachable with currently available movement points
// 1. start from start node
// 2. add nodes to open set and analyze
// 3. if accumulated_g_score is higher than available movement points, throw node in closed set
// 4. continue until open set is empty
// 5. return value all the nodes that were in open set (this could be array of int32_t and its size)

int32_t FindReachableNodes(Pathfinder *pf, int32_t start, int available_movement_points, int faction)
{
	int32_t reachable_nodes_count = 0;

	ZeroPathfinderCounters(pf);

	int32_t g_score = 0;
	float f_score = 0.0f;

	AddOpenSetLeaf( pf, f_score, g_score, start, -1 );
	AddOpenSetMapIndicesLeaf( pf, start);

	while (!OpenSetIsEmpty(pf))
	{
		int32_t map_index;
		int32_t accumulated_g_score;
		int32_t came_along_edge;

		PullMapIndexWithLowestFScoreFromOpenSet( pf, &map_index, &accumulated_g_score, &came_along_edge);
		came_along_edges[map_index] = came_along_edge;
		AnalyzeMapNodeForReachableNodes( pf, map_index, accumulated_g_score, available_movement_points, faction);
	}

	int rn = 0;
	for (int i = 0; i < pf->number_of_nodes_that_were_in_open_set_debug; i++)
	{
		if ( map_nodes[ pf->nodes_that_were_in_open_set_debug[i] ].occupier == -1 )
		{
			reachable_nodes[rn] = pf->nodes_that_were_in_open_set_debug[i];
			rn++;
		}
	}
	reachable_nodes_number = rn;

	//return reachable_nodes_count;
	return rn;
}

bool HexIsInReachableNodes(int32_t hex)
{
	for (int i = 0; i < reachable_nodes_number; i++)
	{
		if ( hex == reachable_nodes[i])
		{
			return true;
		}
	}

	return false;
}

uint32_t ReconstructPath(Pathfinder *pf, int32_t start, int32_t goal)
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

int32_t FindPath(Pathfinder *pf, int32_t start, int32_t goal, int faction)
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
	if (map_nodes[start].pathfind_island_id != map_nodes[goal].pathfind_island_id)
	{
		return -4;
	}

	uint64_t initiating_pathfinding = SDL_GetPerformanceCounter();

	ZeroPathfinderCounters(pf);

	ivec2 start_hex = { map_nodes[start].x, map_nodes[start].y };
	ivec2 goal_hex = { map_nodes[goal].x, map_nodes[goal].y };

	int32_t h_score = CalculateHexDistance(start_hex, goal_hex);
	int32_t g_score = 0;
	float f_score = (float)g_score + pf->pathfind_weight_h * (float) h_score;

	bool goal_found = false;
	int32_t temp_score = 1337;

	AddOpenSetLeaf( pf, f_score, g_score, start, -1 );
	AddOpenSetMapIndicesLeaf( pf, start);

	while (!goal_found && !OpenSetIsEmpty(pf))
	{
		int32_t map_index;
		int32_t accumulated_g_score;
		int32_t came_along_edge;
		PullMapIndexWithLowestFScoreFromOpenSet(pf, &map_index, &accumulated_g_score, &came_along_edge);

		came_along_edges[map_index] = came_along_edge;
		if ( map_index == goal )
		{
			goal_found = true;
			path_edges_size = ReconstructPath(pf, start, goal);
			temp_score = accumulated_g_score;
			break;
		}
		AnalyzeMapNode(pf, map_index, accumulated_g_score, goal_hex, faction);
	}

	analyzed_nodes_number = 0;
	for (int i = 0; i < pf->number_of_nodes_that_were_in_open_set_debug; i++)
	{
		analyzed_nodes[i] = pf->nodes_that_were_in_open_set_debug[i];
		analyzed_nodes_number++;
	}

	uint64_t pathfinding_completed = SDL_GetPerformanceCounter();
	double pathfinding_time = (double)(pathfinding_completed - initiating_pathfinding) / (double)perf_freq;
	snprintf(dumb_debug_string, 256, "Path found in %.04g milliseconds.\n", pathfinding_time*1000.0);

	return temp_score;
}

void ClearPaths(Pathfinder *pf)
{
	ZeroPathfinderCounters(pf);
	path_edges_size = -0;
	draw_path = false;
	reachable_nodes_number = 0;
	analyzed_nodes_number = 0;
}

void InitPathfinder(Pathfinder *pf)
{
	pf->pathfind_weight_h = 0.99f;
	pf->pathfind_weight_g = 1.0f;

	ZeroPathfinderCounters(pf);

	pf->open_set = (ThickLeaf*)malloc( sizeof(ThickLeaf) * OPEN_SET_MAX_SIZE );
	pf->open_set_map_indices = (SlimLeaf*)malloc( sizeof(SlimLeaf) * OPEN_SET_MAX_SIZE );
	pf->closed_set = (SlimLeaf*)malloc( sizeof(SlimLeaf) * OPEN_SET_MAX_SIZE );
	// pf->came_along_edges = (int32_t*)malloc( sizeof(int32_t) * OPEN_SET_MAX_SIZE );
	pf->nodes_that_were_in_open_set_debug = (int32_t*)malloc(sizeof(int32_t) * OPEN_SET_MAX_SIZE);
}

void ShutdownPathfinder(Pathfinder *pf)
{
	free(pf->open_set);
	free(pf->open_set_map_indices);
	free(pf->closed_set);
	free(pf->nodes_that_were_in_open_set_debug);
}
