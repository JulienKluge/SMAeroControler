/*
**
** Aero Jail Controler Plugin - Commands partial File
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
** little note:
** ReplyToCommand is only used, when Admincommands are handled
** because public messages are prepared (add Tag, colorize etc.)
*/

public Action:Cmd_Refuse(client, args)
{
	if (!InWork) { return Plugin_Handled; }
	new teamIndex = GetClientTeam(client);
	if (teamIndex == CS_TEAM_T)
	{
		if (IsPlayerAlive(client))
		{
			if (tAliveCount > 1)
			{
				if (c_r[client] > 0)
				{
					if (!c_cReal[client])
					{
						c_r[client]--;
						if (c_rBaseMax[client] > 1)
						{ AC_PrintToChatAll("%t", "refusing_left", client, c_r[client]); }
						else
						{ AC_PrintToChatAll("%t", "refusing", client); }
						c_rTimeStamp[client] = GetGameTime();
						StateNotifyCounter = 35;
						UpdateClientColor(client);
					}
					else
					{ AC_PrintToChat(client, "%t", "refusing_denied_capitulated"); }
				}
				else
				{ AC_PrintToChat(client, "%t", "refusing_denied_count"); }
			}
		}
		else
		{ AC_PrintToChat(client, "%t", "denied_not_alive"); }
	}
	else
	{ AC_PrintToChat(client, "%t", "denied_not_t"); }
	return Plugin_Handled;
}

