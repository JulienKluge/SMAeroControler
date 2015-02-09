/*
**
** Aero Jail Controler Plugin
** Mute Core
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
** Notes: This file shouldn't have an own branch do you think?
** Well, at the moment you're right but in the future, when special muting
** systems will come , the number of lines will increase dramaticly.
**
*/

#undef REQUIRE_PLUGIN
#include <basecomm>
#define REQUIRE_PLUGIN

new mutedTTime = 40;
new bool:BaseCommIsAvailable = false;
new c_MuteState[MAXPLAYERS + 1]; //0 = muted | 1 = talk | 2 = tempmute
//The tempmute-state will have sense with special mute systems.
new bool:c_MuteBanned[MAXPLAYERS + 1];
new bool:c_MuteImmunity[MAXPLAYERS + 1];
new Handle:endTempMute = INVALID_HANDLE;
new bool:tempMudeEnded = false;
new Float:c_StartetTempMute = -1.0;

stock MuteSys_LoadClient(client)
{
	SetClientListeningFlags(client, VOICE_MUTED);
	c_MuteBanned[client] = MuteSys_GetClientMuteBanState(client);
	new AdminId:adm_id = GetUserAdmin(client);
	#if defined DEBUG
	adm_id = INVALID_ADMIN_ID;
	#endif
	if (adm_id == INVALID_ADMIN_ID)
	{ c_MuteImmunity[client] = false; MuteSys_SetMuteState(client); }
	else
	{
		c_MuteImmunity[client] = GetAdminFlag(adm_id, Admin_Generic);
		if (c_MuteImmunity[client])
		{ MuteSys_SetTalkState(client); }
		else
		{ MuteSys_SetMuteState(client); }
	}
}

stock MuteSys_SetMuteState(client, bool:isOutOfClientDeath = false)
{
	c_MuteState[client] = 0;
	if (c_MuteImmunity[client]) { return; }
	SetClientListeningFlags(client, VOICE_MUTED);
	if (isOutOfClientDeath)
	{ AC_PrintToChat(client, "%t", "mute_mute"); }
}

stock MuteSys_SetTempMuteState(client, bool:isOutOfTempMute = false)
{
	c_MuteState[client] = 2;
	if (c_MuteImmunity[client]) { return; }
	if (!tempMudeEnded)
	{
		SetClientListeningFlags(client, VOICE_MUTED);
		if (isOutOfTempMute)
		{
			if (c_StartetTempMute <= 0.0) { c_StartetTempMute = GetGameTime(); }
			new amount = mutedTTime - RoundFloat(GetGameTime() - c_StartetTempMute);
			if (amount > 0)
			{ AC_PrintToChat(client, "%t", "mute_tempmutebegins", amount); }
		}
	}
}

stock MuteSys_SetTalkState(client)
{
	c_MuteState[client] = 1;
	if (c_MuteBanned[client]) { return; }
	SetClientListeningFlags(client, VOICE_NORMAL);
}

stock bool:MuteSys_GetClientMuteBanState(client)
{
	if (BaseCommIsAvailable)
	{ return BaseComm_IsClientMuted(client); }
	return false;
}

public BaseComm_OnClientMute(client, bool:muteState)
{
	c_MuteBanned[client] = muteState;
	if (!muteState)
	{
		if (c_MuteState[client] == 0)
		{ MuteSys_SetMuteState(client); }
		else if (c_MuteState[client] == 1)
		{ MuteSys_SetTalkState(client); }
		else if (c_MuteState[client] == 2)
		{ MuteSys_SetTempMuteState(client); }
	}
}

stock MuteSys_LibraryAdded_Detour(const String:name[])
{
	if (StrEqual(name, "basecomm"))
	{ BaseCommIsAvailable = true; }
}

stock MuteSys_LibraryRemoved_Detour(const String:name[])
{
	if (StrEqual(name, "basecomm"))
	{ BaseCommIsAvailable = false; }
}

stock MuteSys_RoundStart_Detour()
{
	new teamIndex;
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			teamIndex = GetClientTeam(i);
			if (!(teamIndex == CS_TEAM_CT || teamIndex == CS_TEAM_T))
			{ MuteSys_SetMuteState(i); }
		}
	}
	c_StartetTempMute = GetGameTime();
	endTempMute = CreateTimer(float(mutedTTime), timer_EndTempMute, INVALID_HANDLE, TIMER_FLAG_NO_MAPCHANGE);
}

stock MuteSys_ClientSpawn_Detour(client)
{
	new teamIndex = GetClientTeam(client);
	if (teamIndex == CS_TEAM_CT)
	{ MuteSys_SetTalkState(client); }
	else if (teamIndex == CS_TEAM_T)
	{ MuteSys_SetTempMuteState(client, true); }
	else
	{ MuteSys_SetMuteState(client); }
}

stock MuteSys_ClientDeath_Detour(client)
{
	MuteSys_SetMuteState(client, true);
}

stock MuteSys_RoundEnd_Detour()
{
	c_StartetTempMute = -1.0;
	tempMudeEnded = false;
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			MuteSys_SetTalkState(i);
		}
	}
	if (endTempMute != INVALID_HANDLE)
	{
		new Handle:subHndl = endTempMute;
		endTempMute = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
}

public Action:timer_EndTempMute(Handle:timer)
{
	endTempMute = INVALID_HANDLE;
	tempMudeEnded = true;
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			if (IsPlayerAlive(i) && GetClientTeam(i) == CS_TEAM_T)
			{
				MuteSys_SetTalkState(i);
			}
		}
	}
	AC_PrintToChatAll("%t", "mute_talkafter_tempmute");
	return Plugin_Stop;
}