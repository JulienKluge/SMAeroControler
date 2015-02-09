/*
**
** Aero Jail Controler Plugin
** WAR Base Control System - partial stocks file
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

stock StartWARKindVote(bool:voteForAll = true, _:voteClients[] = {}, voteClientNumber = 0)
{
	if (!WarOnline)
	{
		if (WarCount < 1) { return; }
		if (IsVoteInProgress()) { return; }
		CloseVoteTime();
		new Handle:vote = CreateMenu(voteHandler_WarKindVote, MenuAction_VoteEnd | MenuAction_End);
		decl String:transPhrase[32];
		Format(transPhrase, sizeof(transPhrase), "%T", "war_vote_title", LANG_SERVER);
		SetMenuTitle(vote, transPhrase);
		if (AddNoVoteOption) { SetMenuNoVoteButton(vote, true); }
		SetMenuExitButton(vote, false);
		new WarsIndexArray[WarCount];
		new WarsIndexCount = -1;
		for (new i = 0; i < MAX_WARS; i++)
		{
			if (War_Registered[i])
			{
				WarsIndexCount++;
				WarsIndexArray[WarsIndexCount] = i; //order all in an array
			}
		}
		new menuItemCount = 6;
		if (WarCount < 6)
		{ menuItemCount = WarCount; } //we don't need 6 menu items when less wars are active..
		for (new i = 0; i < menuItemCount; i++)
		{
			new testIndex;
			new arrayIndex;
			for (;;)
			{
				arrayIndex = GetRandomInt(0, WarCount - 1);
				testIndex = WarsIndexArray[arrayIndex];
				if (testIndex != -1) { break; }
			}
			decl String:index[8]; //why 8? Because therefore...
			IntToString(testIndex, index, sizeof(index));
			AddMenuItem(vote, index, War_Name[testIndex]);
			WarsIndexArray[testIndex] = -1;
		}
		if (voteForAll)
		{ VoteMenuToAll(vote, VoteTime); }
		else
		{ VoteMenu(vote, voteClients, voteClientNumber, VoteTime); }
	}
}

stock CloseVoteTime()
{
	VoteIsOpen = false;
	if (ReVoteTimer != INVALID_HANDLE)
	{
		CloseHandle(ReVoteTimer);
	}
	ReVoteTimer = CreateTimer(ReAllowVoteTime, timer_ReAllowVoting);
}

stock StartWar(_:index)
{
	if (WarOnline)
	{ return; }
	ac_setCoreOffline();
	WarOnline = true;
	WarIndex = index;
	WarHasFreezeTime = War_HasFreezeTime[index];
	WarRound = 0;
	LoadWarVectors();
	if (War_StartFunc[index] != WarStart:INVALID_FUNCTION)
	{
		Call_StartFunction(War_Plugin[index], War_StartFunc[index]);
		Call_PushCell(index);
		Call_PushCell(WarMaxRounds);
		Call_PushCell(WarMaxTime);
		Call_Finish();
	}
	if (WarMaxTime > 0)
	{ WarMaxTimer = CreateTimer(float(WarMaxTime), timer_ForceWarEnd, INVALID_HANDLE, TIMER_FLAG_NO_MAPCHANGE); }
	CS_TerminateRound(1.0, CSRoundEnd_Draw, false);
}

stock EndWar(bool:ForceRestart = true)
{
	if (WarOnline)
	{
		WarOnline = false;
		ac_setCoreOnline();
		if (ForceRestart) //is it here in a good position or should it be outside of the first condition???
		{ CS_TerminateRound(1.0, CSRoundEnd_Draw, false); }
		if (WarMaxTimer != INVALID_HANDLE)
		{
			new Handle:subHndl = WarMaxTimer;
			WarMaxTimer = INVALID_HANDLE;
			CloseHandle(subHndl);
		}
		if (WarFreezeTimer != INVALID_HANDLE)
		{
			new Handle:subHndl = WarFreezeTimer;
			WarFreezeTimer = INVALID_HANDLE;
			CloseHandle(subHndl);
		}
		AC_PrintToChatAll("%t", "war_war_end");
		PrintCenterTextAll("%t", "war_war_end_plain");
		PrintHintTextToAll("%t", "war_war_end_plain");
	}
}

stock ForceEndWar()
{
	if (War_EndFunc[WarIndex] != WarEnd:INVALID_FUNCTION)
	{
		Call_StartFunction(War_Plugin[WarIndex], War_EndFunc[WarIndex]);
		Call_PushCell(WarIndex);
		Call_Finish();
	}
	EndWar();
}

stock LoadWarVectors()
{
	new bool:OverrideFound = false;
	new Handle:kv = CreateKeyValues("warvectors");
	decl String:file[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, file, sizeof(file), "configs/AC_WAR_VectorOverride.ini");
	if (FileToKeyValues(kv, file))
	{
		decl String:keyName[PLATFORM_MAX_PATH]; //How long can a mapname be?
		GetCurrentMap(keyName, sizeof(keyName));
		if (KvJumpToKey(kv, keyName))
		{
			KvGetVector(kv, "spawnPos", WarFreezePosition);
			KvGetVector(kv, "eyeAngel", WarFreezeEyeAng);
			OverrideFound = true;
		}
	}
	CloseHandle(kv);
	if (!OverrideFound)
	{
		new lastSpawnEnt = -1;
		new searchOffset = MaxClients + 1;
		while ((searchOffset = FindEntityByClassname(searchOffset, "info_player_counterterrorist")) != -1)
		{ lastSpawnEnt = searchOffset; } //we want the last spawn - the probability should be lower that a ct is spawning there...I think
		if (lastSpawnEnt >= searchOffset)
		{
			GetEntPropVector(lastSpawnEnt, Prop_Data, "m_vecOrigin", WarFreezePosition);
			GetEntPropVector(lastSpawnEnt, Prop_Data, "m_angRotation", WarFreezeEyeAng);
		}
		else
		{
			WarFreezePosition = NULL_VECTOR; WarFreezeEyeAng = NULL_VECTOR;
		}
	}
}