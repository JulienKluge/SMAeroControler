/*
**
** Aero Jail Controler Plugin
** WAR Base Control System - partial commands file
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

public Action Cmd_VoteWar(int client, int args)
{
	if (IsClientValid(client)) //the server shouldn't be able to vote...
	{
		if ((!VoteIsOpen) || (!IsNewVoteAllowed()) || (WarCount < 1) || WarOnline)
		{
			AC_PrintToChat(client, "%t", "war_not_voteable");
			return Plugin_Handled;
		}
		if (!HasVotedWar[client])
		{
			int voteCount = 0; int voteAbleCount = 0;
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsClientValid(i))
				{
					voteAbleCount++;
					if (HasVotedWar[i]) { voteCount++; }
				}
			}
			if (voteAbleCount >= VoteMinimumParticipate)
			{
				HasVotedWar[client] = true;
				if (((float(voteCount) / float(voteAbleCount)) * 100.0) >= VoteAmount)
				{
					AC_PrintToChatAll("%t", "war_participate_done", client);
					StartWARKindVote();
				}
				else
				{
					AC_PrintToChatAll("%t", "war_participate", client, RoundToCeil(float(voteAbleCount) * (VoteAmount / 100.0)) - voteCount);
				}
			}
			else
			{
				AC_PrintToChat(client, "%t", "war_not_voteable");
			}
		}
		else
		{
			AC_PrintToChat(client, "%t", "war_still_voted");
		}
	}
	return Plugin_Handled;
}

public Action ACmd_ForceWarMenu(int client, int args)
{
	if (WarOnline)
	{
		ReplyToCommand(client, "[AC]: War is still running. War vote not possible.");
		return Plugin_Handled;
	}
	char target[MAX_NAME_LENGTH];
	if (args == 0)
	{ target = "@all"; /*it's seems legit to make it as a workaround in this case*/ }
	else
	{ GetCmdArg(1, target, sizeof(target)); }
	char buffer[MAX_NAME_LENGTH];
	int targets[MAXPLAYERS+1];
	bool ml = false;
	int count = ProcessTargetString(target, client, targets, sizeof(targets), COMMAND_FILTER_CONNECTED | COMMAND_FILTER_NO_IMMUNITY | COMMAND_FILTER_NO_BOTS, buffer, sizeof(buffer), ml);
	if (count <= 0) { ReplyToTargetError(client, count); return Plugin_Handled; }
	for (int i = 0; i < count; i++)
	{ LogAction(client, targets[i], "[AC]: %L forced warmenu on %L", client, targets[i]); }
	StartWARKindVote(false, targets, count);
	AC_Notify(client, "war_acmd_forcemenu_activity", buffer, ml);
	return Plugin_Handled;
}

public Action ACmd_StopWar(int client, int args)
{
	if (!WarOnline)
	{
		ReplyToCommand(client, "[AC]: No war is running.");
		return Plugin_Handled;
	}
	ForceEndWar();
	LogAction(client, -1, "[AC]: %L stopped a running war.", client);
	AC_RawNotify(client, "war_acmd_stopwar_activity");
	return Plugin_Handled;
}