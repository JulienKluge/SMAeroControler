/*
**
** Aero Jail Controler Plugin - Events partial File
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

public Action Event_Pre_PlayerSpawn(Handle event, char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (IsClientValid(client))
	{
		/* Test in debug-version: multiple dicing, refusing, capitulating */
		#if defined DEBUG
		c_rBase[client] = 3;
		c_cBase[client] = 3;
		c_dicesBase[client] = 3;
		#endif
		int teamIndex = GetClientTeam(client);
		if (teamIndex > CS_TEAM_SPECTATOR)
		{
			RespawnClientVars(client);
			UpdatePlayerCount();
			if (!InWork) { return Plugin_Continue; }
			if (g_Offset_CollisionGroup != -1)
			{
				if (NoBlockState == 1 || NoBlockState == 3)
				{ SetEntData(client, g_Offset_CollisionGroup, COLLISION_GROUP_DEBRIS_TRIGGER, 4, true); }
				else
				{ SetEntData(client, g_Offset_CollisionGroup, COLLISION_GROUP_PLAYER, 4, true); }
			}
			RemoveAllClientItems(client);
			if (teamIndex == CS_TEAM_T)
			{
				if (tSpawnWeaponCount > 0)
				{
					for (int i = 0; i < tSpawnWeaponCount; i++)
					{ GivePlayerItem(client, tSpawnWeapons[i]); }
				}
				Handle pack;
				c_DiceMenu[client] = CreateDataTimer(1.5, timer_PrepareSpawn, pack, TIMER_FLAG_NO_MAPCHANGE);
				WritePackCell(pack, client);
				WritePackCell(pack, GetClientUserId(client));
			}
			else if (teamIndex == CS_TEAM_CT)
			{
				if (ctSpawnWeaponCount > 0)
				{
					for (int i = 0; i < ctSpawnWeaponCount; i++)
					{ GivePlayerItem(client, ctSpawnWeapons[i]); }
				}
			}
		}
		MuteSys_ClientSpawn_Detour(client);
		Call_StartForward(clientRoundReadyForward);
		Call_PushCell(client);
		Call_Finish();
	}
	return Plugin_Continue;
}

public Action Event_Post_PlayerSpawn(Handle event, char[] name, bool dontBroadcast)
{
	UpdatePlayerCount();
	return Plugin_Continue;
}

public Action Event_PlayerDeath(Handle event, char[] name, bool dontBroadcast)
{
	if (!InWork) { return Plugin_Continue; }
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	Diced_PlayerDeath_Detour(client);
	MuteSys_ClientDeath_Detour(client);
	UpdatePlayerCount();
	return Plugin_Continue;
}

public Action Event_RoundStart(Handle event, char[] name, bool dontBroadcast)
{
	if (!InWork) { return Plugin_Continue; }
	RoundStartedState = NoBlockState;
	StateNotifyCounter = 0;
	UpdatePlayerCount();
	MuteSys_RoundStart_Detour();
	return Plugin_Continue;
}

public Action Event_RoundEnd(Handle event, char[] name, bool dontBroadcast)
{
	Handle subhandle;
	for (int i = 0; i <= MaxClients; i++)
	{
		if (c_topCDisableTimer[i] != INVALID_HANDLE)
		{
			/* These kind of using a temporary 'subhandle' variable prevents that a handle bug, occure more than one time */
			subhandle = c_topCDisableTimer[i];
			c_topCDisableTimer[i] = INVALID_HANDLE;
			CloseHandle(subhandle);
		}
		if (c_DiceMenu[i] != INVALID_HANDLE)
		{
			subhandle = c_DiceMenu[i];
			c_DiceMenu[i] = INVALID_HANDLE;
			CloseHandle(subhandle);
		}
		if (c_dropHandler[i] != INVALID_HANDLE)
		{
			subhandle = c_dropHandler[i];
			c_dropHandler[i] = INVALID_HANDLE;
			CloseHandle(subhandle);
		}
	}
	Diced_EndRound_Detour();
	MuteSys_RoundEnd_Detour();
	UpdatePlayerCount();
	return Plugin_Continue;
}

public Action Event_PlayerJump(Handle event, char[] name, bool dontBroadcast)
{
	if (!InWork) { return Plugin_Continue; }
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	Diced_PlayerJump_Detour(client);
	return Plugin_Continue;
}

public Action Event_WeaponFire(Handle event, char[] name, bool dontBroadcast)
{
	if (!InWork) { return Plugin_Continue; }
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (IsClientValid(client)) //does always a client fire a weapon?
	{
		char weaponString[32];
		GetEventString(event, "weapon", weaponString, sizeof(weaponString));
		Diced_WeaponFire_Detour(client, weaponString);
		if (DropAll_Active)
		{
			if (StrEqual(weaponString, "flashbang") || StrEqual(weaponString, "smokegrenade") || StrEqual(weaponString, "hegrenade"))
			{
				c_IsDropDenied[client] = true;
				Handle pack;
				c_dropHandler[client] = CreateDataTimer(1.0, timer_ReAllowDrop, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
				WritePackCell(pack, client);
				WritePackCell(pack, GetClientUserId(client));
			}
		}
	}
	return Plugin_Continue;
}

public Action Event_PlayerHurt(Handle event, char[] name, bool dontBroadcast)
{
	if (!InWork) { return Plugin_Continue; }
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	int attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	if (IsClientValid(client) && IsClientValid(attacker))
	{
		if (!c_rebell[attacker])
		{
			if (GetClientTeam(client) == CS_TEAM_CT)
			{
				if (GetClientTeam(attacker) == CS_TEAM_T)
				{
					if (tAliveCount > 1)
					{
						c_rebell[attacker] = true;
						UpdateClientColor(attacker, false); //Override Alpha of an rebell - he/she wont deserve the right to be invisible
						AC_PrintToChatAll("%t", "became_rebell", attacker);
					}
				}
			}
		}
	}
	return Plugin_Continue;
}

public Action Event_ActivatePlayer(Handle event, char[] name, bool dontBroadcast)
{
	UpdatePlayerCount();
	return Plugin_Continue;
}