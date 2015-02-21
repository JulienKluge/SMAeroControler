/*
**
** Aero Jail Controler Plugin
** Store System - partial sql callbacks file
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

public void TSQL_SelectJoiner(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (client == 0) { return; } //year...when will this happen? ... SAFETYYY FIRST XD
	char id[32];
	char name[32];
	char sqlname[65];
	GetClientAuthId(client, AuthId_Engine, id, sizeof(id));
	GetClientName(client, name, sizeof(name));
	SQL_EscapeString(db, name, sqlname, sizeof(sqlname));
	if (SQL_FetchRow(hndl)) //client is already in database
	{
		points[client] = SQL_FetchFloat(hndl, 0);
		int daysTillLastJoin = SQL_FetchInt(hndl, 1);
		sf_member[client] = SQL_FetchInt(hndl, 2);
		sf_level[client] = SQL_FetchInt(hndl, 3);
		sf_skill[client] = SQL_FetchInt(hndl, 4);
		sf_skilllevel[client] = SQL_FetchInt(hndl, 5);
		if (sf_member[client] != -1 && sf_skill[client] > 0)
		{ sf_hasSkill[client] = true; }
		char query[270]; //264 characters maximum for the second query...
		if (daysTillLastJoin == 0 || p_JoinDay == 0.0)
		{
			Format(query, sizeof(query), "UPDATE `%s` SET name='%s',lastjoin=NOW() WHERE steamid='%s'", tableName, sqlname, p_JoinDay, p_JoinDay, id);
		}
		else
		{
			points[client] += p_JoinDay;
			Format(query, sizeof(query), "UPDATE `%s` SET name='%s',points=points+%f,gained=gained+%f,lastjoin=NOW() WHERE steamid='%s'", tableName, sqlname, p_JoinDay, p_JoinDay, id);
		}
		SQL_TQuery(db, TSQL_NULL, query); //I'm not interested in the result of that query...
		if (sf_member[client] != -1)
		{
			Format(query, sizeof(query), "SELECT tax FROM `%s` WHERE id=%i", sfTableName, sf_member[client]);
			SQL_TQuery(db, TSQL_FetchSFTax, query, data);
		}
	}
	else //client isn't in the database
	{
		points[client] = p_FirstJoin;
		sf_member[client] = -1;
		char query[384];
		Format(query, sizeof(query), "INSERT INTO `%s`(steamid,name,points,gained,spend,lastjoin,skillforce,sfgained,sflevel,sfskill,sfskilllevel) VALUES ('%s','%s',%f,%f,0.0,NOW(),-1,0.0,0,0,0)", tableName, id, sqlname, p_FirstJoin, p_FirstJoin);
		SQL_TQuery(db, TSQL_NULL, query, 0, DBPrio_High); //the only time, you'll see a high db priority in this plugin...I think xD
	}
}

public void TSQL_FetchSFTax(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (client == 0) { return; }
	if (SQL_FetchRow(hndl))
	{ sf_tax[client] = SQL_FetchFloat(hndl, 0); }
	else
	{ sf_member[client] = -1; } //failsafe
}

public void TSQL_CheckValidTransferLimit(Handle owner, Handle hndl, const char[] error, any data)
{
	Handle pack = Handle:data; //first thing to do: read data packet and dispose it
	ResetPack(pack, false);
	int client = GetClientOfUserId(ReadPackCell(pack));
	float price = ReadPackFloat(pack);
	int code = ReadPackCell(pack);
	CloseHandle(pack);
	if (IsClientValid(client)) //fast client || slow query xD
	{
		if (IsPlayerAlive(client))
		{
			if (SQL_FetchRow(hndl))
			{
				points[client] = SQL_FetchFloat(hndl, 0); //year, fetch the result and update his account info...two birds with one stone xD
				if (points[client] >= price)
				{
					if (!CheckStoreFeatureForClient(client, code)) { return; }
					DescreasePoints(client, price);	//First descrease the client points because, if there are any errors while applying the feature,
					ApplyCodeFeature(client, code);	//the clients could abuse it to gain unlimited features while not losing points.
					OpenStoreMenu(client);
				}
				else //don't try to beat me ^^
				{ AC_PrintToChat(client, "%t", "store_insufficientmoney", currentName); }
			}
		}
	}
}

public void TSQL_StoreTopCmd(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		int count = 20;
		int rowCount = SQL_GetRowCount(hndl);
		if (rowCount < count) { count = rowCount; }
		if (count > 0)
		{
			Handle menu = CreateMenu(menuHandler_CloseHandleOnEnd, MenuAction_End);
			char name[32];
			char itemString[64];
			float r_points;
			float r_gained;
			for (int i = 0; i < count; i++)
			{
				SQL_FetchRow(hndl);
				SQL_FetchString(hndl, 0, name, sizeof(name));
				r_points = SQL_FetchFloat(hndl, 1);
				r_gained = SQL_FetchFloat(hndl, 2);
				Format(itemString, sizeof(itemString), "%T", "store_rankstring_1", client, name, (i + 1));
				AddMenuItem(menu, "", itemString, ITEMDRAW_DEFAULT);
				Format(itemString, sizeof(itemString), "%T", "store_rankstring_2", client, r_points, currentName, r_gained, currentName);
				AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			}
			DisplayMenu(menu, client, 45);
		}
	}
}

public void TSQL_StoreRankCmdRelay(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			float valueToCheckFor = SQL_FetchFloat(hndl, 0);
			char query[160];
			Format(query, sizeof(query), "SELECT COUNT(*) FROM `%s` WHERE (points+gained)>=%f", tableName, valueToCheckFor);
			SQL_TQuery(db, TSQL_StoreRankCmd, query, data);
		}
	}
}
public void TSQL_StoreRankCmd(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			int place = SQL_FetchInt(hndl, 0);
			AC_PrintToChat(client, "%t", "store_rank", place);
		}
	}
}

public void TSQL_TopSFs(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		int count = 20;
		int rowCount = SQL_GetRowCount(hndl);
		if (rowCount < count) { count = rowCount; }
		if (count > 0)
		{
			Handle menu = CreateMenu(menuHandler_BackToSfMenu, MenuAction_Cancel | MenuAction_End);
			SetMenuExitButton(menu, true);
			SetMenuExitBackButton(menu, true);
			char name[32];
			char itemString[64];
			float account;
			float gained;
			for (int i = 0; i < count; i++)
			{
				SQL_FetchRow(hndl);
				SQL_FetchString(hndl, 0, name, sizeof(name));
				account = SQL_FetchFloat(hndl, 1);
				gained = SQL_FetchFloat(hndl, 2);
				Format(itemString, sizeof(itemString), "%T", "sfmenu_ranksfs_1", client, name, (i + 1));
				AddMenuItem(menu, "", itemString, ITEMDRAW_DEFAULT);
				Format(itemString, sizeof(itemString), "%T", "sfmenu_ranksfs_2", client, account, currentName, gained, currentName);
				AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			}
			DisplayMenu(menu, client, 45);
		}
	}
}

public void TSQL_TopSFPlayers(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		int count = 10;
		int rowCount = SQL_GetRowCount(hndl);
		if (rowCount < count) { count = rowCount; }
		if (count > 0)
		{
			Handle menu = CreateMenu(menuHandler_BackToSfMenu, MenuAction_Cancel | MenuAction_End);
			SetMenuExitButton(menu, true);
			SetMenuExitBackButton(menu, true);
			char name[32];
			char sfName[32];
			char itemString[64];
			float gained;
			for (int i = 0; i < count; i++)
			{
				SQL_FetchRow(hndl);
				SQL_FetchString(hndl, 0, name, sizeof(name));
				gained = SQL_FetchFloat(hndl, 1);
				SQL_FetchString(hndl, 2, sfName, sizeof(sfName));
				Format(itemString, sizeof(itemString), "%T", "sfmenu_ranksfplayers_1", client, name, (i + 1));
				AddMenuItem(menu, "", itemString, ITEMDRAW_DEFAULT);
				Format(itemString, sizeof(itemString), "%T", "sfmenu_ranksfplayers_2", client, gained, currentName);
				AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
				AddMenuItem(menu, "", sfName, ITEMDRAW_DISABLED);
			}
			DisplayMenu(menu, client, 60);
		}
	}
}

public void TSQL_CheckUniqueSFName(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	char name[32];
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	if (IsClientValid(client))
	{
		int resultCount = SQL_GetRowCount(hndl);
		if (resultCount > 0)
		{ AC_PrintToChat(client, "%t", "sf_foundsf_nametaken", name); }
		else
		{
			Handle pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(client));
			WritePackString(pack, name);
			Handle menu = CreateMenu(menuHandler_ChooseCoFounder, MenuAction_Select | MenuAction_End);
			SetMenuTitle(menu, "%T", "sfmenu_foundsf_cofounder", client);
			char handleString[32];
			IntToString(_:pack, handleString, sizeof(handleString));
			AddMenuItem(menu, handleString, "", ITEMDRAW_IGNORE); //tricky way to store data in a menu ^^
			char uid[32];
			char cName[32];
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsClientValid(i))
				{
					if (i != client && sf_member[i] == -1)
					{
						IntToString(GetClientUserId(i), uid, sizeof(uid));
						GetClientName(i, cName, sizeof(cName));
						AddMenuItem(menu, uid, cName);
					}
				}
			}
			DisplayMenu(menu, client, 60);
		}
	}
}

public void TSQL_AccomplishSFFounding(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	ReadPackCell(data); ReadPackCell(data);
	char subStr[1]; /* THAT SUCKS!! How to calc the offset in a datapack to use SatPackPosition? */
	ReadPackString(data, subStr, sizeof(subStr));
	ReadPackString(data, subStr, sizeof(subStr));
	char sfName[32];
	ReadPackString(data, sfName, sizeof(sfName));
	char query[160];
	Format(query, sizeof(query), "SELECT id FROM `%s` WHERE name='%s'", sfTableName, sfName);
	SQL_TQuery(db, TSQL_AccomplishSFFounders, query, data);
}

