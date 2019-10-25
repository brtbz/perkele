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
	if ( balors_current_army != -1 && all_armies[balors_current_army].dead == false)
	{
		if (all_armies[balors_current_army].move_done == false || all_armies[balors_current_army].action_done == false)
		{
			return balors_current_army;
		}
	}

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

int32_t FreeHexNextToClosestEnemy()
{
	int32_t own_hex = all_armies[balors_current_army].position_hex;
	int32_t closest_enemy = -1;
	int32_t distance = 1024;

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if ( all_armies[i].faction != active_faction && all_armies[i].dead == false && all_armies[i].active == true)
		{
			ivec2 own_hex_v2 = { map_nodes[own_hex].x, map_nodes[own_hex].y };
			ivec2 enemy_hex_v2 = { map_nodes[ all_armies[i].position_hex ].x, map_nodes[ all_armies[i].position_hex ].y };
			int32_t dist = CalculateHexDistance(own_hex_v2, enemy_hex_v2);
			if (dist < distance)
			{
				closest_enemy = i;
				distance = dist;
			}
		}
	}

	if (closest_enemy == -1)
	{
		return -1;
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			if (map_nodes[ all_armies[closest_enemy].position_hex ].edge[i] != -1)
			{
				int32_t edge = map_nodes[ all_armies[closest_enemy].position_hex ].edge[i];

				if ( HexIsFree(map_edges[edge].end_node_index) && HexIsValidAndPassable(map_edges[edge].end_node_index) )
				{
					return map_edges[edge].end_node_index;
				}
			}
		}
	}

	return -1;
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

bool ShouldIRestMaybe()
{
	if ( all_armies[balors_current_army].move_done == true || all_armies[balors_current_army].action_done == true  )
	{
		return false;
	}

	if (CheckForEnemyZonesOfControl(all_armies[balors_current_army].position_hex, active_faction))
	{
		return false;
	}

	if ( all_armies[balors_current_army].wounded_soldiers == 0 )
	{
		return false;
	}

	// add some randomization here

	return true;
}

int32_t MoveTowardsEnemyMaybe()
{

}

void RequestOrdersFromBalor()
{
	ClearPaths(pathfinder);

	int32_t balors_army = GetArmyForBalorToPlayWith();

	if (balors_army == -1)
	{
		EndTurn();
	}
	else
	{
		if ( balors_current_army != balors_army && balors_current_army != -1 )
		{
			all_armies[balors_current_army].move_done = true;
			all_armies[balors_current_army].action_done = true;
		}

		balors_current_army = balors_army;

		if ( balors_army >= 0)
		{
			bool rest_maybe = ShouldIRestMaybe();

			if (rest_maybe)
			{
				RestArmy( &all_armies[balors_army] );
			}
			else
			{
				int32_t attack_maybe = ShouldIAttackMaybe( all_armies[balors_army].position_hex, active_faction );

				if (attack_maybe != -1 && all_armies[balors_army].action_done == false)
				{
					// attack
					defenders_hex = attack_maybe; // defenders_hex is used by the animation

					BeginArmyAttackAnimation( all_armies[balors_army].index, all_armies[balors_army].position_hex, defenders_hex );
					ResolveCombat( &all_armies[balors_army], &all_armies[map_nodes[attack_maybe].occupier] );
					PlaySfx( all_armies[balors_army].attack_sound );
					unit_data_buffer_needs_update = true;
					pathfinder->path_edges_size = 0;
					draw_path = false;
					all_armies[balors_army].action_done = true;
				}
				else if (all_armies[balors_army].move_done == false)
				{
					int reachables_count = FindReachableNodes(pathfinder, all_armies[balors_army].position_hex, all_armies[balors_army].movement, all_armies[balors_army].faction);
					if ( reachables_count > 0)
					{
						int destination = ReachableNodeWithEnemyInItsNeighbouringNode(reachables_count);

						bool dest_in_reachables = true;

						if (destination < 0)
						{
							int32_t free_hex_next_closest_enemy = FreeHexNextToClosestEnemy();

							if (free_hex_next_closest_enemy == -1)
							{
								destination = reachable_nodes[ MWC % reachables_count ];
							}
							else
							{
								destination = free_hex_next_closest_enemy;
								dest_in_reachables = false;
								// 
							}
						}

						pathfinder->current_path.x = all_armies[balors_army].position_hex;
						pathfinder->current_path.y = destination;

						FindPath(pathfinder, all_armies[balors_army].position_hex, map_nodes[destination].index, all_armies[balors_army].faction);

						if (dest_in_reachables == false)
						{
							allowed_steps = AllowedStepsWithMovementPoints( all_armies[balors_army].movement );
						}

						BeginArmyMoveAnimation(balors_army, all_armies[balors_army].position_hex, map_nodes[destination].index);
						PlaySfx(SFX_UNIT_MOVE);
						unit_data_buffer_needs_update = true;
						pathfinder->path_edges_size = 0;
						draw_path = false;
					}
					all_armies[balors_army].move_done = true;
				}
				else
				{
					all_armies[balors_army].action_done = true;
				}
			}
		}
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
