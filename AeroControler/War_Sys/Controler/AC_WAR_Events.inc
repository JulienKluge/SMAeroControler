/*
**
** Aero Jail Controler Plugin
** WAR Base Control System - partial event file
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

public Action:Event_RoundEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (WarOnline)
	{
		WarRound++;
		if (WarFreezeTimer != INVALID_HANDLE)
		{
			new Handle:subHndl = WarFreezeTimer;
			WarFreezeTimer = INVALID_HANDLE;
			CloseHandle(subHndl);
		}
		if ((WarRound - 1) == WarMaxRounds)
		{ ForceEndWar(); }
	}
	return Plugin_Continue;
}

public Action:Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (WarOnline)
	{
		if (WarHasFreezeTime)
		{
			WarInFreezeTime = true;
			WarFTCount = WarFreezeTime;
			WarFreezeTimer = CreateTimer(1.0, timer_EndFreezeTime, INVALID_HANDLE, TIMER_FLAG_NO_MAPCHANGE | TIMER_REPEAT);
			if (War_FT_Start[WarIndex] != WarFreezeTimeStart:INVALID_HANDLE)
			{
				Call_StartFunction(War_Plugin[WarIndex], War_FT_Start[WarIndex]);
				Call_PushCell(WarFreezeTime);
				Call_Finish();
			}
		}
		AC_PrintToChatAll("%t", "war_war_start", War_Name[WarIndex], WarRound, WarMaxRounds);
		PrintCenterTextAll("%t", "war_war_start_plain");
		PrintHintTextToAll("%t", "war_war_start_plain");
	}
	return Plugin_Continue;
}

public Action:Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (WarOnline)
	{
		SetEntData(client, g_Offset_CollisionGroup, COLLISION_GROUP_DEBRIS_TRIGGER, 4, true);
		if (WarHasFreezeTime)
		{
			if (GetClientTeam(client) == CS_TEAM_T)
			{
				TeleportEntity(client, WarFreezePosition, WarFreezeEyeAng, NULL_VECTOR);
				ApplyClientFade(client, 2000, { 255, 255, 255, 255 }, false, true, true);
				RemoveAllClientItems(client);
				SetEntityMoveType(client, MOVETYPE_NONE);
			}
		}
	}
}