public void TSQL_AccomplishSFFounders(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int founder1 = GetClientOfUserId(ReadPackCell(data));
	bool founder1_Valid = IsClientValid(founder1);
	int founder2 = GetClientOfUserId(ReadPackCell(data));
	bool founder2_Valid = IsClientValid(founder2);
	if (SQL_FetchRow(hndl))
	{
		int id = SQL_FetchInt(hndl, 0);
		char id_1[32];
		char id_2[32];
		ReadPackString(data, id_1, sizeof(id_1));
		ReadPackString(data, id_2, sizeof(id_2));
		char sfName[32];
		ReadPackString(data, sfName, sizeof(sfName));
		char query[255];
		Format(query, sizeof(query), "UPDATE `%s` SET skillforce=%i,sfgained=0.0,sflevel=2,sfskill=0,sfskilllevel=0 WHERE steamid='%s' OR steamid='%s'", tableName, id, id_1, id_2);
		SQL_TQuery(db, TSQL_NULL, query);
		if (founder1_Valid || founder2_Valid)
		{
			if (founder1_Valid && founder2_Valid)
			{ AC_PrintToChatAll("%t", "sf_founded_1", founder1, founder2, sfName); }
			else if (founder1_Valid)
			{ AC_PrintToChatAll("%t", "sf_founded_2", founder1, id_2, sfName); }
			else if (founder2_Valid)
			{ AC_PrintToChatAll("%t", "sf_founded_2", founder2, id_1, sfName); }
			if (founder1_Valid)
			{ sf_member[founder1] = id; sf_level[founder1] = 2; sf_tax[founder1] = 0.2; }
			if (founder2_Valid)
			{ sf_member[founder2] = id; sf_level[founder2] = 2; sf_tax[founder2] = 0.2; }
		}
	}
	else
	{
		if (founder1_Valid)
		{ AC_PrintToChat(founder1, "%t", "sf_found_error"); }
		if (founder2_Valid)
		{ AC_PrintToChat(founder2, "%t", "sf_found_error"); }
	}
	CloseHandle(data);
}

