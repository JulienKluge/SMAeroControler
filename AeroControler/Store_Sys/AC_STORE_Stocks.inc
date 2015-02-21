/*
**
** Aero Jail Controler Plugin
** Store System - partial stocks file
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

stock void LoadClient(int client, const char[] auth)
{
	points[client] = 0.0;
	c_MultiplierPrivate[client] = c_MultiplierPublic[client] = 1.0;
	c_PressureNadeCount[client] = sf_skill[client] = sf_skilllevel[client] = 0;
	c_InSFNameChooseProcess[client] = sf_hasSkill[client] = false;
	int userid = GetClientUserId(client);
	char query[256];
	Format(query, sizeof(query), "SELECT points,DATEDIFF(NOW(),lastjoin),skillforce,sflevel,sfskill,sfskilllevel FROM `%s` WHERE steamid='%s'", tableName, auth);
	SQL_TQuery(db, TSQL_SelectJoiner, query, userid);
}

stock void UpdateClientAsync(int client)
{
	if (!InWork) { return; }
	if (IsClientReplay(client) || IsClientSourceTV(client) || IsFakeClient(client)) { return; }
	if (newPoints[client] == 0.0) { return; }
	float updatePoints = newPoints[client] * c_MultiplierPrivate[client] * c_MultiplierPublic[client] * p_PointsMultiplierPublic;
	float updateSFPoints = 0.0;
	if (sf_member[client] != -1)
	{
		updateSFPoints = updatePoints * sf_tax[client];
		updatePoints = updatePoints * (1.0 - sf_tax[client]);
	}
	char id[32];
	GetClientAuthId(client, AuthId_Engine, id, sizeof(id));
	//GetClientAuthString(client, id, sizeof(id));
	char query[256];
	Format(query, sizeof(query), "UPDATE `%s` SET points=points+%f,gained=gained+%f,sfgained=sfgained+%f WHERE steamid='%s'", tableName, updatePoints, updatePoints, updateSFPoints, id);
	SQL_TQuery(db, TSQL_NULL, query, 0, DBPrio_Low);
	if (updateSFPoints > 0.0)
	{
		Format(query, sizeof(query), "UPDATE `%s` SET account=account+%f,gained=gained+%f WHERE id=%i", sfTableName, updateSFPoints, updateSFPoints, sf_member[client]);
		SQL_TQuery(db, TSQL_NULL, query, 0, DBPrio_Low);
	}
	points[client] += updatePoints;
	newPoints[client] = 0.0;
	#if defined DEBUG
	PrintToServer("Updated Client %N: +%f (+%f)", client, updatePoints, updateSFPoints);
	#endif
}

stock void GainPoints(int client, float pointsToGain)
{
	if (!InWork) { return; }
	#if defined DEBUG
	tCount = ctCount = 1;
	#endif
	newPoints[client] += pointsToGain;
	virtualNewPoints[client] += pointsToGain;
}

stock void OpenStoreMenu(int client)
{
	if (!InWork) { return; }
	int teamIndex = GetClientTeam(client);
	if (teamIndex == CS_TEAM_T || teamIndex == CS_TEAM_CT) //failsafe
	{
		char subStr[64];
		Handle menu = CreateMenu(menuHandler_Store, MenuAction_Select | MenuAction_End);
		SetMenuTitle(menu, "%T", "storemenu_title", client, points[client], currentName);
		Format(subStr, sizeof(subStr), "%T", "storemenu_cat_jail", client);
		AddMenuItem(menu, "jail", subStr, (teamIndex == CS_TEAM_T) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
		Format(subStr, sizeof(subStr), "%T", "storemenu_cat_props", client);
		AddMenuItem(menu, "prop", subStr, ITEMDRAW_DEFAULT);
		Format(subStr, sizeof(subStr), "%T", "storemenu_cat_items", client);
		AddMenuItem(menu, "item", subStr, ITEMDRAW_DEFAULT);
		Format(subStr, sizeof(subStr), "%T", "storemenu_cat_misc", client);
		AddMenuItem(menu, "misc", subStr, ITEMDRAW_DEFAULT);
		DisplayMenu(menu, client, 45);
	}
}

stock float DecodePrice(int code)
{
	switch (code)
	{
		case 1: { return s_j_dice; }				//1
		case 2: { return s_j_refuse; }			//2
		case 3: { return s_j_capitulate; }		//3
		case 4: { return s_j_diceFive; }			//4
		case 5: { return s_j_dice_map; }			//5
		case 6: { return s_j_refuse_map; }		//6
		case 7: { return s_j_capitulate_map; }	//7
		case 8: { return s_p_gravity; }	 		//8
		case 9: { return s_p_speed; }				//9
		case 10: { return s_p_hp; }				//10
		case 11: { return s_p_invisfive; }		//11
		case 12: { return s_p_invisten; }			//12
		case 13: { return s_i_smoke; }			//13
		case 14: { return s_i_flash; }			//14
		case 15: { return s_i_armor; }			//15
		case 16: { return s_i_nv; }				//16
		case 17: { return s_i_pressurehe; }		//17
		case 18: { return s_m_respawn; }			//18
		case 19: { return s_m_threepointgain; }	//19
		case 20: { return s_m_enemypingpulse; }	//20
		case 21: { return s_m_enemyskin; }		//21
		default: { Log(EL_Error, "STORE", "An hard error was fetched. This isn't your fault but please send the author this error. (1_CO:%i)", code); return -1.0; }
	}
	return -1.0; //year...the compiler cries when it isn't there...
}

stock void ApplyCodeFeature(int client, int code)
{
	switch (code)
	{
		case 1: { StoreFeature_DicePlus(client); }
		case 2: { StoreFeature_RefusePlus(client);  }
		case 3: { StoreFeature_CapitulatePlus(client);  }
		case 4: { StoreFeature_FiveDices(client);  }
		case 5: { StoreFeature_DicePlusMap(client);  }
		case 6: { StoreFeature_RefusePlusMap(client);  }
		case 7: { StoreFeature_CapitulatePlusMap(client);  }
		case 8: { StoreFeature_LessGravity(client);  }
		case 9: { StoreFeature_MoreSpeed(client);  }
		case 10: { StoreFeature_MoreHP(client);  }
		case 11: { StoreFeature_FiveSecInvisible(client);  }
		case 12: { StoreFeature_TenSecInvisible(client);  }
		case 13: { StoreFeature_Smoke(client);  }
		case 14: { StoreFeature_Flash(client);  }
		case 15: { StoreFeature_Armor(client);  }
		case 16: { StoreFeature_NV(client);  }
		case 17: { StoreFeature_PressureHE(client);  }
		case 18: { StoreFeature_Respawn(client);  }
		case 19: { StoreFeature_MultiPointGain(client);  }
		case 20: { StoreFeature_EnemyPingPulse(client);  }
		case 21: { StoreFeature_EnemyModel(client);  }
		default: { Log(EL_Error, "STORE", "An hard error was fetched. This isn't your fault but please send the author this error. (2_CO:%i_CL:%i)", code, client); }
	}
}

stock void DescreasePoints(int client, float amount)
{
	points[client] -= amount;
	char query[192];
	char id[32];
	GetClientAuthId(client, AuthId_Engine, id, sizeof(id));
	//GetClientAuthString(client, id, sizeof(id));
	Format(query, sizeof(query), "UPDATE `%s` SET points=points-%f,spend=spend+%f WHERE steamid='%s'", tableName, amount, amount, id);
	SQL_TQuery(db, TSQL_NULL, query);
}

stock bool CheckStoreFeatureForClient(int client, int code)
{
	int teamIndex = GetClientTeam(client);
	if (!(teamIndex == CS_TEAM_T || teamIndex == CS_TEAM_CT))
	{ return false; }
	if (code > 0 && code < 8)
	{
		if (teamIndex == CS_TEAM_CT)
		{ return false; }
	}
	return true;
}

stock void OpenSkillForceMenu(int client)
{
	if (sf_member[client] == -1)
	{
		char subStr[64];
		Handle menu = CreateMenu(menuHandler_SFMenu, MenuAction_Select | MenuAction_End);
		SetMenuTitle(menu, "%T", "sfmenu_nosf", client);
		Format(subStr, sizeof(subStr), "%T", "sfmenu_foundsf", client);
		AddMenuItem(menu, "found", subStr);
		Format(subStr, sizeof(subStr), "%T", "sfmenu_ranksfs", client);
		AddMenuItem(menu, "ranks", subStr);
		Format(subStr, sizeof(subStr), "%T", "sfmenu_rankplayers", client);
		AddMenuItem(menu, "rankp", subStr);
		DisplayMenu(menu, client, 60);
	}
	else
	{
		int userid = GetClientUserId(client);
		char query[255];
		Format(query, sizeof(query), "SELECT name FROM `%s` WHERE id=%i", sfTableName, sf_member[client]);
		SQL_TQuery(db, TSQL_OpenSFMenu, query, userid);
	}
}

stock void OpenRekursivePayOutMenu(int client, int value, const char[] steamid, const char[] name)
{
	if (value < 1) { value = 1; }
	if (value > 100000) { value = 100000; }
	Handle menu = CreateMenu(menuHandler_RekursivePayOutMenu, MenuAction_Select | MenuAction_End);
	AddMenuItem(menu, steamid, "", ITEMDRAW_IGNORE);
	AddMenuItem(menu, name, "", ITEMDRAW_IGNORE);
	char itemString[64];
	IntToString(value, itemString, sizeof(itemString));
	AddMenuItem(menu, itemString, "", ITEMDRAW_IGNORE);
	AddMenuItem(menu, "ptnd", "+1000");
	AddMenuItem(menu, "phnd", "+100");
	AddMenuItem(menu, "pten", "+10");
	AddMenuItem(menu, "pone", "+1");
	Format(itemString, sizeof(itemString), "%T", "sfmenu_payout_amount", client, value, currentName);
	AddMenuItem(menu, "okay", itemString);
	AddMenuItem(menu, "mone", "-1");
	AddMenuItem(menu, "mten", "-10");
	AddMenuItem(menu, "mhnd", "-100");
	AddMenuItem(menu, "mtnd", "-1000");
	DisplayMenu(menu, client, 45);
}

stock void ConvertSkillNumberIntoString(int skill, char[] str, int strlength, int clientToTranslate = LANG_SERVER)
{
	if (skill == Skill_Medic)
	{ Format(str, strlength, "%T", "sf_skill_medic", clientToTranslate); }
	else if (skill == Skill_Fighter)
	{ Format(str, strlength, "%T", "sf_skill_fighter", clientToTranslate); }
	else if (skill == Skill_Defender)
	{ Format(str, strlength, "%T", "sf_skill_defender", clientToTranslate); }
	else if (skill == Skill_Crook)
	{ Format(str, strlength, "%T", "sf_skill_crook", clientToTranslate); }
	else
	{ Format(str, strlength, "%T", "sf_skill_none", clientToTranslate); }
}

stock int GetClientAimingClient(int client)
{
	int target = -1;
	float pos[3]; float ang[3];
	GetClientEyePosition(client, pos);
	GetClientEyeAngles(client, ang);
	Handle tr = TR_TraceRayFilterEx(pos, ang, MASK_SOLID, RayType_Infinite, Filter_FilterOutData, client);
	if (TR_DidHit(tr))
	{
		int ent = TR_GetEntityIndex(tr);
		if (IsClientValid(ent))
		{
			target = ent;
		}
	}
	CloseHandle(tr);
	return target;
}