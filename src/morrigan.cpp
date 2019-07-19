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

void MoveToNextFaction();

void FactionTakesItsTurn(int faction)
{	
	int faction_army_count = 0;

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if ( all_armies[i].faction == faction && all_armies[i].dead == false && all_armies[i].active == true )
		{
			faction_army_count++;
		}
	}

	if ( faction_army_count < 1)
	{
		MoveToNextFaction();
		// endless loop if all the armies are dead :)
	}

	strncpy(active_faction_str, faction_names[active_faction], 32);

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if (all_armies[i].faction == active_faction && all_armies[i].dead == false)
		{
			all_armies[i].move_done = false;
			all_armies[i].action_done = false;
		}
	}
}

void MoveToNextFaction()
{
	active_faction++;

	if (active_faction > FACTION_COUNT)
	{
		active_faction = 0;
		game_turn++;
	}
	FactionTakesItsTurn(active_faction);
}

void EndTurn()
{
	selected_army = NULL;

	MoveToNextFaction();
}

void DestroyArmy(Army *a)
{

	a->dead = true;

	unit_data_buffer_needs_update = true;

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
