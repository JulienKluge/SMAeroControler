/*
**
** Aero Jail Controler Plugin
** Store System - partial feature-stocks file
** Author: _AeonOne_
**
** binding license: GPLv3
** voluntary license:
** "THE BEER-WARE LICENSE" (Revision 43-1 by Julien Kluge):
** Julien Kluge wrote this file. As long as you retain this notice you
** can do what is defined in the binding license (GPLv3). If we meet some day, and you think
** this stuff is worth it, you can buy me a beer, pizza or something else which you think is appropriate.
** This license is a voluntary license. You don't have to observe it.
**
*/

stock void StoreFeature_DicePlus(int client)
{
	int diceCount;
	ac_getClientDiceVars(client, diceCount);
	ac_setClientDiceVars(client, diceCount + 1);
}

stock void StoreFeature_RefusePlus(int client)
{
	int refuseCount;
	int maxRefuseCount;
	ac_getClientRefuseVars(client, refuseCount, maxRefuseCount);
	ac_setClientRefuseVars(client, refuseCount + 1, maxRefuseCount + 1);
}

stock void StoreFeature_CapitulatePlus(int client)
{
	int capitulateCount;
	int maxCapitulateCount;
	ac_getClientCapitulateVars(client, capitulateCount, maxCapitulateCount);
	ac_setClientCapitulateVars(client, capitulateCount + 1, maxCapitulateCount + 1);
}

stock void StoreFeature_FiveDices(int client)
{
	for (int i = 0; i < 5; i++)
	{
		if (IsClientValid(client))
		{
			if (IsPlayerAlive(client))
			{
				ac_performDiceOnClient(client);
			}
		}
	}
}

stock void StoreFeature_DicePlusMap(int client)
{
	int diceCount;
	int diceBaseCount;
	ac_getClientDiceVars(client, diceCount, diceBaseCount);
	ac_setClientDiceVars(client, diceCount + 1, diceBaseCount + 1);
}

stock void StoreFeature_RefusePlusMap(int client)
{
	int refuseCount;
	int maxRefuseCount;
	int baseRefuseCount;
	ac_getClientRefuseVars(client, refuseCount, maxRefuseCount, baseRefuseCount);
	ac_setClientRefuseVars(client, refuseCount + 1, maxRefuseCount + 1, baseRefuseCount + 1);
}

stock void StoreFeature_CapitulatePlusMap(int client)
{
	int capitulateCount;
	int maxCapitulateCount;
	int baseCapitulateCount;
	ac_getClientCapitulateVars(client, capitulateCount, maxCapitulateCount, baseCapitulateCount);
	ac_setClientCapitulateVars(client, capitulateCount + 1, maxCapitulateCount + 1, baseCapitulateCount + 1);
}

stock void StoreFeature_LessGravity(int client)
{
	SetEntityGravity(client, 0.6 * GetEntityGravity(client));
}

stock void StoreFeature_MoreSpeed(int client)
{
	SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", GetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue") * 1.3);
}

stock void StoreFeature_MoreHP(int client)
{
	SetEntityHealth(client, GetClientHealth(client) + 50);
}

