/*
**
** Aero Jail Controler Plugin
** Store System - partial commands file
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

public Action Cmd_OpenStore(int client, int args)
{
	if (!InWork) { return Plugin_Handled; }
	UpdateClientAsync(client);
	OpenStoreMenu(client);
	return Plugin_Handled;
}

public Action Cmd_OpenStoreTop(int client, int args)
{
	char query[160];
	Format(query, sizeof(query), "SELECT name,points,gained FROM `%s` ORDER BY (points+gained) DESC LIMIT 20", tableName);
	SQL_TQuery(db, TSQL_StoreTopCmd, query, GetClientUserId(client));
	return Plugin_Handled;
}

public Action Cmd_OpenStoreRank(int client, int args)
{
	char query[160];
	char id[32];
	GetClientAuthId(client, AuthId_Engine, id, sizeof(id));
	Format(query, sizeof(query), "SELECT (points+gained) FROM `%s` WHERE steamid='%s'", tableName, id);
	SQL_TQuery(db, TSQL_StoreRankCmdRelay, query, GetClientUserId(client));
	return Plugin_Handled;
}

public Action Cmd_SkillForce(int client, int args)
{
	OpenSkillForceMenu(client);
	return Plugin_Handled;
}

public Action Cmd_SFChat(int client, int args)
{
	int sf = sf_member[client];
	if (sf == -1)
	{
		AC_PrintToChat(client, "%t", "sf_sfc_nosf");
		return Plugin_Handled;
	}
	char message[192];
	GetCmdArgString(message, sizeof(message));
	TrimString(message);
	StripQuotes(message);
	if (strlen(message) > 0)
	{
		IncludeTag = false;
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsClientValid(i))
			{
				if (sf_member[i] == sf)
				{
					AC_PrintToChat(i, "\x07AAAAAA[SF]%N: %s", client, message);
				}
			}
		}
		IncludeTag = true;
	}
	return Plugin_Handled;
}