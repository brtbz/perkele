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
		if ( all_armies[i].faction == active_faction && all_armies[i].dead == false && all_armies[i].active == true && all_armies[i].move_done == false)
		{
			return i;
		}
	}
	return -1;
}

void RequestOrdersFromBalor()
{
	ClearPaths(pathfinder);
	bool orders_delivered = false;

	int32_t balors_army = GetArmyForBalorToPlayWith();

	if ( balors_army >= 0)
	{
		int reachables_count = FindReachableNodes(pathfinder, all_armies[balors_army].position_hex, all_armies[balors_army].movement, all_armies[balors_army].faction);

		int destination = reachable_nodes[ MWC % reachables_count ];

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
