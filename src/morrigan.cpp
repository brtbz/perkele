/*
The Morrigan determine the fate of soldiers in battle.
*/

typedef struct CombatTableEntry
{
	int32_t strength_minus_armor;
	float wound_chance;
	float kill_chance;
} CombatTableEntry;

CombatTableEntry combat_table[41] = 
{
	-20, 0.05f,  0.01f,
	-19, 0.06f,  0.012f,
	-18, 0.07f,  0.014f,
	-17, 0.08f,  0.016f,
	-16, 0.09f,  0.018f ,
	-15, 0.1f,   0.02f,
	-14, 0.11f,  0.022f,
	-13, 0.12f,  0.024f,
	-12, 0.13f,  0.026f,
	-11, 0.14f,  0.028f,
	-10, 0.15f,  0.03f,
	-9,  0.16f,  0.032f,
	-8,  0.17f,  0.034f,
	-7,  0.18f,  0.036f,
	-6,  0.19f,  0.038f,
	-5,  0.2f,   0.04f,
	-4,  0.21f,  0.042f,
	-3,  0.22f,  0.044f,
	-2,  0.23f,  0.046f,
	-1,  0.24f,  0.048f,
	 0,  0.25f,  0.05f,
	 1,  0.275f, 0.055f,
	 2,  0.3f,   0.06f,
	 3,  0.325f, 0.065f,
	 4,  0.35f,  0.07f,
	 5,  0.375f, 0.075f,
	 6,  0.4f,   0.08f,
	 7,  0.425f, 0.085f,
	 8,  0.45f,  0.09f,
	 9,  0.475f, 0.095f,
	 10, 0.5f,   0.1f,
	 11, 0.525f, 0.105f,
	 12, 0.55f,  0.11f,
	 13, 0.575f, 0.115f,
	 14, 0.6f,   0.12f,
	 15, 0.625f, 0.125f,
	 16, 0.65f,  0.13f,
	 17, 0.675f, 0.135f,
	 18, 0.7f,   0.14f,
	 19, 0.725f, 0.145f,
	 20, 0.75f,  0.15f,
};



int faction_count = FACTION_COUNT;
// int active_faction = 0;

const char* faction_names[] = 
{
	"REDS",
	"GREENS",
	"BLUES",
	"GOBLINS",
	"SURFERS",
};

void PrepareFactionForItsTurn(int faction)
{
	strncpy(active_faction_str, faction_names[faction], 32);

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if (all_armies[i].faction == faction && all_armies[i].dead == false)
		{
			all_armies[i].move_done = false;
			all_armies[i].action_done = false;
		}
	}

	if (faction == FACTION_GOBLINS)
	{
		balors_turn = true;
		BalorTakesControl();
	}
	else
	{
		balors_turn = false;
	}
}

bool FactionHasActiveUnits(int faction)
{
	int faction_army_count = 0;

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if ( all_armies[i].faction == faction && all_armies[i].dead == false && all_armies[i].active == true )
		{
			faction_army_count++;
		}
	}

	if (faction_army_count > 0)
	{
		return true;
	}
	return false;
}

void SwitchToNextFaction()
{
	active_faction++;
	while ( !FactionHasActiveUnits(active_faction) )
	{
		active_faction++;
		if (active_faction >= faction_count)
		{
			active_faction = 0;
		}
		// possible endless loop !!!
	}
	PrepareFactionForItsTurn(active_faction);
}

void EndTurn()
{
	selected_army = NULL;

	SwitchToNextFaction();
}

void DestroyArmy(Army *a)
{
	PlaySfx(SFX_UNIT_DEATH);

	a->dead = true;

	unit_data_buffer_needs_update = true;
}

