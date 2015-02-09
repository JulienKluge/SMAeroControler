/*
**
** Aero Jail Controler Plugin
** WAR Base Control System - partial handler and callbacks file
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

public Action:timer_ReAllowVoting(Handle:timer)
{
	ReVoteTimer = INVALID_HANDLE;
	VoteIsOpen = true;
	AC_PrintToChatAll("%t", "war_vote_reopened");
	return Plugin_Stop;
}

public Action:timer_ForceWarEnd(Handle:timer)
{
	WarMaxTimer = INVALID_HANDLE;
	ForceEndWar();
	return Plugin_Stop;
}

public Action:timer_EndFreezeTime(Handle:timer)
{
	WarFTCount--;
	if (WarFTCount == 0)
	{
		WarFreezeTimer = INVALID_HANDLE;
		WarInFreezeTime = false;
		for (new i = 1; i <= MaxClients; i++)
		{
			if (IsClientValid(i))
			{
				if (IsPlayerAlive(i))
				{
					if (GetClientTeam(i) == CS_TEAM_T)
					{
						SetEntityMoveType(i, MOVETYPE_WALK);
						ApplyClientFade(i, 500, { 255, 255, 255, 0 }, true, false, true);
						GivePlayerItem(i, "weapon_knife");
					}
				}
			}
		}
		if (War_FT_End[WarIndex] != WarFreezeTimeEnd:INVALID_FUNCTION)
		{
			Call_StartFunction(War_Plugin[WarIndex], War_FT_End[WarIndex]);
			Call_Finish();
		}
		AC_PrintToChatAll("%t", "war_freeze_time_over");
		return Plugin_Stop;
	}
	else
	{
		for (new i = 1; i <= MaxClients; i++)
		{
			if (IsClientValid(i))
			{
				if (IsPlayerAlive(i))
				{
					if (GetClientTeam(i) == CS_TEAM_T)
					{ SetEntityMoveType(i, MOVETYPE_NONE); }
				}
			}
		}
		if ((WarFTCount <= 5) || ((WarFTCount % 5) == 0))
		{ AC_PrintToChatAll("%t", "war_freeze_time_iter", WarFTCount); }
	}
	return Plugin_Continue;
}

/*
**
** Menu Handler
**
*/

public voteHandler_WarKindVote(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_End)
	{
		CloseHandle(menu);
	}
	else if (action == MenuAction_VoteEnd)
	{
		decl String:info[32];
		GetMenuItem(menu, param1, info, sizeof(info));
		new index = StringToInt(info);
		if (index >= 0 && index < MAX_WARS)
		{ StartWar(index); }
	}
}

/*
**
** Menu Handler
**
*/

/*
**
** SDKHooks
**
*/

public Action:SDKH_OnTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype)
{
	if (WarOnline)
	{
		if (WarInFreezeTime)
		{
			if (IsClientValid(victim) && IsClientValid(attacker))
			{ return Plugin_Handled; }
		}
	}
	return Plugin_Continue;
}

/*
**
** SDKHooks
**
*/

/*
**
** Native's
**
*/

public _:AC_Native_RegWar(Handle:plugin, numParams)
{
	new index = -1;
	for (new i = 0; i < MAX_WARS; i++)
	{
		if (!War_Registered[i])
		{ index = i; break; }
	}
	if (index >= 0)
	{
		War_StartFunc[index] = GetNativeCell(1);
		War_EndFunc[index] = GetNativeCell(2);
		GetNativeString(3, War_Name[index], MAX_WAR_NAME);
		War_HasFreezeTime[index] = bool:GetNativeCell(4);
		if (War_HasFreezeTime[index])
		{
			War_FT_Start[index] = GetNativeCell(5);
			War_FT_End[index] = GetNativeCell(6);
		}
		War_Plugin[index] = plugin;
		War_Registered[index] = true;
		WarCount++;
	}
	return index;
}
public AC_Native_UnRegWar(Handle:plugin, numParams)
{
	new index = GetNativeCell(1);
	if (index >= 0 && index < MAX_WARS)
	{
		if (War_Registered[index])
		{
			War_Registered[index] = false;
			WarCount--;
		}
	}
}

public AC_Native_EndWar(Handle:plugin, numParams)
{
	EndWar(bool:GetNativeCell(1));
}

/*
**
** Native's
**
*/

/*
**
** Forward Callbacks
**
*/

public AddWarCmdToCmdMenu(client, Handle:menu)
{
	if (Alw_warVoteCmd)
	{
		decl String:cmd_string[32];
		decl String:itemString[64];
		GetFirstDelimitedCmd(Str_WarVoteCmd, cmd_string);
		Format(itemString, sizeof(itemString), "%s - %T", PStr_WarVoteCmd, "war_cmd_wvote", client);
		AddMenuItem(menu, cmd_string, itemString, (WarOnline || WarCount == 0) ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);
	}
}

/*
**
** Forward Callbacks
**
*/