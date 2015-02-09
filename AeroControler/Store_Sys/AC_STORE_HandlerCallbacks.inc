/*
**
** Aero Jail Controler Plugin
** Store System - partial handler, callbacks file
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

public AddStoreCmdsToCmdMenu(client, Handle:menu)
{
	decl String:subStr[32];
	decl String:itemString[64];
	if (Cmd_Store)
	{
		GetFirstDelimitedCmd(Cmd_Str_Store, subStr);
		Format(itemString, sizeof(itemString), "%s - %T", Cmd_PStr_Store, "sf_cmd_store", client);
		AddMenuItem(menu, subStr, itemString, (IsPlayerAlive(client)) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	}
	if (Cmd_StoreTop)
	{
		GetFirstDelimitedCmd(Cmd_Str_StoreTop, subStr);
		Format(itemString, sizeof(itemString), "%s - %T", Cmd_PStr_StoreTop, "sf_cmd_storetop", client);
		AddMenuItem(menu, subStr, itemString, ITEMDRAW_DEFAULT);
	}
	if (Cmd_StoreRank)
	{
		GetFirstDelimitedCmd(Cmd_Str_StoreRank, subStr);
		Format(itemString, sizeof(itemString), "%s - %T", Cmd_PStr_StoreRank, "sf_cmd_storerank", client);
		AddMenuItem(menu, subStr, itemString, ITEMDRAW_DEFAULT);
	}
	if (Cmd_SF)
	{
		GetFirstDelimitedCmd(Cmd_Str_SF, subStr);
		Format(itemString, sizeof(itemString), "%s - %T", Cmd_PStr_SF, "sf_cmd_sf", client);
		AddMenuItem(menu, subStr, itemString, ITEMDRAW_DEFAULT);
	}
	if (Cmd_SFC)
	{
		Format(itemString, sizeof(itemString), "%s - %T", Cmd_PStr_SFC, "sf_cmd_sfc", client);
		AddMenuItem(menu, "", itemString, ITEMDRAW_DISABLED);
	}
}

/* MENU HANDLER */
public menuHandler_Store(Handle:menu, MenuAction:action, param1, param2)
{ //HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING, HARDCODING
	if (action == MenuAction_Select)
	{
		if (!InWork) { return; }
		if (!IsPlayerAlive(param1)) { AC_PrintToChat(param1, "%t", "storemenu_notalive"); }
		decl String:info[8];
		GetMenuItem(menu, param2, info, sizeof(info));
		new Float:account = points[param1];
		if (StrEqual(info, "jail", true))
		{
			decl String:subStr[64];
			new Handle:nMenu = CreateMenu(menuHandler_StoreEnd, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuTitle(nMenu, "%T", "storemenu_subtitle", param1, "storemenu_cat_jail", param1, account, currentName);
			SetMenuExitButton(nMenu, true);
			SetMenuExitBackButton(nMenu, true);
			if (s_j_dice != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_j_dice, currentName, "storemenu_j_1", param1);
			AddMenuItem(nMenu, "1", subStr, (account >= s_j_dice) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			new val_one;
			new val_two;
			ac_getClientRefuseVars(param1, val_one, val_two);
			if (s_j_refuse != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_j_refuse, currentName, "storemenu_j_2", param1);
			AddMenuItem(nMenu, "2", subStr, ((account >= s_j_refuse) && (val_one == val_two)) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			ac_getClientCapitulateVars(param1, val_one, val_two);
			if (s_j_capitulate != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_j_capitulate, currentName, "storemenu_j_3", param1);
			AddMenuItem(nMenu, "3", subStr, ((account >= s_j_capitulate) && (val_one == val_two)) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_j_diceFive != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_j_diceFive, currentName, "storemenu_j_4", param1);
			AddMenuItem(nMenu, "4", subStr, (account >= s_j_diceFive) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_j_dice_map != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_j_dice_map, currentName, "storemenu_j_5", param1);
			AddMenuItem(nMenu, "5", subStr, (account >= s_j_dice_map) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_j_refuse_map != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_j_refuse_map, currentName, "storemenu_j_6", param1);
			AddMenuItem(nMenu, "6", subStr, (account >= s_j_refuse_map) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_j_capitulate_map != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_j_capitulate_map, currentName, "storemenu_j_7", param1);
			AddMenuItem(nMenu, "7", subStr, (account >= s_j_capitulate_map) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			DisplayMenu(nMenu, param1, 45);
		}
		else if (StrEqual(info, "prop", true))
		{
			decl String:subStr[64];
			new Handle:nMenu = CreateMenu(menuHandler_StoreEnd, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuTitle(nMenu, "%T", "storemenu_subtitle", param1, "storemenu_cat_props", param1, account, currentName);
			SetMenuExitButton(nMenu, true);
			SetMenuExitBackButton(nMenu, true);
			if (s_p_gravity != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_p_gravity, currentName, "storemenu_p_8", param1);
			AddMenuItem(nMenu, "8", subStr, (account >= s_p_gravity) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_p_speed != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_p_speed, currentName, "storemenu_p_9", param1);
			AddMenuItem(nMenu, "9", subStr, (account >= s_p_speed) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_p_hp != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_p_hp, currentName, "storemenu_p_10", param1);
			AddMenuItem(nMenu, "10", subStr, (account >= s_p_hp) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_p_invisfive != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_p_invisfive, currentName, "storemenu_p_11", param1);
			AddMenuItem(nMenu, "11", subStr, (account >= s_p_invisfive) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_p_invisten != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_p_invisten, currentName, "storemenu_p_12", param1);
			AddMenuItem(nMenu, "12", subStr, (account >= s_p_invisten) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			DisplayMenu(nMenu, param1, 45);
		}
		else if (StrEqual(info, "item", true))
		{
			decl String:subStr[64];
			new Handle:nMenu = CreateMenu(menuHandler_StoreEnd, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuTitle(nMenu, "%T", "storemenu_subtitle", param1, "storemenu_cat_items", param1, account, currentName);
			SetMenuExitButton(nMenu, true);
			SetMenuExitBackButton(nMenu, true);
			if (s_i_smoke != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_i_smoke, currentName, "storemenu_i_13", param1);
			AddMenuItem(nMenu, "13", subStr, (account >= s_i_smoke) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_i_flash != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_i_flash, currentName, "storemenu_i_14", param1);
			AddMenuItem(nMenu, "14", subStr, (account >= s_i_flash) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_i_armor != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_i_armor, currentName, "storemenu_i_15", param1);
			AddMenuItem(nMenu, "15", subStr, (account >= s_i_armor) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_i_nv != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_i_nv, currentName, "storemenu_i_16", param1);
			AddMenuItem(nMenu, "16", subStr, (account >= s_i_nv) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_i_pressurehe != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_i_pressurehe, currentName, "storemenu_i_17", param1);
			AddMenuItem(nMenu, "17", subStr, (account >= s_i_pressurehe) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			DisplayMenu(nMenu, param1, 45);
		}
		else if (StrEqual(info, "misc", true))
		{
			decl String:subStr[64];
			new Handle:nMenu = CreateMenu(menuHandler_StoreEnd, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuTitle(nMenu, "%T", "storemenu_subtitle", param1, "storemenu_cat_misc", param1, account, currentName);
			SetMenuExitButton(nMenu, true);
			SetMenuExitBackButton(nMenu, true);
			if (s_m_respawn != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_m_respawn, currentName, "storemenu_m_18", param1);
			AddMenuItem(nMenu, "18", subStr, (account >= s_m_respawn && (!c_HasRespawn[param1])) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_m_threepointgain != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_m_threepointgain, currentName, "storemenu_m_19", param1);
			AddMenuItem(nMenu, "19", subStr, (account >= s_m_threepointgain) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_m_enemypingpulse != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_m_enemypingpulse, currentName, "storemenu_m_20", param1);
			AddMenuItem(nMenu, "20", subStr, (account >= s_m_enemypingpulse) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			if (s_m_enemyskin != -1) {
			Format(subStr, sizeof(subStr), "%0.1f %s: %T", s_m_enemyskin, currentName, "storemenu_m_21", param1);
			AddMenuItem(nMenu, "21", subStr, (account >= s_m_enemyskin) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); }
			DisplayMenu(nMenu, param1, 45);
		}
	}
	else if (action == MenuAction_End)
	{
		CloseHandle(menu);
	}
}

public menuHandler_StoreEnd(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:codeStr[4];
		GetMenuItem(menu, param2, codeStr, sizeof(codeStr));
		new code = StringToInt(codeStr);
		new Float:price = DecodePrice(code);
		if (price == -1.0) { return; }
		decl String:query[160];
		decl String:id[32];
		GetClientAuthId(param1, AuthId_Engine, id, sizeof(id));
		Format(query, sizeof(query), "SELECT points FROM `%s` WHERE steamid='%s'", tableName, id);
		new Handle:pack = CreateDataPack();
		WritePackCell(pack, GetClientUserId(param1));
		WritePackFloat(pack, price);
		WritePackCell(pack, code);
		SQL_TQuery(db, TSQL_CheckValidTransferLimit, query, pack);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenStoreMenu(param1); }
	}
	else if (action == MenuAction_End)
	{
		CloseHandle(menu);
	}
}

public menuHandler_SFMenu(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[8];
		GetMenuItem(menu, param2, info, sizeof(info));
		if (StrEqual(info, "found")) //Found SF
		{
			c_InSFNameChooseProcess[param1] = true;
			AC_PrintToChat(param1, "%t", "sf_foundsf_nameprocess");
		}
		else if (StrEqual(info, "sfrce")) //my skillforce menu
		{
			decl String:query[160];
			Format(query, sizeof(query), "SELECT name,membercount,account,gained,tax FROM `%s` WHERE id=%i", sfTableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_MySFRelay, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "mates")) //member list
		{
			decl String:query[192];
			Format(query, sizeof(query), "SELECT name,sflevel,sfskill,sfskilllevel FROM `%s` WHERE skillforce=%i ORDER BY sflevel DESC, sfgained DESC", tableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_SFMemberList, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "mbrnk")) //member ranks
		{
			decl String:query[192];
			Format(query, sizeof(query), "SELECT name,sfgained FROM `%s` WHERE skillforce=%i ORDER BY sfgained DESC, sflevel ASC", tableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_SFMemberRanks, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "addmm")) //add member
		{
			decl String:query[127];
			Format(query, sizeof(query), "SELECT membercount FROM `%s` WHERE id=%i", sfTableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_CheckForMemberBorder, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "rmvmm")) //remove member
		{
			decl String:query[127];
			Format(query, sizeof(query), "SELECT membercount FROM `%s` WHERE id=%i", sfTableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_SelectMemberCountForRemove, query, GetClientUserId(param1));
			
		}
		else if (StrEqual(info, "payof")) //pay off
		{
			decl String:query[127];
			Format(query, sizeof(query), "SELECT steamid,name FROM `%s` WHERE skillforce=%i", tableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_SelectPlayerForPayOff, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "mrght")) //set member rights
		{
			decl String:itemString[64];
			new Handle:cMenu = CreateMenu(menuHandler_SetSFRights, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_setrole_member", param1);
			AddMenuItem(cMenu, "mem", itemString);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_setrole_admin", param1);
			AddMenuItem(cMenu, "adm", itemString);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_setrole_proadmin", param1);
			AddMenuItem(cMenu, "ldr", itemString);
			DisplayMenu(cMenu, param1, 45);
		}
		else if (StrEqual(info, "cngtx")) //change taxes
		{
			new Handle:cMenu = CreateMenu(menuHandler_ChangeSFTaxes, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuTitle(cMenu, "%T", "sfmenu_changetaxes", param1);
			decl String:infoTax[16];
			decl String:itemString[64];
			for (new i = 0; i <= 20; i++)
			{
				new percent = i * 5;
				new Float:percentFloat = float(percent) / 100.0;
				FloatToString(percentFloat, infoTax, sizeof(infoTax));
				Format(itemString, sizeof(itemString), "%i %%", percent);
				AddMenuItem(cMenu, infoTax, itemString, (sf_tax[param1] == percentFloat) ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);
			}
			DisplayMenu(cMenu, param1, 60);
		}
		else if (StrEqual(info, "ranks")) //Top ranking of the SFs
		{
			decl String:query[160];
			Format(query, sizeof(query), "SELECT name,account,gained FROM `%s` ORDER BY (account+gained) DESC LIMIT 20", sfTableName);
			SQL_TQuery(db, TSQL_TopSFs, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "rankp")) //Top ranking of the SF-players
		{
			decl String:query[576]; //a hyper query, needs a hyper string length
			Format(query, sizeof(query), "SELECT `%s`.name,`%s`.sfgained,`%s`.name FROM `%s`,`%s` WHERE `%s`.skillforce=`%s`.id ORDER BY sfgained DESC LIMIT 10", tableName, tableName, sfTableName, tableName, sfTableName, tableName, sfTableName); //at this point I thought: well, to allow setting the tablename was a bad idea...
			SQL_TQuery(db, TSQL_TopSFPlayers, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "setsk")) //set skill
		{
			new Handle:cMenu = CreateMenu(menuHandler_ChooseSkillToSet, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuTitle(cMenu, "%T", "sfmenu_chooseskill_title", param1);
			decl String:itemString[64];
			Format(itemString, sizeof(itemString), "%T", "sf_skill_medic", param1);
			AddMenuItem(cMenu, "medi", itemString);
			Format(itemString, sizeof(itemString), "%T", "sf_skill_medic_expl", param1, Skill_Medic_HPPerLevel);
			AddMenuItem(cMenu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sf_skill_fighter", param1);
			AddMenuItem(cMenu, "figh", itemString);
			Format(itemString, sizeof(itemString), "%T", "sf_skill_fighter_expl", param1, (Skill_Fighter_PercentPerLevel * 100.0));
			AddMenuItem(cMenu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sf_skill_defender", param1);
			AddMenuItem(cMenu, "defe", itemString);
			Format(itemString, sizeof(itemString), "%T", "sf_skill_defender_expl", param1, (Skill_Defender_PercentPerLevel * 100.0));
			AddMenuItem(cMenu, "defe", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sf_skill_crook", param1);
			AddMenuItem(cMenu, "crok", itemString);
			Format(itemString, sizeof(itemString), "%T", "sf_skill_crook_expl", param1, (Skill_Crook_PercentPerLevel / 10.0));
			AddMenuItem(cMenu, "crok", itemString, ITEMDRAW_DISABLED);
			DisplayMenu(cMenu, param1, 45);
		}
		else if (StrEqual(info, "sklvl")) //increase skill-level
		{
			decl String:query[192];
			Format(query, sizeof(query), "SELECT steamid,name,sfskill,sfskilllevel FROM `%s` WHERE skillforce=%i ORDER BY sfskilllevel ASC", tableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_SFIncreaseLevelList, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "leave")) //leave skillforce
		{
			decl String:query[160];
			Format(query, sizeof(query), "SELECT sflevel FROM `%s` WHERE skillforce=%i", tableName, sf_member[param1]); //with this query, we can check: 1st membercount & 2nd how many have proadmin-rights
			SQL_TQuery(db, TSQL_GetSkillforceMemberForLeave, query, GetClientUserId(param1));
		}
		else if (StrEqual(info, "delet")) //delete skillforce
		{
			new Handle:cMenu = CreateMenu(menuHandler_DeleteSF, MenuAction_Select | MenuAction_Cancel | MenuAction_End);
			SetMenuTitle(cMenu, "%T", "sfmenu_suredelete", param1);
			decl String:itemString[8];
			Format(itemString, sizeof(itemString), "%T", "No", param1);
			AddMenuItem(cMenu, "n", itemString);
			Format(itemString, sizeof(itemString), "%T", "Yes", param1);
			AddMenuItem(cMenu, "y", itemString);
			SetMenuExitButton(cMenu, true);
			SetMenuExitBackButton(cMenu, true);
			DisplayMenu(cMenu, param1, 20);
		}
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_BackToSfMenu(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_ChooseCoFounder(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:uid[32];
		GetMenuItem(menu, param2, uid, sizeof(uid));
		new target = GetClientOfUserId(StringToInt(uid));
		if (IsClientValid(target))
		{
			decl String:packHandleString[32];
			GetMenuItem(menu, 0, packHandleString, sizeof(packHandleString));
			new Handle:subPack = Handle:StringToInt(packHandleString);
			ResetPack(subPack);
			decl String:name[32];
			new client = GetClientOfUserId(ReadPackCell(subPack));
			ReadPackString(subPack, name, sizeof(name)); /* We'll copy the informations to another datapacket, because this one is bound in this menu */
			new Handle:pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(client));
			WritePackString(pack, name);
			IntToString(_:pack, packHandleString, sizeof(packHandleString));
			new Handle:chooseMenu = CreateMenu(menuHandler_ChooseSFFoundership, MenuAction_Select | MenuAction_End);
			SetMenuTitle(chooseMenu, "%T", "sfmenu_foundsf_cfchoose", target);
			AddMenuItem(chooseMenu, packHandleString, "", ITEMDRAW_IGNORE); //tricky way to store data in a menu ^^
			decl String:itemString[64];
			Format(itemString, sizeof(itemString), "%T", "sfmenu_foundsf_cfco", target, param1);
			AddMenuItem(chooseMenu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "sfmenu_foundsf_cfsfname", target, name);
			AddMenuItem(chooseMenu, "", itemString, ITEMDRAW_DISABLED);
			Format(itemString, sizeof(itemString), "%T", "Yes", target);
			AddMenuItem(chooseMenu, "y", itemString);
			Format(itemString, sizeof(itemString), "%T", "No", target);
			AddMenuItem(chooseMenu, "n", itemString);
			DisplayMenu(chooseMenu, target, 30);
		}
	}
	else if (action == MenuAction_End)
	{
		decl String:packHandleString[32];
		GetMenuItem(menu, 0, packHandleString, sizeof(packHandleString));
		new Handle:pack = Handle:StringToInt(packHandleString);
		CloseHandle(pack);
		CloseHandle(menu);
	}
}

public menuHandler_ChooseSFFoundership(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[4];
		GetMenuItem(menu, param2, info, sizeof(info));
		decl String:packHandleString[32];
		GetMenuItem(menu, 0, packHandleString, sizeof(packHandleString));
		new Handle:pack = Handle:StringToInt(packHandleString);
		decl String:sfName[32];
		ResetPack(pack);
		new founder1 = GetClientOfUserId(ReadPackCell(pack));
		ReadPackString(pack, sfName, sizeof(sfName));
		if (StrEqual(info, "y"))
		{
			if (IsClientValid(founder1) && (sf_member[founder1] == -1 && sf_member[param1] == -1))
			{
				decl String:id[32];
				new Handle:dPack = CreateDataPack();
				WritePackCell(dPack, GetClientUserId(founder1));
				WritePackCell(dPack, GetClientUserId(param1));
				GetClientAuthId(founder1, AuthId_Engine, id, sizeof(id));
				WritePackString(dPack, id);
				GetClientAuthId(param1, AuthId_Engine, id, sizeof(id));
				WritePackString(dPack, id);
				WritePackString(dPack, sfName);
				decl String:query[192];
				Format(query, sizeof(query), "INSERT INTO `%s`(name,membercount,account,gained,tax) VALUES ('%s',2,0.0,0.0,0.2)", sfTableName, sfName);
				SQL_TQuery(db, TSQL_AccomplishSFFounding, query, dPack);
			}
		}
		else if (StrEqual(info, "n"))
		{ AC_PrintToChat(founder1, "%t", "sf_clientreject", param1); }
	}
	else if (action == MenuAction_End)
	{
		decl String:packHandleString[32];
		GetMenuItem(menu, 0, packHandleString, sizeof(packHandleString));
		new Handle:pack = Handle:StringToInt(packHandleString);
		CloseHandle(pack);
		CloseHandle(menu);
	}
}

public menuHandler_DeleteSF(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:answer[8];
		GetMenuItem(menu, param2, answer, sizeof(answer));
		if (StrEqual(answer, "y"))
		{
			decl String:query[128];
			Format(query, sizeof(query), "SELECT name FROM `%s` WHERE id=%i", sfTableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_DeletedSFGetName, query, GetClientUserId(param1));
		}
		else if (StrEqual(answer, "n"))
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_ChangeSFTaxes(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[16];
		GetMenuItem(menu, param2, info, sizeof(info));
		new Float:tax = StringToFloat(info);
		new Handle:pack = CreateDataPack();
		WritePackCell(pack, GetClientUserId(param1));
		WritePackCell(pack, sf_member[param1]);
		WritePackFloat(pack, tax);
		decl String:query[160];
		Format(query, sizeof(query), "UPDATE `%s` SET tax=%f WHERE id=%i", sfTableName, tax, sf_member[param1]);
		SQL_TQuery(db, TSQL_SFTaxesChanged, query, pack);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_SetSFRights(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[8];
		GetMenuItem(menu, param2, info, sizeof(info));
		new memberLevel;
		if (StrEqual(info, "mem"))
		{ memberLevel = 0; }
		else if (StrEqual(info, "adm"))
		{ memberLevel = 1; }
		else if (StrEqual(info, "ldr"))
		{ memberLevel = 2; }
		decl String:query[160];
		Format(query, sizeof(query), "SELECT name,steamid FROM `%s` WHERE skillforce=%i", tableName, sf_member[param1]);
		new Handle:pack = CreateDataPack();
		WritePackCell(pack, GetClientUserId(param1));
		WritePackCell(pack, memberLevel);
		SQL_TQuery(db, TSQL_SetSFRightsMenu, query, pack);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_SetMemberRights(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:name[32];
		decl String:steamid[32];
		decl String:memberLevelStr[8];
		GetMenuItem(menu, param2, steamid, sizeof(steamid), _, name, sizeof(name));
		GetMenuItem(menu, 0, memberLevelStr, sizeof(memberLevelStr));
		new memberLevel = StringToInt(memberLevelStr);
		new Handle:pack = CreateDataPack();
		WritePackCell(pack, GetClientUserId(param1));
		WritePackCell(pack, memberLevel);
		WritePackString(pack, steamid);
		WritePackString(pack, name);
		decl String:query[160];
		Format(query, sizeof(query), "UPDATE `%s` SET sflevel=%i WHERE steamid='%s'", tableName, memberLevel, steamid);
		SQL_TQuery(db, TSQL_SetMemberRights, query, pack);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_AddSFMember(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		new String:info[32];
		GetMenuItem(menu, param2, info, sizeof(info));
		new client = GetClientOfUserId(StringToInt(info));
		if (IsClientValid(client))
		{
			new Handle:pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(client));
			WritePackCell(pack, sf_member[param1]);
			decl String:cName[32];
			GetClientName(param1, cName, sizeof(cName));
			WritePackString(pack, cName);
			decl String:query[255];
			Format(query, sizeof(query), "SELECT name,tax FROM `%s` WHERE id=%i", sfTableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_AddSFMember, query, pack);
		}
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_ChooseSFAccession(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[8];
		GetMenuItem(menu, param2, info, sizeof(info));
		if (StrEqual(info, "y"))
		{
			decl String:subStr[32];
			decl String:name[32];
			GetMenuItem(menu, 0, subStr, sizeof(subStr));
			new id = StringToInt(subStr);
			GetMenuItem(menu, 1, name, sizeof(name));
			GetMenuItem(menu, 2, subStr, sizeof(subStr));
			new Float:tax = StringToFloat(subStr);
			new Handle:pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(param1));
			WritePackCell(pack, id);
			WritePackFloat(pack, tax);
			WritePackString(pack, name);
			decl String:steamid[32];
			GetClientAuthId(param1, AuthId_Engine, steamid, sizeof(steamid));
			decl String:query[255];
			Format(query, sizeof(query), "UPDATE `%s` SET membercount=membercount+1 WHERE id=%i", sfTableName, id);
			SQL_TQuery(db, TSQL_NULL, query);
			Format(query, sizeof(query), "UPDATE `%s` SET skillforce=%i,sfgained=0.0,sflevel=0,sfskill=0,sfskilllevel=0 WHERE steamid='%s'", tableName, id, steamid);
			SQL_TQuery(db, TSQL_SFMemberAdded, query, pack);
		}
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_SelectClientPayOff(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:steamid[32];
		decl String:name[32];
		GetMenuItem(menu, param2, steamid, sizeof(steamid), _, name, sizeof(name));
		OpenRekursivePayOutMenu(param1, 5, steamid, name);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_RekursivePayOutMenu(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:steamid[32];
		decl String:name[32];
		decl String:valueStr[32];
		GetMenuItem(menu, 0, steamid, sizeof(steamid));
		GetMenuItem(menu, 1, name, sizeof(name));
		GetMenuItem(menu, 2, valueStr, sizeof(valueStr));
		new value = StringToInt(valueStr);
		decl String:info[8];
		GetMenuItem(menu, param2, info, sizeof(info));
		if (StrEqual(info, "okay"))
		{
			new Handle:pack = CreateDataPack();
			WritePackCell(pack, GetClientUserId(param1));
			WritePackCell(pack, sf_member[param1]);
			WritePackCell(pack, value);
			WritePackString(pack, steamid);
			WritePackString(pack, name);
			decl String:query[255];
			Format(query, sizeof(query), "SELECT (account-%i) FROM `%s` WHERE id=%i", value, sfTableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_CheckSFAccount, query, pack);
		}
		else
		{
			if (StrEqual(info, "ptnd"))
			{ value += 1000;}
			else if (StrEqual(info, "phnd"))
			{ value += 100;}
			else if (StrEqual(info, "pten"))
			{ value += 10;}
			else if (StrEqual(info, "pone"))
			{ value += 1;}
			else if (StrEqual(info, "mone"))
			{ value -= 1;}
			else if (StrEqual(info, "mten"))
			{ value -= 10;}
			else if (StrEqual(info, "mhnd"))
			{ value -= 100;}
			else if (StrEqual(info, "mtnd"))
			{ value -= 1000;}
			OpenRekursivePayOutMenu(param1, value, steamid, name);
		}
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_SelectRemovingMember(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:steamid[32];
		decl String:name[32];
		GetMenuItem(menu, param2, steamid, sizeof(steamid), _, name, sizeof(name));
		new Handle:cMenu = CreateMenu(menuHandler_RemoveConfirm, MenuAction_Select | MenuAction_End);
		SetMenuTitle(cMenu, "%T", "sfmenu_remove_confirm", param1, name);
		AddMenuItem(cMenu, steamid, "", ITEMDRAW_IGNORE);
		AddMenuItem(cMenu, name, "", ITEMDRAW_IGNORE);
		decl String:itemString[32];
		Format(itemString, sizeof(itemString), "%T", "No", param1);
		AddMenuItem(cMenu, "n", itemString);
		Format(itemString, sizeof(itemString), "%T", "Yes", param1);
		AddMenuItem(cMenu, "y", itemString);
		DisplayMenu(cMenu, param1, 20);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_RemoveConfirm(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:steamid[32];
		decl String:name[32];
		decl String:info[32];
		GetMenuItem(menu, 0, steamid, sizeof(steamid));
		GetMenuItem(menu, 0, name, sizeof(name));
		GetMenuItem(menu, param2, info, sizeof(info));
		if (StrEqual(info, "y"))
		{
			new Handle:pack = CreateDataPack();
			WritePackCell(pack, sf_member[param1]);
			WritePackCell(pack, GetClientUserId(param1));
			WritePackString(pack, steamid);
			WritePackString(pack, name);
			decl String:query[127];
			Format(query, sizeof(query), "SELECT name,membercount FROM `%s` WHERE id=%i", sfTableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_CheckMemberCountForEndRemove, query, pack);
		}
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_LeaveSF(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[8];
		GetMenuItem(menu, param2, info, sizeof(info));
		if (StrEqual(info, "y"))
		{
			decl String:query[160];
			Format(query, sizeof(query), "SELECT name FROM `%s` WHERE id=%i", sfTableName, sf_member[param1]);
			SQL_TQuery(db, TSQL_GetSkillforceMemberForLeaveRelay, query, GetClientUserId(param1));
		}
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_ChooseSkillToSet(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[8];
		GetMenuItem(menu, param2, info, sizeof(info));
		new skillNumber = 0;
		if (StrEqual(info, "medi"))
		{ skillNumber = Skill_Medic; }
		else if (StrEqual(info, "figh"))
		{ skillNumber = Skill_Fighter; }
		else if (StrEqual(info, "defe"))
		{ skillNumber = Skill_Defender; }
		else if (StrEqual(info, "crok"))
		{ skillNumber = Skill_Crook; }
		new Handle:pack = CreateDataPack();
		WritePackCell(pack, GetClientUserId(param1));
		WritePackCell(pack, skillNumber);
		decl String:query[160];
		Format(query, sizeof(query), "SELECT steamid,name,sfskill,sfskilllevel FROM `%s` WHERE skillforce=%i", tableName, sf_member[param1]);
		SQL_TQuery(db, TSQL_SetSkillMenuFetchClients, query, pack);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_SetClientNewSkill(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[32];
		GetMenuItem(menu, 0, info, sizeof(info));
		new skill = StringToInt(info);
		decl String:name[32];
		GetMenuItem(menu, param2, info, sizeof(info), _, name, sizeof(name));
		new Handle:pack = CreateDataPack();
		WritePackCell(pack, GetClientUserId(param1));
		WritePackCell(pack, skill);
		WritePackString(pack, info);
		WritePackString(pack, name);
		decl String:query[160];
		Format(query, sizeof(query), "UPDATE `%s` SET sfskill=%i,sfskilllevel=0 WHERE steamid='%s'", tableName, skill, info);
		SQL_TQuery(db, TSQL_NewSkillSet, query, pack);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_IncreaseLevelClient(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:name[32];
		decl String:steamid[32];
		GetMenuItem(menu, param2, steamid, sizeof(steamid));
		GetMenuItem(menu, param2 - 1, name, sizeof(name));
		new Handle:pack = CreateDataPack();
		WritePackCell(pack, GetClientUserId(param1));
		WritePackString(pack, steamid);
		WritePackString(pack, name);
		decl String:query[160];
		Format(query, sizeof(query), "SELECT sfskill,sfskilllevel FROM `%s` WHERE steamid='%s'", tableName, steamid);
		SQL_TQuery(db, TSQL_FetchSFSkillsForInc, query, pack);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ OpenSkillForceMenu(param1); }
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_CloseHandleOnEnd(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_End)
	{ CloseHandle(menu); }
}
/* MENU HANDLER */

/* TIMER */
public Action:timer_RespawnClient(Handle:timer, any:data)
{
	ResetPack(Handle:data);
	new clientIndex = ReadPackCell(data);
	new client = GetClientOfUserId(ReadPackCell(data));
	CloseHandle(data);
	c_RespawnTimer[clientIndex] = INVALID_HANDLE;
	c_HasRespawn[clientIndex] = false;
	if (IsClientValid(client))
	{
		CS_RespawnPlayer(client);
		AC_PrintToChatAll("%t", "store_respawnpublic", client);
	}
	return Plugin_Stop;
}

public Action:timer_InvisTimer(Handle:timer, any:data)
{
	ResetPack(Handle:data);
	new clientIndex = ReadPackCell(data);
	new client = GetClientOfUserId(ReadPackCell(data));
	c_InvisibleSeconds[clientIndex]--;
	if (c_InvisibleSeconds[clientIndex] == 0 || (!IsClientValid(client)))
	{
		//CloseHandle(data); TIMER_DATA_HNDL didn't it?
		c_InvisCountTimer[clientIndex] = INVALID_HANDLE;
		if (IsClientValid(client))
		{ SetEntityRenderMode(client, RENDER_NORMAL); }
		return Plugin_Stop;
	}
	return Plugin_Continue;
}

public Action:timer_CheckSkill(Handle:timer)
{
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			if (sf_hasSkill[i])
			{
				if (sf_skill[i] == Skill_Medic)
				{
					if (c_MedicSkillAmount[i] > 0)
					{
						new buttons = GetClientButtons(i);
						if (buttons & IN_USE)
						{
							new target = GetClientAimingClient(i);
							if (target != -1)
							{
								if (GetClientTeam(i) == GetClientTeam(target))
								{
									new targetHealth = GetClientHealth(target);
									new hpToAdd = 100 - targetHealth;
									if (hpToAdd > 0)
									{
										if (hpToAdd > c_MedicSkillAmount[i]) //what a FUCKING HUGE if-tree
										{ hpToAdd = c_MedicSkillAmount[i]; }
										c_MedicSkillAmount[i] -= hpToAdd;
										SetEntityHealth(target, targetHealth + hpToAdd);
										new Float:pos_one[3];
										new Float:pos_two[3];
										GetClientAbsOrigin(i, pos_one);
										GetClientAbsOrigin(target, pos_two);
										pos_one[2] += 40.0;
										pos_two[2] += 40.0;
										new clients[2]; clients[0] = target; clients[1] = i;
										TE_SetupBeamPoints(pos_one, pos_two, g_BeamSprite, g_HaloSprite, 0, 10, 1.5, 2.0, 2.0, 1, 0.0, { 0, 128, 0, 255 }, 10);
										TE_Send(clients, 2, 0.0);
										TE_SetupBeamRingPoint(pos_two, 150.0, 20.0, g_BeamSprite, g_HaloSprite, 0, 10, 1.5, 2.0, 0.0, { 0, 128, 0, 255 }, 10, 0);
										TE_Send(clients, 2, 0.0);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return Plugin_Continue;
}

/* TIMER */

/* SDK HOOKS */

public Action:SDKH_OnTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype, &weapon, Float:damageForce[3], Float:damagePosition[3]) //Make us backwards compatible until 2.0+
{
	new Action:result = Plugin_Continue;
	new bool:victimValid = IsClientValid(victim);
	if (victimValid)
	{
		if (sf_hasSkill[victim])
		{
			if (sf_skill[victim] == Skill_Defender)
			{
				damage = damage * (1.0 - (float(sf_skilllevel[victim]) * Skill_Defender_PercentPerLevel)); //equation: 1-(n*p)/100
				result = Plugin_Changed;
			}
		}
	}
	if (IsClientValid(attacker))
	{
		if (sf_hasSkill[attacker])
		{
			if (sf_skill[attacker] == Skill_Fighter)
			{
				damage = damage * (1.0 + (float(sf_skilllevel[victim]) * Skill_Fighter_PercentPerLevel)); //equation: 1+(n*p)/100
				result = Plugin_Changed;
			}
			else if (sf_skill[attacker] == Skill_Crook)
			{
				if (victimValid)
				{
					new wpn = GetEntPropEnt(attacker, Prop_Send, "m_hActiveWeapon"); //Because the wepon entity fails!
					if (IsValidEntity(wpn))
					{
						decl String:className[32];
						GetEntityClassname(wpn, className, sizeof(className));
						if (StrEqual(className, "weapon_knife"))
						{
							if (GetRandomInt(1, 1000) <= (sf_skilllevel[attacker] * Skill_Crook_PercentPerLevel))
							{
								new ent = GetEntPropEnt(victim, Prop_Send, "m_hActiveWeapon");
								if (ent != -1)
								{ CS_DropWeapon(victim, ent, true); }
							}
						}
					}
				}
			}
		}
	}
	return result;
}

/* SDK HOOKS */

/* FILTER */

public bool:Filter_FilterOutData(ent, contentMask, any:data)
{
	if (ent == data) { return false; }
	return true;
}

/* FILTER */