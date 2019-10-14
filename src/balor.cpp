// Balor decides what enemy-controlled units do
// (ie. it's the AI)

// void BalorHandlesTheTurn(int faction)
void BalorTakesControl()
{
/*
	for (active_units)
	{
		if (units_health is low)
		{
			rest if no enemies nearby
			otherwise run away
		}

		if enemy units close by or reachable
		check if they have low health

		if enemies only farther away
		move towards them

		if enemies far away and friends nearby with low health wait for friends to rest up

		screening low health friends from enemies is too advanced!
		coordinated pushes to make traps for enemies or clearing way for friends is way too advanced!
	}
*/
}

int32_t GetArmyForBalorToPlayWith()
{
	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if ( all_armies[i].faction == active_faction && all_armies[i].dead == false && all_armies[i].active == true && all_armies[i].move_done == false )
		{
			return i;
		}
	}
	return -1;
}

int32_t ReachableNodeWithEnemyInItsNeighbouringNode(int count)
{
	for (int i = 0; i < count; i++)
	{
		if ( CheckForEnemyZonesOfControl( reachable_nodes[i], active_faction ) )
		{
			return reachable_nodes[i]; // just goes to first possible node with enemy next to it
		}
	}

	return -1; // didn't find anything
}

int32_t ShouldIAttackMaybe(int32_t own_hex, int own_faction)
{
	// -1, no
	// anything else, this hex number
	for (int i = 0; i < 6; i++)
	{
		if ( map_nodes[own_hex].edge[i] != -1 )
		{
			int32_t edge = map_nodes[own_hex].edge[i];

			if ( map_nodes[ map_edges[edge].end_node_index ].occupier != -1 )
			{
				int32_t occupier = map_nodes[ map_edges[edge].end_node_index ].occupier;

				if ( all_armies[occupier].faction != own_faction )
				{
					return map_nodes[ map_edges[edge].end_node_index ].index;
				}
			}
		}
	}

	return -1;
}

void RequestOrdersFromBalor()
{
	ClearPaths(pathfinder);

	int32_t balors_army = GetArmyForBalorToPlayWith();

	if ( balors_current_army != balors_army && balors_current_army != -1 )
	{
		all_armies[balors_current_army].move_done = true;
		all_armies[balors_current_army].action_done = true;
	}

	balors_current_army = balors_army;

	if ( balors_army >= 0)
	{
		int reachables_count = FindReachableNodes(pathfinder, all_armies[balors_army].position_hex, all_armies[balors_army].movement, all_armies[balors_army].faction);

		int destination = ReachableNodeWithEnemyInItsNeighbouringNode(reachables_count);

		if (destination < 0)
		{
			destination = reachable_nodes[ MWC % reachables_count ];
		}

		pathfinder->current_path.x = all_armies[balors_army].position_hex;
		pathfinder->current_path.y = destination;

		FindPath(pathfinder, all_armies[balors_army].position_hex, map_nodes[destination].index, all_armies[balors_army].faction);

		BeginArmyMoveAnimation(balors_army, all_armies[balors_army].position_hex, map_nodes[destination].index);
		PlaySfx(SFX_UNIT_MOVE);
		unit_data_buffer_needs_update = true;
		pathfinder->path_edges_size = 0;
		draw_path = false;
	}
}

/*

if units in balors faction with free move remain, request decision from balor
if no units with free move remain, balor ends the turn

void RequestOrdersFromBalor()
{
	
}

bool carrying_out_balors_orders = false;

void CarryOutBalorsOrders()
void AdvanceBalorsOrders()
{
	
}

if (balors_turn)
{
	if (carrying_out_balors_orders)
	{
		AdavanceBalorsOrders()
	}
	else
	{
		RequestOrdersFromBalor()
	}
}

*/
