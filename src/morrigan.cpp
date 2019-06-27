/*
The Morrigan determine the fate of soldiers in battle.
*/

typedef enum Faction
{
	FACTION_RED,
	FACTION_BLUE,
	FACTION_GREEN,
	FACTION_COUNT
} Faction;

int faction_count = FACTION_COUNT;
int active_faction = 0;

void FactionTakesItsTurn(int faction)
{
	/*
	int faction_army_count = 0;

	for each army
		if ( army.faction == faction && army.dead == false)
			faction_army_count++

	if (faction_army_count == 0)
		MoveToNextFaction()
		// endless loop if all the armies are dead :)

	*/
}

void MoveToNextFaction()
{
	active_faction++;

	if (active_faction >= FACTION_COUNT)
	{
		active_faction = 0;
		game_turn++;
	}
	FactionTakesItsTurn(active_faction);
}

void EndTurn()
{
/*
	for each army
		army_has_moved = false
		army_has_attacked = false
		army.movement_points_cur = army.movement_point_max

	MoveToNextFaction();
*/
}

void DestroyArmy(Army *a)
{
/*
	a->dead = true

	unit_buffer_needs_update = true;
*/
}
