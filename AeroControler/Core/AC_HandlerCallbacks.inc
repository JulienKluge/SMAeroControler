/*
**
** Aero Jail Controler Plugin - Handler/Callbacks partial File
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


/* **************************************************************************TIMER************************************************************************** */

public Action timer_Introduce(Handle timer, any client)
{
	if (IntroduceMe) //Double save when the variable is set into the 90seconds to 0.0
	{
		IncludeTag = false;
		AC_PrintToChatAll("%t", "introduceplugin", PLUGIN_VERSION, PLUGIN_AUTHOR);
		char dateToCheck[7];
		FormatTime(dateToCheck, sizeof(dateToCheck), "%d%m");
		if (StrEqual(dateToCheck, "3001", true))
		{
			char actualYear[6];
			FormatTime(actualYear, sizeof(actualYear), "%Y");
			int actYear = StringToInt(actualYear);
			if (actYear > 0)
			{
				int age = actYear - 1995;
				AC_PrintToChatAll("%t", "introduceplugin_bday", "_AeonOne_", age);
			}
		}
	}
	IncludeTag = true;
	return Plugin_Stop;
}

public Action timer_disableTopC(Handle timer, Handle pack)
{
	ResetPack(pack);
	int handleIndex = ReadPackCell(pack);
	c_topCDisableTimer[handleIndex] = INVALID_HANDLE;
	if (!InWork) { return Plugin_Stop; }
	int client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			c_topC[client] = false;
			GivePlayerItem(client, "weapon_knife");
			if (InWork) { AC_PrintToChatAll("%t", "capitulating_top_end", client); }
		}
	}
	return Plugin_Stop;
}

public Action timer_PrepareSpawn(Handle timer, Handle pack)
{
	ResetPack(pack);
	int handleIndex = ReadPackCell(pack);
	int client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client) && InWork)
	{
		if (IsPlayerAlive(client))
		{
			if (c_DiceState[client] == 0)
			{
				c_DiceMenu[handleIndex] = INVALID_HANDLE;
				if (autoshow_dicemenu) { DisplayDiceMenu(client); }
				if (announce_dice)
				{ AC_PrintToChat(client, "%t", "announce_dice", Cmd_PStr_Dice, Cmd_PStr_DiceMenu); }
				return Plugin_Stop;
			}
			else if (c_DiceState[client] == 1)
			{
				ThrowDiceChecked(client, true);
			}
		}
	}
	c_DiceMenu[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}

public Action timer_CheckForDiceMenu(Handle timer, Handle pack)
{
	ResetPack(pack);
	int handleIndex = ReadPackCell(pack);
	int client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client) && InWork)
	{
		if (IsPlayerAlive(client))
		{
			if (GetClientMenu(client) != MenuSource_None)
			{
				return Plugin_Continue;
			}
			DisplayMenu(diceMenu, client, 10);
		}
	}
	c_DiceMenu[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}

public Action timer_NotifyOfStates(Handle timer)
{
	if (!InWork) { return Plugin_Continue; }
	int notifyClients[MAXPLAYERS + 1]; int notifyCount = 0;
	int victimClients[MAXPLAYERS + 1]; int victimCount = 0;
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			if (IsPlayerAlive(i))
			{
				int teamIndex = GetClientTeam(i);
				if (teamIndex == CS_TEAM_CT)
				{
					if (StateNotifyCounter > 0 || c_StateNotifyOverride[i])
					{
						notifyClients[notifyCount] = i;
						notifyCount++;
					}
					PrintStateString(i, CS_TEAM_CT);
				}
				else if (teamIndex == CS_TEAM_T)
				{
					victimClients[victimCount] = i;
					victimCount++;
					PrintStateString(i, CS_TEAM_T);
				}
			}
		}
	}
	if (StateNotifyCounter > 0) { StateNotifyCounter--; }
	if (notifyCount > 0 && victimCount > 0)
	{
		//I love this piece of code :D
		float pos[3];
		for (int i = 0; i < victimCount; i++)
		{
			int client = victimClients[i];
			GetClientAbsOrigin(client, pos);
			if (c_c[client] < c_cBase[client])
			{
				pos[2] += 35.0;
				TE_SetupBeamRingPoint(pos, 40.0, 20.0, g_BeamSprite, g_HaloSprite, 0, 10, 1.0, 2.0, 0.0, { 255, 255, 255, 255 }, 10, 0);
				TE_Send(notifyClients, notifyCount);
			}
			else
			{
				pos[2] += 20.0;
				int refuseCount = c_r[client] - 1;
				for (int j = 0; j < c_rBaseMax[client]; j++)
				{
					if (refuseCount >= j)
					{
						TE_SetupBeamRingPoint(pos, 40.0, 20.0, g_BeamSprite, g_HaloSprite, 0, 10, 1.0, 2.0, 0.0, { 0, 0, 255, 255 }, 10, 0);
						TE_Send(notifyClients, notifyCount);
					}
					else
					{
						float secondsUntil = GetGameTime() - c_rTimeStamp[client];
						float lifeTime = 1.0;
						if (secondsUntil < 35.0) { lifeTime = 0.5; }
						TE_SetupBeamRingPoint(pos, 40.0, 20.0, g_BeamSprite, g_HaloSprite, 0, 10, lifeTime, 2.0, 0.0, { 128, 192, 255, 255 }, 10, 0);
						TE_Send(notifyClients, notifyCount);
					}
					pos[2] += 10.0;
				}
				if (c_rebell[client])
				{
					TE_SetupBeamRingPoint(pos, 40.0, 20.0, g_BeamSprite, g_HaloSprite, 0, 10, 1.0, 2.0, 0.0, { 255, 0, 0, 255 }, 10, 0);
					TE_Send(notifyClients, notifyCount);
				}
			}
		}
	}
	return Plugin_Continue;
}