public void TSQL_OpenSFMenu(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		char sfName[32];
		if (SQL_FetchRow(hndl)) //well, the if is sensless, why do i check an impossible possibility? FAILSAFE XD
		{
			SQL_FetchString(hndl, 0, sfName, sizeof(sfName));
			char itemString[64];
			Handle menu = CreateMenu(menuHandler_SFMenu, MenuAction_Select | MenuAction_End);
			SetMenuTitle(menu, sfName);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_sf", client);
			AddMenuItem(menu, "sfrce", itemString);
			if (sf_level[client] > 0)
			{
				Format(itemString, sizeof(itemString), "%T", "sfmenu_payoff", client, currentName);
				AddMenuItem(menu, "payof", itemString);
				Format(itemString, sizeof(itemString), "%T", "sfmenu_addmember", client);
				AddMenuItem(menu, "addmm", itemString);
				Format(itemString, sizeof(itemString), "%T", "sfmenu_removemember", client);
				AddMenuItem(menu, "rmvmm", itemString);
				Format(itemString, sizeof(itemString), "%T", "sfmenu_setskill", client);
				AddMenuItem(menu, "setsk", itemString);
				Format(itemString, sizeof(itemString), "%T", "sfmenu_setskilllevel", client);
				AddMenuItem(menu, "sklvl", itemString);
				if (sf_level[client] == 2)
				{
					Format(itemString, sizeof(itemString), "%T", "sfmenu_memberrights", client);
					AddMenuItem(menu, "mrght", itemString);
					Format(itemString, sizeof(itemString), "%T", "sfmenu_changetaxes", client);
					AddMenuItem(menu, "cngtx", itemString);
				}
			}
			Format(itemString, sizeof(itemString), "%T", "sfmenu_memberlist", client);
			AddMenuItem(menu, "mates", itemString);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_memberranks", client);
			AddMenuItem(menu, "mbrnk", itemString);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_ranksfs", client);
			AddMenuItem(menu, "ranks", itemString);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_rankplayers", client);
			AddMenuItem(menu, "rankp", itemString);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_leave", client);
			AddMenuItem(menu, "leave", itemString);
			if (sf_level[client] == 2)
			{
				Format(itemString, sizeof(itemString), "%T", "sfmenu_delete", client);
				AddMenuItem(menu, "delet", itemString);
			}
			DisplayMenu(menu, client, 60);
		}
	}
}

public void TSQL_MySFRelay(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			char name[32];
			SQL_FetchString(hndl, 0, name, sizeof(name));
			int membercount = SQL_FetchInt(hndl, 1);
			float account = SQL_FetchFloat(hndl, 2);
			float gained = SQL_FetchFloat(hndl, 3);
			float tax = SQL_FetchFloat(hndl, 4);
			Handle pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(client));
			WritePackString(pack, name);
			WritePackCell(pack, membercount);
			WritePackFloat(pack, account);
			WritePackFloat(pack, gained);
			WritePackFloat(pack, tax);
			char query[255];
			Format(query, sizeof(query), "SELECT COUNT(*) FROM `%s` WHERE (account+gained)>=(SELECT (account+gained) FROM `%s` WHERE id=%i)", sfTableName, sfTableName, sf_member[client]);
			SQL_TQuery(db, TSQL_MySF, query, pack);
		}
	}
}

public void TSQL_MySF(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	char name[32];
	ReadPackString(data, name, sizeof(name));
	int membercount = ReadPackCell(data);
	float account = ReadPackFloat(data);
	float gained = ReadPackFloat(data);
	float tax = ReadPackFloat(data);
	int RealTaxes = RoundFloat(tax * 100);
	CloseHandle(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			int rank = SQL_FetchInt(hndl, 0);
			char itemString[64];
			Handle menu = CreateMenu(menuHandler_BackToSfMenu, MenuAction_Cancel | MenuAction_End);
			SetMenuExitButton(menu, true);
			SetMenuExitBackButton(menu, true);
			AddMenuItem(menu, "", name, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_mysf_rank", client, rank);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			AddMenuItem(menu, "", "", ITEMDRAW_SPACER | ITEMDRAW_NOTEXT);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_mysf_member", client, membercount);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_mysf_account", client, account, currentName);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_mysf_gained", client, gained, currentName);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_mysf_taxes", client, RealTaxes);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			DisplayMenu(menu, client, 45);
		}
	}
}

