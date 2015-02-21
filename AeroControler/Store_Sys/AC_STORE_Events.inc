/*
**
** Aero Jail Controler Plugin
** Store System - partial events file
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

public Action Event_RoundEnd(Handle event, const char[] name, bool dontBroadcast)
{
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			int teamIndex = GetClientTeam(i);
			if (IsPlayerAlive(i))
			{
				if (teamIndex == CS_TEAM_T)
				{ GainPoints(i, p_Survive_T); }
				else if (teamIndex == CS_TEAM_CT)
				{ GainPoints(i, p_Survive_CT); }
			}
			if (teamIndex == CS_TEAM_T || teamIndex == CS_TEAM_CT)
			{
				GainPoints(i, p_Round);
				UpdateClientAsync(i);
				virtualNewPoints[i] = virtualNewPoints[i] * c_MultiplierPrivate[i] * c_MultiplierPublic[i] * p_PointsMultiplierPublic;
				if (virtualNewPoints[i] >= 0.1)
				{
					if (sf_member[i] != -1)
					{
						float virtualSFPoints = virtualNewPoints[i] * sf_tax[i];
						float virtualPoints = virtualNewPoints[i] * (1.0 - sf_tax[i]);
						PrintCenterText(i, "%t", "sf_sf_gained_string", virtualPoints, currentName, virtualSFPoints, currentName);
					}
					else
					{
						PrintCenterText(i, "%t", "sf_gained_string", virtualNewPoints[i], currentName);
					}
				}
				virtualNewPoints[i] = 0.0;
			}
		}
		c_HasRespawn[i] = false;
		c_MultiplierPrivate[i] = 1.0;
		c_PressureNadeCount[i] = 0;
		if (c_InvisibleSeconds[i] > 0)
		{ SetEntityRenderMode(i, RENDER_NORMAL); }
		c_InvisibleSeconds[i] = 0;
		Handle subHndl;
		if (c_RespawnTimer[i] != INVALID_HANDLE)
		{
			subHndl = c_RespawnTimer[i];
			c_RespawnTimer[i] = INVALID_HANDLE;
			CloseHandle(subHndl);
		}
		if (c_InvisCountTimer[i] != INVALID_HANDLE)
		{
			subHndl = c_InvisCountTimer[i];
			c_InvisCountTimer[i] = INVALID_HANDLE;
			CloseHandle(subHndl);
		}
	}
	return Plugin_Continue;
}

public Action Event_PlayerSpawn(Handle event, const char[] name, bool dontBroadcast)
{
	if (!InWork) { return Plugin_Continue; }
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (IsClientValid(client))
	{
		virtualNewPoints[client] = 0.0;
		if (sf_hasSkill[client])
		{
			if (sf_skill[client] == Skill_Medic)
			{ c_MedicSkillAmount[client] = sf_skilllevel[client] * Skill_Medic_HPPerLevel; }
		}
	}
	return Plugin_Continue;
}

public Action Event_PlayerDeath(Handle event, const char[] name, bool dontBroadcast)
{
	if (!InWork) { return Plugin_Continue; }
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	int attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	if (IsClientValid(client))
	{
		if (IsClientValid(attacker))
		{
			int teamIndex_Vic = GetClientTeam(client);
			int teamIndex_Att = GetClientTeam(attacker);
			if (teamIndex_Vic == CS_TEAM_T && teamIndex_Att == CS_TEAM_CT)
			{ GainPoints(attacker, p_CT_Kill_T); }
			if (teamIndex_Vic == CS_TEAM_CT && teamIndex_Att == CS_TEAM_T)
			{ GainPoints(attacker, p_T_Kill_CT); }
		}
		if (c_HasRespawn[client])
		{
			Handle pack;
			c_RespawnTimer[client] = CreateDataTimer(3.0, timer_RespawnClient, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
			WritePackCell(pack, client);
			WritePackCell(pack, GetClientUserId(client));
			AC_PrintToChat(client, "%t", "store_respawnprivate");
		}
	}
	return Plugin_Continue;
}

public Action Event_HEDetonate(Handle event, const char[] name, bool dontBroadcast)
{
	if (!InWork) { return Plugin_Continue; }
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (IsClientValid(client))
	{
		if (c_PressureNadeCount[client] > 0)
		{
			c_PressureNadeCount[client]--;
			float pos[3];
			pos[0] = GetEventFloat(event, "x");
			pos[1] = GetEventFloat(event, "y");
			pos[2] = GetEventFloat(event, "z");
			float cPos[3];
			int teamIndex = GetClientTeam(client);
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsClientValid(i) && i != client)
				{
					if (GetClientTeam(i) != teamIndex)
					{
						GetClientAbsOrigin(i, cPos);
						float length = GetVectorDistance(pos, cPos);
						if (length <= 300.0)
						{
							float vec[3];
							SubtractVectors(cPos, pos, vec);
							NormalizeVector(vec, vec);
							ScaleVector(vec, (300.0 / length) * 1000.0);
							TeleportEntity(i, NULL_VECTOR, NULL_VECTOR, vec);
						}
					}
				}
			}
			TE_SetupBeamRingPoint(pos, 20.0, 200.0, g_BeamSprite, g_HaloSprite, 0, 10, 0.5, 2.0, 0.0, { 255, 255, 255, 255 }, 10, 0);
			TE_SendToClient(client);
		}
	}
	return Plugin_Continue;
}