public Action timer_ReAllowDrop(Handle timer, Handle pack)
{
	ResetPack(pack);
	int handleIndex = ReadPackCell(pack);
	c_dropHandler[handleIndex] = INVALID_HANDLE;
	int client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{ c_IsDropDenied[client] = false; }
	return Plugin_Handled;
}

public Action timer_Announce(Handle timer)
{
	bool cmds = (announce_cmds && Cmd_Alw_Commands);
	bool rules = (announce_rules && Cmd_Alw_Rules);
	if (rules && cmds)
	{ AC_PrintToChatAll("%t", "announce_rulescmds", Cmd_PStr_Rules, Cmd_PStr_Commands); }
	else
	{
		if (cmds)
		{ AC_PrintToChatAll("%t", "announce_cmds", Cmd_PStr_Commands); }
		if (rules)
		{ AC_PrintToChatAll("%t", "announce_rules", Cmd_PStr_Rules); }
	}
	return Plugin_Continue;
}

/* **************************************************************************TIMER************************************************************************** */




/* ***********************************************************************MENUHANDLER*********************************************************************** */

public int menuHandler_DiceMenu(Handle menu, MenuAction action, int param1, int param2)
{
	if (action == MenuAction_Select)
	{
		if (!InWork) { return 0; }
		char info[5];
		GetMenuItem(menu, param2, info, sizeof(info));
		if (StrEqual(info, "dice"))
		{
			c_DiceState[param1] = 0;
			ThrowDiceChecked(param1);
		}
		else if (StrEqual(info, "nodc")) //yea, at first i've also thought this is useless
		{
			c_DiceState[param1] = 0; //but that is what the user expecting ;D
		}
		else if (StrEqual(info, "awdc"))
		{
			c_DiceState[param1] = 1;
			ThrowDiceChecked(param1, true); //suppress when failed because there wasn't an explicit dice command
		}
		else if (StrEqual(info, "nvdc"))
		{
			c_DiceState[param1] = 2;
		}
	}
	else if (action == MenuAction_DisplayItem)
	{
		char display[32];
		char displayTranslated[32];
		GetMenuItem(menu, param2, "", _:0, _, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, param1);
		return RedrawMenuItem(displayTranslated);
	}
	else if (action == MenuAction_Display)
	{
		char display[32];
		char displayTranslated[32];
		GetMenuTitle(menu, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, param1);
		SetPanelTitle(view_as<Handle>param2, displayTranslated);
	}
	return 0;
}