public void TSQL_DeletedSFGetName(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			char name[32];
			SQL_FetchString(hndl, 0, name, sizeof(name));
			int id = sf_member[client];
			char query[160];
			Handle pack = CreateDataPack();
			WritePackCell(pack, id);
			WritePackCell(pack, GetClientUserId(client));
			WritePackString(pack, name);
			Format(query, sizeof(query), "UPDATE `%s` SET skillforce=-1,sfgained=0.0,sflevel=0,sfskill=0,sfskilllevel=0 WHERE skillforce=%i", tableName, id);
			SQL_TQuery(db, TSQL_DeletedSFRelay, query, pack);
		}
	}
}

public void TSQL_DeletedSFRelay(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int id = ReadPackCell(data);
	char query[128];
	Format(query, sizeof(query), "DELETE FROM `%s` WHERE id=%i", sfTableName, id);
	SQL_TQuery(db, TSQL_DeletedSF, query, data);
}

public void TSQL_DeletedSF(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int id = ReadPackCell(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	char name[32];
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	for (int i = 1; i <= MaxClients; i++)
	{
		if (sf_member[i] == id)
		{ sf_member[i] = -1; }
	}
	if (IsClientValid(client))
	{ AC_PrintToChatAll("%t", "sf_deleted_client", client, name); }
	else
	{ AC_PrintToChatAll("%t", "sf_deleted", name); }
}

public void TSQL_SFTaxesChanged(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int id = ReadPackCell(data);
	float tax = ReadPackFloat(data);
	CloseHandle(data);
	int newTax = RoundFloat(tax * 100.0);
	int clientValid = IsClientValid(client);
	for (int i = 1; i <= MaxClients; i++)
	{
		if (sf_member[i] == id)
		{
			sf_tax[i] = tax;
			if (clientValid)
			{
				if (client == i)
				{ AC_PrintToChat(i, "%t", "sf_taxesset", newTax); }
				else
				{ AC_PrintToChat(i, "%t", "sf_taxesset_client", client, newTax); }
			}
			else
			{ AC_PrintToChat(i, "%t", "sf_taxesset", newTax); }
		}
	}
}

public void TSQL_SFMemberList(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		Handle menu = CreateMenu(menuHandler_BackToSfMenu, MenuAction_Cancel | MenuAction_End);
		SetMenuExitButton(menu, true);
		SetMenuExitBackButton(menu, true);
		int count = SQL_GetRowCount(hndl);
		char itemString[64];
		char name[32];
		char roleName[28];
		int sfLevel;
		int skill;
		int skilllevel;
		for (int i = 0; i < count; i++)
		{
			if (SQL_FetchRow(hndl))
			{
				SQL_FetchString(hndl, 0, name, sizeof(name));
				sfLevel = SQL_FetchInt(hndl, 1);
				skill = SQL_FetchInt(hndl, 2);
				skilllevel = SQL_FetchInt(hndl, 3);
				if (sfLevel == 2)
				{ Format(roleName, sizeof(roleName), "%T", "sf_role_proadmin", client); }
				else if (sfLevel == 1)
				{ Format(roleName, sizeof(roleName), "%T", "sf_role_admin", client); }
				else
				{ Format(roleName, sizeof(roleName), "%T", "sf_role_member", client); }
				Format(itemString, sizeof(itemString), "%s  (%s)", name, roleName);
				AddMenuItem(menu, "", itemString, ITEMDRAW_DEFAULT);
				if (skill > 0)
				{
					ConvertSkillNumberIntoString(skill, name, sizeof(name), client); //what a misuse of the name-variable xD
					Format(itemString, sizeof(itemString), "%T", "sfmenu_skillline", client, name, skilllevel);
					AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
				}
				else
				{
					Format(itemString, sizeof(itemString), "%T", "sf_skill_none", client);
					AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
				}
			}
		}
		DisplayMenu(menu, client, 60);
	}
}

public void TSQL_SFMemberRanks(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		Handle menu = CreateMenu(menuHandler_BackToSfMenu, MenuAction_Cancel | MenuAction_End);
		SetMenuExitButton(menu, true);
		SetMenuExitBackButton(menu, true);
		int count = SQL_GetRowCount(hndl);
		char itemString[64];
		char name[32];
		float sfGained;
		for (int i = 0; i < count; i++)
		{
			if (SQL_FetchRow(hndl))
			{
				SQL_FetchString(hndl, 0, name, sizeof(name));
				sfGained = SQL_FetchFloat(hndl, 1);
				AddMenuItem(menu, "", name, ITEMDRAW_DEFAULT);
				Format(itemString, sizeof(itemString), "%0.1f %s", sfGained, currentName);
				AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			}
		}
		DisplayMenu(menu, client, 60);
	}
}

public void TSQL_SetSFRightsMenu(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int memberLevel = ReadPackCell(data);
	CloseHandle(data);
	if (IsClientValid(client))
	{
		char memberLevelStr[8];
		IntToString(memberLevel, memberLevelStr, sizeof(memberLevelStr));
		char steamid[32];
		GetClientAuthId(client, AuthId_Engine, steamid, sizeof(steamid));
		int count = SQL_GetRowCount(hndl);
		char name[32];
		char sid[32];
		Handle menu = CreateMenu(menuHandler_SetMemberRights, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
		AddMenuItem(menu, memberLevelStr, "", ITEMDRAW_IGNORE);
		for (int i = 0; i < count; i++)
		{
			if (SQL_FetchRow(hndl))
			{
				SQL_FetchString(hndl, 0, name, sizeof(name));
				SQL_FetchString(hndl, 1, sid, sizeof(sid));
				if (!StrEqual(sid, steamid))
				{ AddMenuItem(menu, sid, name); }
			}
		}
		DisplayMenu(menu, client, 60);
	}
}

public void TSQL_SetMemberRights(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int memberLevel = ReadPackCell(data);
	char steamid[32];
	ReadPackString(data, steamid, sizeof(steamid));
	char name[32];
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	char sid[32];
	bool clientValid = IsClientValid(client);
	char roleName[64];
	if (memberLevel == 2)
	{ Format(roleName, sizeof(roleName), "%T", "sf_role_proadmin", client); }
	else if (memberLevel == 1)
	{ Format(roleName, sizeof(roleName), "%T", "sf_role_admin", client); }
	else
	{ Format(roleName, sizeof(roleName), "%T", "sf_role_member", client); }
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			GetClientAuthId(i, AuthId_Engine, sid, sizeof(sid));
			if (StrEqual(sid, steamid))
			{
				sf_level[i] = memberLevel;
				if (clientValid)
				{ AC_PrintToChat(i, "%t", "sf_setrole_got_client", client, roleName); }
				else
				{ AC_PrintToChat(i, "%t", "sf_setrole_got", roleName); }
				break;
			}
		}
	}
	if (clientValid)
	{ AC_PrintToChat(client, "%t", "sf_setrole_set", name, roleName); }
}