int PushArmy(Army *pusher, Army *pushee)
{
	int32_t pushers_hex = pusher->position_hex;
	int32_t pushees_hex = pushee->position_hex;
	int32_t destination_hex = -1;
	int32_t maybe_hex = -1;
	int32_t maybe_hex2 = -1;
	int32_t maybe_edge = -1;
	int32_t maybe_edge2 = -1;
	int32_t direction1 = -1;
	int32_t direction2 = -1;

	int direction = -1;
	// 0 n, 1 ne, 2 se, 3 s, 4 sw, 5 nw

	for (int i = 0; i < 6; i++)
	{
		if ( map_edges[ map_nodes[pushers_hex].edge[i] ].end_node_index == pushees_hex)
		{
			direction = i;
			// hexes are neighbours, so push is possible
			// (should the push be possible from further away?)
		}
	}

	if (direction == -1)
	{
		return -1; // push failed
	}

	// first attempt straight push
	maybe_edge = map_edges[ map_nodes[ pushees_hex ].edge[ direction ] ].index;
	if ( maybe_edge != -1 )
	{
		maybe_hex = map_edges[ map_nodes[ pushees_hex ].edge[ direction ] ].end_node_index;
		if ( HexIsValidAndPassable(maybe_hex) )
		{
			if (HexIsFree( maybe_hex ))
			{
				// get pushed here
				destination_hex = maybe_hex;

				MoveArmyToNewHex(pushee->index, destination_hex);
				unit_data_buffer_needs_update = true;

				return 0;
			}
		}
	}

	for ( int i = 0; i < 2; i++)
	{
		// second attempt is 60 degrees left or right from the straight push direction
		// third attempt is 120 degrees left or right from the straight push direction
		direction1 = direction + ( i + 1);
		direction2 = direction - ( i + 1);
		if (direction1 < 0) { direction1 += 6; }
		if (direction2 < 0) { direction2 += 6; }
		if (direction1 > 5) { direction1 -= 6; }
		if (direction2 > 5) { direction2 -= 6; }

		maybe_edge = map_edges[ map_nodes[ pushees_hex ].edge[ direction1 ] ].index;
		maybe_edge2 = map_edges[ map_nodes[ pushees_hex ].edge[ direction2 ] ].index;

		if ( maybe_edge != -1 )
		{
			maybe_hex = map_edges[ map_nodes[ pushees_hex ].edge[ direction1 ] ].end_node_index;
			if ( HexIsValidAndPassable(maybe_hex))
			{
				if ( !HexIsFree( maybe_hex ))
				{
					maybe_hex = -2;
				}
			}
		}

		if ( maybe_edge2 != -1 )
		{
			maybe_hex2 = map_edges[ map_nodes[ pushees_hex ].edge[ direction2 ] ].end_node_index;
			if ( HexIsValidAndPassable(maybe_hex2))
			{
				if ( !HexIsFree( maybe_hex2 ))
				{
					maybe_hex2 = -2;
				}
			}
		}

		if ( HexIsValidAndPassable(maybe_hex) && !HexIsValidAndPassable(maybe_hex2) )
		{
			destination_hex = maybe_hex;
		}
		else if ( HexIsValidAndPassable(maybe_hex2) && !HexIsValidAndPassable(maybe_hex) )
		{
			destination_hex = maybe_hex2;
		}
		else if ( HexIsValidAndPassable(maybe_hex) && HexIsValidAndPassable(maybe_hex2) )
		{
			int random_roll = MWC % 100;
			if (random_roll < 50)
			{
				destination_hex = maybe_hex;
			}
			else
			{
				destination_hex = maybe_hex2;
			}
		}

		if ( HexIsValidAndPassable(destination_hex) )
		{
			MoveArmyToNewHex(pushee->index, destination_hex);
			unit_data_buffer_needs_update = true;

			return 0;	
		}

	}

	// if army can't run away, it surrenders
	DestroyArmy(pushee);

	return 0;

	// prefered push edge is map_edges[ map_nodes[ pushees_hex ].edge[ direction ] ]
	// secondary map_edges[ map_nodes [pushees_hex].edge[direction +/- 1] ] // direction should stay in range 0-5 and loop over instead of being clamped
	// if both available, choose one at random
	// tertiary map_edges[ map_nodes [pushees_hex].edge[direction +/- 2] ]
	// if none are available, kill the army
}

void ResolveCombat(Army *attacker, Army *defender)
{
	int attackers_attack_minus_defenders_armor = attacker->strength - defender->armor;
	attackers_attack_minus_defenders_armor = ClampValueToRange(attackers_attack_minus_defenders_armor, -20, 20);

	int defenders_attack_minus_attackers_armor = defender->strength - attacker->armor;
	defenders_attack_minus_attackers_armor = ClampValueToRange(defenders_attack_minus_attackers_armor, -20, 20);

	float attackers_wounding_chance = 0.0f;
	float attackers_killing_chance = 0.0f;

	float defenders_wounding_chance = 0.0f;
	float defenders_killing_chance = 0.0f;

	for (int i = 0; i < 41; i++)
	{
		if ( combat_table[i].strength_minus_armor == attackers_attack_minus_defenders_armor )
		{
			attackers_wounding_chance = combat_table[i].wound_chance;
			attackers_killing_chance = combat_table[i].kill_chance;
		}

		if ( combat_table[i].strength_minus_armor == defenders_attack_minus_attackers_armor )
		{
			defenders_wounding_chance = combat_table[i].wound_chance;
			defenders_killing_chance = combat_table[i].kill_chance;
		}
	}

	int attacker_attacks_left = attacker->hits_current;
	int defender_attacks_left = defender->hits_current;
	int combat_rounds = attacker_attacks_left;
	if (defender_attacks_left > attacker_attacks_left)
	{
		combat_rounds = defender_attacks_left;
	}

	// THIS FIGHT
	int attacker_dead = 0;
	int attacker_wounded = 0;
	int defender_dead = 0;
	int defender_wounded = 0;

	for (int i = 0; i < combat_rounds; i++)
	{
		if (defender_attacks_left > 0)
		{
			// DEFENDER ATTACKS!
			float random_roll = (float)(MWC % 100000) / 100000.f;
			if ( random_roll < defenders_killing_chance )
			{
				attacker->dead_soldiers++;
				attacker->hits_current--;
				attacker_dead++;
			}
			else if ( random_roll < defenders_wounding_chance)
			{
				attacker->wounded_soldiers++;
				attacker->hits_current--;
				attacker_wounded++;
			}

			defender_attacks_left--;
		}

		if (attacker_attacks_left > 0)
		{
			// ATTACKER ATTACKS
			float random_roll = (float)(MWC % 100000) / 100000.f;
			if ( random_roll < attackers_killing_chance )
			{
				defender->dead_soldiers++;
				defender->hits_current--;
				defender_dead++;
			}
			else if ( random_roll < attackers_wounding_chance)
			{
				defender->wounded_soldiers++;
				defender->hits_current--;
				defender_wounded++;
			}

			attacker_attacks_left--;
		}
	}

	snprintf(&combat_result_str1[0], 256, "Attacker: %s Dead: %d Wounded: %d", attacker->name, attacker_dead, attacker_wounded);
	snprintf(&combat_result_str2[0], 256, "Defender: %s Dead: %d Wounded: %d", defender->name, defender_dead, defender_wounded);

	PushArmy(attacker, defender);

	if (attacker->hits_current <= 0)
	{
		DestroyArmy(attacker);
	}
	if (defender->hits_current <= 0)
	{
		DestroyArmy(defender);
	}

	attacker->action_done = true;
}