stock void StoreFeature_FiveSecInvisible(int client)
{
	c_InvisibleSeconds[client] += 5;
	if (c_InvisCountTimer[client] == INVALID_HANDLE)
	{
		SetEntityRenderMode(client, RENDER_NONE);
		Handle pack;
		c_InvisCountTimer[client] = CreateDataTimer(1.0, timer_InvisTimer, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
		WritePackCell(pack, client);
		WritePackCell(pack, GetClientUserId(client));
	}
}

stock void StoreFeature_TenSecInvisible(int client)
{
	c_InvisibleSeconds[client] += 10;
	if (c_InvisCountTimer[client] == INVALID_HANDLE)
	{
		SetEntityRenderMode(client, RENDER_NONE);
		Handle pack;
		c_InvisCountTimer[client] = CreateDataTimer(1.0, timer_InvisTimer, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
		WritePackCell(pack, client);
		WritePackCell(pack, GetClientUserId(client));
	}
}

stock void StoreFeature_Smoke(int client)
{
	GivePlayerItem(client, "weapon_smokegrenade");
}

stock void StoreFeature_Flash(int client)
{
	GivePlayerItem(client, "weapon_flashbang");
}

stock void StoreFeature_Armor(int client)
{
	int armorVal = GetEntProp(client, Prop_Data, "m_ArmorValue");
	int hasHelm = GetEntProp(client, Prop_Send, "m_bHasHelmet", 4);
	if (armorVal < 100) { SetEntProp(client, Prop_Send, "m_ArmorValue", 100); }
	if (hasHelm != 1) { SetEntProp(client, Prop_Send, "m_bHasHelmet", 1); }
}

stock void StoreFeature_NV(int client)
{
	if (g_offset_hasnv == -1) { g_offset_hasnv = FindSendPropOffs("CCSPlayer", "m_bHasNightVision"); }
	if (g_offset_nvon == -1) { g_offset_nvon = FindSendPropOffs("CCSPlayer", "m_bNightVisionOn"); }
	if (GetEntData(client, g_offset_hasnv, 1) == 1) { return; }
	SetEntData(client, g_offset_hasnv, true, 1, true);
	SetEntData(client, g_offset_nvon, true, 1, true);
}

stock void StoreFeature_PressureHE(int client)
{
	c_PressureNadeCount[client]++;
	int ent = GetPlayerWeaponSlot(client, CS_SLOT_GRENADE);
	if (ent != -1)
	{ RemoveEdict(ent); }
	GivePlayerItem(client, "weapon_hegrenade");
}

stock void StoreFeature_Respawn(int client)
{
	c_HasRespawn[client] = true;
}

stock void StoreFeature_MultiPointGain(int client)
{
	c_MultiplierPrivate[client] = 3.0;
}

stock void StoreFeature_EnemyPingPulse(int client)
{
	int teamIndex = GetClientTeam(client);
	float pos_one[3];
	float pos_two[3];
	GetClientAbsOrigin(client, pos_one);
	pos_one[2] += 35.0;
	int enemyTeamColor[4];
	if (teamIndex == CS_TEAM_T)
	{ enemyTeamColor = { 255, 32, 32, 255 }; }
	else
	{ enemyTeamColor = { 255, 255, 32, 32 }; }
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i) && i != client)
		{
			if (IsPlayerAlive(i) && teamIndex != GetClientTeam(i))
			{
				GetClientAbsOrigin(i, pos_two);
				pos_two[2] += 35.0;
				TE_SetupBeamPoints(pos_one, pos_two, g_BeamSprite, g_HaloSprite, 0, 10, 2.0, 2.0, 5.0, 0, 0.0, enemyTeamColor, 10);
				TE_SendToClient(client);
				TE_SetupBeamRingPoint(pos_two, 20.0, 200.0, g_BeamSprite, g_HaloSprite, 0, 10, 2.0, 5.0, 0.0, enemyTeamColor, 10, 0);
				TE_SendToClient(client);
				EmitAmbientSound("buttons/blip1.wav", pos_two, client, SNDLEVEL_RAIDSIREN);
			}
		}
	}
}

stock void StoreFeature_EnemyModel(int client)
{
	int teamIndex = GetClientTeam(client);
	int rand = GetRandomInt(1, 4);
	if (teamIndex == CS_TEAM_T)
	{
		switch (rand)
		{
			case 1: { SetEntityModel(client, "models/player/ct_gign.mdl"); }
			case 2: { SetEntityModel(client, "models/player/ct_gsg9.mdl"); }
			case 3: { SetEntityModel(client, "models/player/ct_sas.mdl"); }
			case 4: { SetEntityModel(client, "models/player/ct_urban.mdl"); }
		}
		int entIndex = -1;
		if ((entIndex = GetPlayerWeaponSlot(client, CS_SLOT_C4)) != -1)
		{ CS_DropWeapon(client, entIndex, true); }
	}
	else
	{
		switch (rand)
		{
			case 1: { SetEntityModel(client, "models/player/t_arctic.mdl"); }
			case 2: { SetEntityModel(client, "models/player/t_guerilla.mdl"); }
			case 3: { SetEntityModel(client, "models/player/t_leet.mdl"); }
			case 4: { SetEntityModel(client, "models/player/t_phoenix.mdl"); }
		}
	}
}