public void TSQL_AddSFMember(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int id = ReadPackCell(data);
	char cName[32];
	ReadPackString(data, cName, sizeof(cName));
	CloseHandle(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			char name[32];
			SQL_FetchString(hndl, 0, name, sizeof(name));
			float tax = SQL_FetchFloat(hndl, 1);
			char itemString[64];
			Handle menu = CreateMenu(menuHandler_ChooseSFAccession, MenuAction_Select | MenuAction_End);
			IntToString(id, itemString, sizeof(itemString));
			AddMenuItem(menu, itemString, "", ITEMDRAW_IGNORE);
			AddMenuItem(menu, name, "", ITEMDRAW_IGNORE);
			FloatToString(tax, itemString, sizeof(itemString));
			AddMenuItem(menu, itemString, "", ITEMDRAW_IGNORE);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_joinquestion", client);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_joinquestion_name", client, name);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_joinquestion_inviter", client, cName);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "No", client);
			AddMenuItem(menu, "n", itemString, ITEMDRAW_DEFAULT);
			Format(itemString, sizeof(itemString), "%T", "Yes", client);
			AddMenuItem(menu, "y", itemString, ITEMDRAW_DEFAULT);
			DisplayMenu(menu, client, 45);
		}
	}
}

public void TSQL_SFMemberAdded(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int id = ReadPackCell(data);
	float tax = ReadPackFloat(data);
	char name[32];
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	if (IsClientValid(client))
	{
		sf_member[client] = id;
		sf_level[client] = 0;
		sf_tax[client] = tax;
		AC_PrintToChatAll("%t", "sf_joined", client, name);
	}
}

public void TSQL_SelectPlayerForPayOff(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		int count = SQL_GetRowCount(hndl);
		char name[32];
		char steamid[32];
		Handle menu = CreateMenu(menuHandler_SelectClientPayOff, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
		SetMenuExitButton(menu, true);
		SetMenuExitBackButton(menu, true);
		SetMenuTitle(menu, "%T", "sfmenu_payout_selectclient", client);
		for (int i = 0; i < count; i++)
		{
			SQL_FetchRow(hndl);
			SQL_FetchString(hndl, 0, steamid, sizeof(steamid));
			SQL_FetchString(hndl, 1, name, sizeof(name));
			AddMenuItem(menu, steamid, name);
		}
		DisplayMenu(menu, client, 45);
	}
}

public void TSQL_CheckSFAccount(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int id = ReadPackCell(data);
	int value = ReadPackCell(data);
	char steamid[32];
	char name[32];
	ReadPackString(data, steamid, sizeof(steamid));
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	bool clientValid = IsClientValid(client);
	if (SQL_FetchRow(hndl))
	{
		if (SQL_FetchFloat(hndl, 0) > 0.0)
		{
			Handle pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(client));
			WritePackCell(pack, value);
			WritePackString(pack, steamid);
			WritePackString(pack, name);
			char query[255];
			Format(query, sizeof(query), "UPDATE `%s` SET account=account-%i WHERE id=%i", sfTableName, value, id);
			SQL_TQuery(db, TSQL_NULL, query);
			Format(query, sizeof(query), "UPDATE `%s` SET points=points+%i WHERE id=%i", tableName, value, id);
			SQL_TQuery(db, TSQL_PayOutDone, query, pack);
		}
		else
		{
			if (clientValid)
			{ AC_PrintToChat(client, "%t", "sfmenu_insufficient", currentName); }
		}
	}
}

public void TSQL_PayOutDone(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int value = ReadPackCell(data);
	char steamid[32];
	char name[32];
	ReadPackString(data, steamid, sizeof(steamid));
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	bool clientValid = IsClientValid(client);
	bool jumpClientMessage = false;
	char sid[32];
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			GetClientAuthId(i, AuthId_Engine, sid, sizeof(sid));
			if (StrEqual(sid, steamid))
			{
				points[i] += float(value);
				if (clientValid)
				{
					if (client == i)
					{ jumpClientMessage = true; }
					AC_PrintToChat(i, "%t", "sf_payout_msg_client", value, currentName, client);
				}
				else
				{
					AC_PrintToChat(i, "%t", "sf_payout_msg", value, currentName);
				}
				break;
			}
		}
	}
	if (!jumpClientMessage)
	{
		if (clientValid)
		{ AC_PrintToChat(client, "%t", "sf_payoutdone", value, currentName, name, steamid); }
	}
}