public Action:Cmd_Capitulate(client, args)
{
	if (!InWork) { return Plugin_Handled; }
	new teamIndex = GetClientTeam(client);
	if (teamIndex == CS_TEAM_T)
	{
		if (IsPlayerAlive(client))
		{
			if (tAliveCount > 1)
			{
				if (c_c[client] > 0)
				{
					c_c[client]--;
					c_cReal[client] = true;
					c_topC[client] = true;
					RemoveAllClientItems(client);
					if (c_topCDisableTimer[client] != INVALID_HANDLE)
					{
						CloseHandle(c_topCDisableTimer[client]);
						c_topCDisableTimer[client] = INVALID_HANDLE; //just because beautiful code
					}
					new Handle:pack;
					c_topCDisableTimer[client] = CreateDataTimer(30.0, timer_disableTopC, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
					WritePackCell(pack, client);
					WritePackCell(pack, GetClientUserId(client));
					if (c_cBaseMax[client] > 1)
					{
						AC_PrintToChatAll("%t", "capitulating_left", client, c_c[client]);
						PrintHintTextToAll("%t", "capitulating_plain", client);
						PrintCenterTextAll("%t", "capitulating_plain", client);
					}
					else
					{
						AC_PrintToChatAll("%t", "capitulating", client);
						PrintHintTextToAll("%t", "capitulating_plain", client);
						PrintCenterTextAll("%t", "capitulating_plain", client);
					}
					StateNotifyCounter = 2;
					UpdateClientColor(client, false);
				}
				else
				{ AC_PrintToChat(client, "%t", "capitulating_denied_count"); }
			}
		}
		else
		{ AC_PrintToChat(client, "%t", "denied_not_alive"); }
	}
	else if (teamIndex == CS_TEAM_CT)
	{
		ForcePlayerSuicide(client);
		AC_PrintToChatAll("%t", "capitulating_ct", client);
	}
	return Plugin_Handled;
}

public Action:Cmd_Kill(client, args)
{
	if (!InWork) { return Plugin_Handled; }
	if (IsPlayerAlive(client))
	{
		RemoveAllClientItems(client);
		ForcePlayerSuicide(client);
	}
	else
	{ AC_PrintToChat(client, "%t", "denied_not_alive"); }
	return Plugin_Handled;
}

public Action:Cmd_SetColors(client, args)
{
	if (!InWork) { return Plugin_Handled; }
	if (GetClientTeam(client) == CS_TEAM_CT)
	{
		if (IsPlayerAlive(client))
		{
			for (new i = 1; i <= MaxClients; i++)
			{
				if (IsClientValid(i))
				{
					if (IsPlayerAlive(i) && GetClientTeam(i) == CS_TEAM_T)
					{
						UpdateClientColor(i);
					}
				}
			}
			AC_PrintToChatAll("%t", "setcolors_notify", client);
		}
		else
		{ AC_PrintToChat(client, "%t", "denied_not_alive"); }
	}
	else
	{ AC_PrintToChat(client, "%t", "denied_not_ct"); }
	return Plugin_Handled;
}

public Action:Cmd_DeleteColors(client, args)
{
	if (!InWork) { return Plugin_Handled; }
	if (GetClientTeam(client) == CS_TEAM_CT)
	{
		if (IsPlayerAlive(client))
		{
			for (new i = 1; i <= MaxClients; i++)
			{
				if (IsClientValid(i))
				{
					if (IsPlayerAlive(i) && GetClientTeam(i) == CS_TEAM_T)
					{
						SetEntityRenderColor(i, 255, 255, 255, GetClientAlphaRenderColor(i));
					}
				}
			}
			AC_PrintToChatAll("%t", "deletecolors_notify", client);
		}
		else
		{ AC_PrintToChat(client, "%t", "denied_not_alive"); }
	}
	else
	{ AC_PrintToChat(client, "%t", "denied_not_ct"); }
	return Plugin_Handled;
}

public Action:Cmd_Noblock(client, args)
{
	if (!InWork) { return Plugin_Handled; }
	if (RoundStartedState > 1)
	{
		if (GetClientTeam(client) == CS_TEAM_CT)
		{
			if (IsPlayerAlive(client))
			{
				for (new i = 1; i <= MaxClients; i++)
				{
					if (IsClientValid(i))
					{
						if (IsPlayerAlive(i))
						{
							SetEntData(i, g_Offset_CollisionGroup, COLLISION_GROUP_DEBRIS_TRIGGER, 4, true);
						}
					}
				}
				AC_PrintToChatAll("%t", "noblock_notify", client);
			}
			else
			{ AC_PrintToChat(client, "%t", "denied_not_alive"); }
		}
		else
		{ AC_PrintToChat(client, "%t", "denied_not_ct"); }
	}
	else
	{ AC_PrintToChat(client, "%t", "denied_option_disabled"); }
	return Plugin_Handled;
}

public Action:Cmd_Block(client, args)
{
	if (!InWork) { return Plugin_Handled; }
	if (RoundStartedState > 1)
	{
		if (GetClientTeam(client) == CS_TEAM_CT)
		{
			if (IsPlayerAlive(client))
			{
				for (new i = 1; i <= MaxClients; i++)
				{
					if (IsClientValid(i))
					{
						if (IsPlayerAlive(i))
						{
							SetEntData(i, g_Offset_CollisionGroup, COLLISION_GROUP_PLAYER, 4, true);
						}
					}
				}
				AC_PrintToChat(client, "%t", "block_notify", client);
			}
			else
			{ AC_PrintToChat(client, "%t", "denied_not_alive"); }
		}
		else
		{ AC_PrintToChat(client, "%t", "denied_not_ct"); }
	}
	else
	{ AC_PrintToChat(client, "%t", "denied_option_disabled"); }
	return Plugin_Handled;
}

public Action:Cmd_Drop(client, args)
{
	if (c_IsDropDenied[client]) { return Plugin_Handled; }
	if (DropAll_Active)
	{
		new weaponIndex = GetEntPropEnt(client, Prop_Send, "m_hActiveWeapon");
		if (IsValidEdict(weaponIndex))
		{ CS_DropWeapon(client, weaponIndex, true, false); }
		return Plugin_Handled;
	}
	return Plugin_Continue;
}

public Action:Cmd_About(client, args)
{
	DisplayMenu(aboutMenu, client, 60);
	return Plugin_Handled;
}

public Action:Cmd_Commands(client, args)
{
	CreateAndDisplayCmdMenu(client);
	return Plugin_Handled;
}

public Action:Cmd_Rules(client, args)
{
	DisplayMenu(ruleMenu, client, 60);
	return Plugin_Handled;
}

/*
**
** Listener
**
*/

public Action:Listener_Drop(client, const String:command[], argc)
{
	if (c_IsDropDenied[client]) { return Plugin_Handled; }
	if (DropAll_Active)
	{
		new weaponIndex = GetEntPropEnt(client, Prop_Send, "m_hActiveWeapon");
		if (IsValidEdict(weaponIndex))
		{ CS_DropWeapon(client, weaponIndex, true, false); }
		return Plugin_Handled;
	}
	return Plugin_Continue;
}

public Action:Listener_ShouldStripped(client, const String:command[], argc)
{
	if (argc < 1) { return Plugin_Continue; }
	if (StrEqual(command, "sm_slay", false))
	{
		if (!CheckCommandAccess(client, "", ADMFLAG_SLAY, false)) { return Plugin_Continue; }
	}
	else if (StrEqual(command, "sm_kick", false))
	{
		if (!CheckCommandAccess(client, "sm_kick", ADMFLAG_KICK, false)) { return Plugin_Continue; }
	}
	else if (StrEqual(command, "sm_ban", false))
	{
		if (!CheckCommandAccess(client, "sm_ban", ADMFLAG_BAN, false)) { return Plugin_Continue; }
	}
	else
	{
		return Plugin_Continue;
	}
	decl String:targetString[32];
	GetCmdArg(1, targetString, sizeof(targetString));
	new bool:isMl = true;
	new targets[MAXPLAYERS + 1];
	new targetCount = ProcessTargetString(targetString, client, targets, sizeof(targets), COMMAND_FILTER_CONNECTED, "", _:0, bool:isMl);
	if (targetCount > 0)
	{
		for (new i = 0; i < targetCount; i++)
		{
			RemoveAllClientItems(targets[i]);
		}
	}
	return Plugin_Continue;
}

/*
**
** Listener
**
*/

/*
** 
** Admin Commands
** 
*/
//Well, it's funny that I write a little note that i only use ReplyToCommand on admincmds even not having any of them xD
/*
** 
** Admin Commands
** 
*/