public int menuHandler_AboutMenu(Handle menu, MenuAction action, int param1, int param2)
{
	if (action == MenuAction_DisplayItem)
	{
		char info[5];
		char display[64];
		GetMenuItem(menu, param2, info, sizeof(info));
		if (StrEqual(info, "auth"))
		{
			Format(display, sizeof(display), "%T", "about_author", param1, "_AeonOne_", "Julien Kluge");
		}
		else if (StrEqual(info, "vers"))
		{
			Format(display, sizeof(display), "%T", "about_version", param1, PLUGIN_VERSION);
		}
		else if (StrEqual(info, "cont"))
		{
			Format(display, sizeof(display), "%T", "about_contact", param1, "Julien.Kluge@gmail.com");
		}
		else if (StrEqual(info, "smvs"))
		{
			Format(display, sizeof(display), "Sourcemod %s", SOURCEMOD_VERSION);
		}
		else if (StrEqual(info, "exts"))
		{
			Format(display, sizeof(display), "%i %T", (GetArraySize(extensionPlugins) / 4), "menu_pluginextensions", param1);
		}
		return RedrawMenuItem(display);
	}
	else if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ DisplayMenu(ruleMenu, param1, 40); }
	}
	else if (action == MenuAction_Select)
	{
		int extCount = GetArraySize(extensionPlugins);
		if (extCount > 0)
		{
			Handle extMenu = CreateMenu(menuHandler_DisposeMenu, MenuAction_End);
			char pName[64];
			char subStr[64];
			for (int i = 0; i < extCount; i += 4)
			{
				GetArrayString(extensionPlugins, i, pName, sizeof(pName));
				if (strlen(pName) > 1)
				{
					AddMenuItem(extMenu, "", pName, ITEMDRAW_DISABLED);
					GetArrayString(extensionPlugins, i + 1, pName, sizeof(pName));
					Format(subStr, sizeof(subStr), "%T: %s", "menu_author", param1, pName);
					AddMenuItem(extMenu, "", subStr, ITEMDRAW_DISABLED);
					GetArrayString(extensionPlugins, i + 2, pName, sizeof(pName));
					Format(subStr, sizeof(subStr), "%T: %s", "menu_version", param1, pName);
					AddMenuItem(extMenu, "", subStr, ITEMDRAW_DISABLED);
					GetArrayString(extensionPlugins, i + 3, pName, sizeof(pName));
					AddMenuItem(extMenu, "", pName, ITEMDRAW_DISABLED);
				}
			}
			SetMenuExitButton(extMenu, true);
			DisplayMenu(extMenu, param1, 30);
		}
	}
	return 0;
}

public int menuHandler_Cmds(Handle menu, MenuAction action, int param1, int param2)
{
	if (action == MenuAction_Select)
	{
		char cmd[64];
		GetMenuItem(menu, param2, cmd, sizeof(cmd));
		if (strlen(cmd) > 1)
		{
			FakeClientCommand(param1, cmd);
		}
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public int menuHandler_RuleMenu(Handle menu, MenuAction action, int param1, int param2)
{
	if (action == MenuAction_Select)
	{
		char info[64];
		char display[64];
		GetMenuItem(menu, param2, info, sizeof(info), _, display, sizeof(display));
		if (strlen(info) > 0)
		{
			if (strlen(info) > 1)
			{
				if (info[0] == '3')
				{
					char parts[2][64]; //<----
					if (ExplodeString(info, "\x01", parts, 2, 64) > 1)
					{
						ShowMOTDPanel(param1, display, parts[1], MOTDPANEL_TYPE_URL);
					}
				}
				else if (info[0] == '4')
				{
					char sub_parts[2][64]; //that is pretty strange, it's seems, that the part variable some lines below is in the same code scope so that i cannot name it "parts"...
					if (ExplodeString(info, "\x01", sub_parts, 2, 64) > 1)
					{
						int arrayIndex = StringToInt(sub_parts[1]);
						Handle sMenu = view_as<Handle>GetArrayCell(ruleSubMenus, arrayIndex);
						DisplayMenu(sMenu, param1, 60);
					}
				}
			}
			else
			{
				if (StrEqual(info, "1"))
				{
					DisplayMenu(aboutBackMenu, param1, 60);
				}
				else if (StrEqual(info, "2"))
				{
					CreateAndDisplayCmdMenu(param1, 60);
				}
			}
		}
	}
	return;
}

public int menuHandler_SubRuleMenus(Handle menu, MenuAction action, int param1, int param2)
{
	if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ DisplayMenu(ruleMenu, param1, 40); }
	}
}

public int menuHandler_DisposeMenu(Handle menu, MenuAction action, int param1, int param2)
{
	if (action == MenuAction_End)
	{
		CloseHandle(menu);
	}
}

/* ***********************************************************************MENUHANDLER*********************************************************************** */




/* ********************************************************************SDKHOOKSCALLBACKS******************************************************************** */

public Action SDKH_OnCanUseWeapon(int client, int weapon)
{
	if (!InWork) { return Plugin_Continue; }
	if (GetClientTeam(client) == CS_TEAM_T)
	{
		if (c_topC[client])
		{
			return Plugin_Handled;
		}
	}
	if (!Diced_CanUseWeapon_Detour(client, weapon))
	{
		return Plugin_Handled;
	}
	return Plugin_Continue;
}

public Action SDKH_OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3]) //Make us backwards until 2.0+
{
	if (!InWork) { return Plugin_Continue; }
	Action returnResult = Plugin_Continue;
	float newDamage = Diced_SDKH_TakeDmg(victim, attacker, damage);
	if (newDamage != damage)
	{
		damage = newDamage;
		returnResult = Plugin_Changed;
	}
	return returnResult;
}

/* ********************************************************************SDKHOOKSCALLBACKS******************************************************************** */




/* *********************************************************************FILTER CALLBACKS******************************************************************** */

public bool Filter_FilterOutData(int ent, int contentMask, any data)
{
	if (ent == data) { return false; }
	return true;
}

/* *********************************************************************FILTER CALLBACKS******************************************************************** */