public void TSQL_SelectMemberCountForRemove(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{		
			int memberCount = SQL_FetchInt(hndl, 0);
			if (memberCount < 3)
			{ AC_PrintToChat(client, "%t", "sfmenu_removeerror_justtwo"); }
			else
			{
				char query[160];
				Format(query, sizeof(query), "SELECT steamid,name FROM `%s` WHERE skillforce=%i AND sflevel<=%i", tableName, sf_member[client], sf_level[client]);
				SQL_TQuery(db, TSQL_SelectRemoveMember, query, GetClientUserId(client));
			}
		}
	}
}

public void TSQL_SelectRemoveMember(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		char steamid[32];
		char sid[32];
		GetClientAuthId(client, AuthId_Engine, sid, sizeof(sid));
		char name[32];
		Handle menu = CreateMenu(menuHandler_SelectRemovingMember, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
		SetMenuExitButton(menu, true);
		SetMenuExitBackButton(menu, true);
		int count = SQL_GetRowCount(hndl);
		for (int i = 0; i < count; i++)
		{
			SQL_FetchRow(hndl);
			SQL_FetchString(hndl, 0, steamid, sizeof(steamid));
			SQL_FetchString(hndl, 1, name, sizeof(name));
			if (!StrEqual(sid, steamid))
			{ AddMenuItem(menu, steamid, name); }
		}
		DisplayMenu(menu, client, 60);
	}
}

public void TSQL_CheckMemberCountForEndRemove(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int id = ReadPackCell(data);
	int userid = ReadPackCell(data);
	int client = GetClientOfUserId(userid);
	bool clientValid = IsClientValid(client);
	char steamid[32];
	char name[32];
	char sfName[32];
	ReadPackString(data, steamid, sizeof(steamid));
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	if (SQL_FetchRow(hndl))
	{
		SQL_FetchString(hndl, 0, sfName, sizeof(sfName));
		int memberCount = SQL_FetchInt(hndl, 1);
		if (memberCount > 2)
		{
			Handle pack = CreateDataPack();
			WritePackCell(pack, userid);
			WritePackString(pack, name);
			WritePackString(pack, sfName);
			char query[255];
			Format(query, sizeof(query), "UPDATE `%s` SET membercount=membercount-1 WHERE id=%i", sfTableName, id);
			SQL_TQuery(db, TSQL_NULL, query);
			Format(query, sizeof(query), "UPDATE `%s` SET skillforce=-1,sfgained=0.0,sflevel=0,sfskill=0,sfskilllevel=0 WHERE steamid='%s'", tableName, steamid);
			SQL_TQuery(db, TSQL_PlayerRemoved, query, pack);
		}
		else
		{
			if (clientValid)
			{ AC_PrintToChat(client, "%t", "sfmenu_removeerror_justtwo"); }
		}
	}
}

public void TSQL_PlayerRemoved(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	char name[32];
	char sfName[32];
	ReadPackString(data, name, sizeof(name));
	ReadPackString(data, sfName, sizeof(sfName));
	if (IsClientValid(client))
	{ AC_PrintToChatAll("%t", "sf_sfremoved", name, sfName); }
	else
	{ AC_PrintToChatAll("%t", "sf_sfremoved_client", client, name, sfName); }
	CloseHandle(data);
}

public void TSQL_GetSkillforceMemberForLeave(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		bool LeaveWouldDeleteSF = false;
		int memberCount = SQL_GetRowCount(hndl);
		if (memberCount < 3)
		{ LeaveWouldDeleteSF = true; }
		if (!LeaveWouldDeleteSF)
		{
			if (sf_level[client] == 2)
			{
				int proAdminCount = 0;
				for (int i = 0; i < memberCount; i++)
				{
					SQL_FetchRow(hndl);
					if (SQL_FetchInt(hndl, 0) == 2)
					{ proAdminCount++; }
				}
				if (proAdminCount == 1)
				{ LeaveWouldDeleteSF = true; }
			}
		}
		Handle menu = CreateMenu(menuHandler_LeaveSF, MenuAction_Select | MenuAction_End);
		SetMenuTitle(menu, "%T", "sfmenu_leavequestion", client);
		char itemString[64];
		if (LeaveWouldDeleteSF)
		{
			Format(itemString, sizeof(itemString), "%T", "sfmenu_leavedelete", client);
			AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
		}
		Format(itemString, sizeof(itemString), "%T", "No", client);
		AddMenuItem(menu, "n", itemString);
		Format(itemString, sizeof(itemString), "%T", "Yes", client);
		AddMenuItem(menu, "y", itemString);
		DisplayMenu(menu, client, 20);
	}
}

public void TSQL_GetSkillforceMemberForLeaveRelay(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			char name[32];
			SQL_FetchString(hndl, 0, name, sizeof(name));
			Handle pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(client));
			WritePackString(pack, name);
			char query[160];
			Format(query, sizeof(query), "SELECT sflevel FROM `%s` WHERE skillforce=%i", tableName, sf_member[client]);
			SQL_TQuery(db, TSQL_GetSFMemberForLeavingProcess, query, pack);
		}
	}
}

public void TSQL_GetSFMemberForLeavingProcess(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	char name[32];
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	if (IsClientValid(client))
	{
		bool LeaveWouldDeleteSF = false;
		int memberCount = SQL_GetRowCount(hndl);
		if (memberCount < 3)
		{ LeaveWouldDeleteSF = true; }
		if (!LeaveWouldDeleteSF)
		{
			if (sf_level[client] == 2)
			{
				int proAdminCount = 0;
				for (int i = 0; i < memberCount; i++)
				{
					SQL_FetchRow(hndl);
					if (SQL_FetchInt(hndl, 0) == 2)
					{ proAdminCount++; }
				}
				if (proAdminCount == 1)
				{ LeaveWouldDeleteSF = true; }
			}
		}
		int id = sf_member[client];
		if (LeaveWouldDeleteSF)
		{
			Handle pack = CreateDataPack();
			WritePackCell(pack, id);
			WritePackCell(pack, GetClientUserId(client));
			WritePackString(pack, name);
			char query[255];
			Format(query, sizeof(query), "DELETE FROM `%s` WHERE id=%i", sfTableName, id);
			SQL_TQuery(db, TSQL_NULL, query);
			Format(query, sizeof(query), "UPDATE `%s` SET skillforce=-1,sfgained=0.0,sflevel=0,sfskill=0,sfskilllevel=0 WHERE skillforce=%i", tableName, id);
			SQL_TQuery(db, TSQL_LeavedSFAndDeleted, query, pack);
		}
		else
		{
			char cName[32];
			GetClientName(client, cName, sizeof(cName));
			Handle pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(client));
			WritePackString(pack, cName);
			WritePackString(pack, name);
			char sid[32];
			GetClientAuthId(client, AuthId_Engine, sid, sizeof(sid));
			char query[255];
			Format(query, sizeof(query), "UPDATE `%s` SET membercount=membercount-1 WHERE id=%i", sfTableName, id);
			SQL_TQuery(db, TSQL_NULL, query);
			Format(query, sizeof(query), "UPDATE `%s` SET skillforce=-1,sfgained=0.0,sflevel=0,sfskill=0,sfskilllevel=0 WHERE steamid='%s'", tableName, sid);
			SQL_TQuery(db, TSQL_LeavedSF, query, pack);
		}
	}
}

public void TSQL_LeavedSFAndDeleted(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int id = ReadPackCell(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	char name[32];
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	for (int i = 1; i <= MaxClients; i++)
	{
		if (sf_member[i] == id)
		{
			sf_member[i] = -1;
			sf_level[i] = 0;
			sf_tax[i] = 0.0;
		}
	}
	if (IsClientValid(client))
	{ AC_PrintToChatAll("%t", "sf_leftsf_del_client", client, name); }
	else
	{ AC_PrintToChatAll("%t", "sf_leftsf_del", name); }
}

public void TSQL_LeavedSF(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	char name[32];
	ReadPackString(data, name, sizeof(name));
	char sfName[32];
	ReadPackString(data, sfName, sizeof(sfName));
	CloseHandle(data);
	if (IsClientValid(client))
	{
		sf_member[client] = -1;
		sf_level[client] = 0;
		sf_tax[client] = 0.0;
	}
	AC_PrintToChatAll("%t", "sf_leftsf", name, sfName);
}

public void TSQL_SetSkillMenuFetchClients(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int skillNumber = ReadPackCell(data);
	CloseHandle(data);
	if (IsClientValid(client))
	{
		int count = SQL_GetRowCount(hndl);
		if (count > 0)
		{
			Handle menu = CreateMenu(menuHandler_SetClientNewSkill, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuTitle(menu, "%T", "sfmenu_chooseskill_client", client);
			char itemString[64];
			IntToString(skillNumber, itemString, sizeof(itemString));
			AddMenuItem(menu, itemString, "", ITEMDRAW_IGNORE);
			char steamid[32];
			char name[32];
			char skillName[32];
			int skill;
			int skilllevel;
			for (int i = 0; i < count; i++)
			{
				SQL_FetchRow(hndl);
				SQL_FetchString(hndl, 0, steamid, sizeof(steamid));
				SQL_FetchString(hndl, 1, name, sizeof(name));
				skill = SQL_FetchInt(hndl, 2);
				skilllevel = SQL_FetchInt(hndl, 3);
				ConvertSkillNumberIntoString(skill, skillName, sizeof(skillName), client);
				AddMenuItem(menu, steamid, name, ITEMDRAW_DEFAULT);
				if (skill < 1)
				{ AddMenuItem(menu, "", skillName, ITEMDRAW_DISABLED); }
				else
				{
					Format(itemString, sizeof(itemString), "%T", "sfmenu_skillline", client, skillName, skilllevel);
					AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
				}
				
			}
			DisplayMenu(menu, client, 60);
		}
	}
}

public void TSQL_NewSkillSet(Handle owner, Handle hndl, const char[] error, any data)
{
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int skill = ReadPackCell(data);
	char steamid[32];
	char name[32];
	ReadPackString(data, steamid, sizeof(steamid));
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	int clientValid = IsClientValid(client);
	char sid[32];
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			GetClientAuthId(i, AuthId_Engine, sid, sizeof(sid));
			if (StrEqual(steamid, sid))
			{
				sf_skill[i] = skill;
				sf_skilllevel[i] = 0;
				char skillName[32];
				ConvertSkillNumberIntoString(skill, skillName, sizeof(skillName), i);
				AC_PrintToChat(i, "%t", "sf_skillchanged", skillName);
				if (clientValid)
				{ if (i == client) { return; } }
				break;
			}
		}
	}
	if (clientValid)
	{
		char skillName[32];
		ConvertSkillNumberIntoString(skill, skillName, sizeof(skillName), client);
		AC_PrintToChat(client, "%t", "sf_skillchanged_setter", name, skillName);
		OpenSkillForceMenu(client);
	}
}

public void TSQL_SFIncreaseLevelList(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		int count = SQL_GetRowCount(hndl);
		if (count > 0)
		{
			char name[32];
			char steamid[32];
			char itemString[64];
			char skillName[32];
			int skill;
			int skilllevel;
			Handle menu = CreateMenu(menuHandler_IncreaseLevelClient, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuExitButton(menu, true);
			SetMenuExitBackButton(menu, true);
			for (int i = 0; i < count; i++)
			{
				SQL_FetchRow(hndl);
				SQL_FetchString(hndl, 0, steamid, sizeof(steamid));
				SQL_FetchString(hndl, 1, name, sizeof(name));
				skill = SQL_FetchInt(hndl, 2);
				skilllevel = SQL_FetchInt(hndl, 3);
				ConvertSkillNumberIntoString(skill, skillName, sizeof(skillName), client);
				AddMenuItem(menu, name, "", ITEMDRAW_IGNORE);
				Format(itemString, sizeof(itemString), "%s (%s)", name, skillName);
				AddMenuItem(menu, steamid, itemString, ITEMDRAW_DEFAULT);
				Format(itemString, sizeof(itemString), "%T", "sfmenu_increaseskill", client, (skilllevel + 1), Pow(sf_level_Base, float(skilllevel + 1)), currentName);
				AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
			}
			DisplayMenu(menu, client, 60);
		}
	}
}

public void TSQL_FetchSFSkillsForInc(Handle owner, Handle hndl, const char[] error, any data)
{
	char name[32];
	char steamid[32];
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	ReadPackString(data, steamid, sizeof(steamid));
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			int skill = SQL_FetchInt(hndl, 0);
			int skilllevel = SQL_FetchInt(hndl, 1);
			float costs = Pow(sf_level_Base, float(skilllevel + 1));
			Handle pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(client));
			WritePackCell(pack, skill);
			WritePackCell(pack, skilllevel);
			WritePackString(pack, steamid);
			WritePackString(pack, name);
			char query[160];
			Format(query, sizeof(query), "SELECT (account-%f) FROM `%s` WHERE id=%i", costs, sfTableName, sf_member[client]);
			SQL_TQuery(db, TSQL_FetchValidAccountFoIncSkill, query, pack);
		}
	}
}

public void TSQL_FetchValidAccountFoIncSkill(Handle owner, Handle hndl, const char[] error, any data)
{
	char name[32];
	char steamid[32];
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int skill = ReadPackCell(data);
	int skilllevel = ReadPackCell(data);
	ReadPackString(data, steamid, sizeof(steamid));
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			float accountDelta = SQL_FetchFloat(hndl, 0);
			if (accountDelta > 0.0)
			{
				Handle pack = CreateDataPack();
				WritePackCell(pack, GetClientUserId(client));
				WritePackCell(pack, skill);
				WritePackCell(pack, skilllevel);
				WritePackString(pack, steamid);
				WritePackString(pack, name);
				char query[160];
				Format(query, sizeof(query), "UPDATE `%s` SET account=account-%f WHERE id=%i", sfTableName, Pow(sf_level_Base, float(skilllevel + 1)), sf_member[client]);
				SQL_TQuery(db, TSQL_NULL, query);
				Format(query, sizeof(query), "UPDATE `%s` SET sfskilllevel=sfskilllevel+1 WHERE steamid='%s'", tableName, steamid);
				SQL_TQuery(db, TSQL_IncSkill, query, pack);
			}
			else
			{ AC_PrintToChat(client, "%t", "sfmenu_insufficient", currentName); }
		}
	}
}

public void TSQL_IncSkill(Handle owner, Handle hndl, const char[] error, any data)
{
	char name[32];
	char steamid[32];
	ResetPack(data);
	int client = GetClientOfUserId(ReadPackCell(data));
	int clientValid = IsClientValid(client);
	int skill = ReadPackCell(data);
	int skilllevel = ReadPackCell(data);
	ReadPackString(data, steamid, sizeof(steamid));
	ReadPackString(data, name, sizeof(name));
	CloseHandle(data);
	char sid[32];
	char skillName[32];
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			GetClientAuthId(i, AuthId_Engine, sid, sizeof(sid));
			if (StrEqual(steamid, sid))
			{
				sf_skilllevel[i]++;
				ConvertSkillNumberIntoString(skill, skillName, sizeof(skillName), i);
				if (clientValid)
				{ AC_PrintToChat(i, "%t", "sf_levelincto", skillName, (skilllevel + 1)); }
				else
				{ AC_PrintToChat(i, "%t", "sf_levelincto_client", client, skillName, (skilllevel + 1)); }
				if (i == client) { return; }
				break;
			}
		}
	}
	if (clientValid)
	{
		OpenSkillForceMenu(client);
		AC_PrintToChat(client, "%t", "sf_levelincreased", name, (skilllevel + 1));
	}
}

public void TSQL_CheckForMemberBorder(Handle owner, Handle hndl, const char[] error, any data)
{
	int client = GetClientOfUserId(data);
	if (IsClientValid(client))
	{
		if (SQL_FetchRow(hndl))
		{
			int mCount = SQL_FetchInt(hndl, 0);
			if (mCount >= sf_maximumMemberCount)
			{
				AC_PrintToChat(client, "%t", "sf_toomuchmember");
				OpenSkillForceMenu(client);
			}
			else
			{
				Handle cMenu = CreateMenu(menuHandler_AddSFMember, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
				char uid[32];
				char name[32];
				for (int i = 1; i <= MaxClients; i++)
				{
					if (IsClientValid(i) && i != client)
					{
						if (sf_member[i] == -1)
						{
							int userid = GetClientUserId(i);
							IntToString(userid, uid, sizeof(uid));
							GetClientName(i, name, sizeof(name));
							AddMenuItem(cMenu, uid, name);
						}
					}
				}
				DisplayMenu(cMenu, client, 60);
			}
		}
	}
}

public void TSQL_NULL(Handle owner, Handle hndl, const char[] error, any data) { }