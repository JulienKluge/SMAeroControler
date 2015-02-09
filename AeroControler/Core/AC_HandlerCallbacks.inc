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

public Action:timer_Introduce(Handle:timer, any:client)
{
	if (IntroduceMe) //Double save when the variable is set into the 90seconds to 0.0
	{
		IncludeTag = false;
		AC_PrintToChatAll("%t", "introduceplugin", PLUGIN_VERSION, PLUGIN_AUTHOR);
		new String:dateToCheck[7];
		FormatTime(dateToCheck, sizeof(dateToCheck), "%d%m");
		if (StrEqual(dateToCheck, "3001", true))
		{
			new String:actualYear[6];
			FormatTime(actualYear, sizeof(actualYear), "%Y");
			new actYear = StringToInt(actualYear);
			if (actYear > 0)
			{
				new age = actYear - 1995;
				AC_PrintToChatAll("%t", "introduceplugin_bday", "_AeonOne_", age);
			}
		}
	}
	IncludeTag = true;
	return Plugin_Stop;
}

public Action:timer_disableTopC(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	c_topCDisableTimer[handleIndex] = INVALID_HANDLE;
	if (!InWork) { return Plugin_Stop; }
	new client = GetClientOfUserId(ReadPackCell(pack));
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

public Action:timer_PrepareSpawn(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
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

public Action:timer_CheckForDiceMenu(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
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

public Action:timer_NotifyOfStates(Handle:timer)
{
	if (!InWork) { return Plugin_Continue; }
	new notifyClients[MAXPLAYERS + 1]; new notifyCount = 0;
	new victimClients[MAXPLAYERS + 1]; new victimCount = 0;
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			if (IsPlayerAlive(i))
			{
				new teamIndex = GetClientTeam(i);
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
		new Float:pos[3];
		for (new i = 0; i < victimCount; i++)
		{
			new client = victimClients[i];
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
				new refuseCount = c_r[client] - 1;
				for (new j = 0; j < c_rBaseMax[client]; j++)
				{
					if (refuseCount >= j)
					{
						TE_SetupBeamRingPoint(pos, 40.0, 20.0, g_BeamSprite, g_HaloSprite, 0, 10, 1.0, 2.0, 0.0, { 0, 0, 255, 255 }, 10, 0);
						TE_Send(notifyClients, notifyCount);
					}
					else
					{
						new Float:secondsUntil = GetGameTime() - c_rTimeStamp[client];
						new Float:lifeTime = 1.0;
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

public Action:timer_ReAllowDrop(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	c_dropHandler[handleIndex] = INVALID_HANDLE;
	new client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{ c_IsDropDenied[client] = false; }
	return Plugin_Handled;
}

public Action:timer_Announce(Handle:timer)
{
	new bool:cmds = (announce_cmds && Cmd_Alw_Commands);
	new bool:rules = (announce_rules && Cmd_Alw_Rules);
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

public _:menuHandler_DiceMenu(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		if (!InWork) { return 0; }
		decl String:info[5];
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
		decl String:display[32];
		decl String:displayTranslated[32];
		GetMenuItem(menu, param2, "", _:0, _, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, param1);
		return RedrawMenuItem(displayTranslated);
	}
	else if (action == MenuAction_Display)
	{
		decl String:display[32];
		decl String:displayTranslated[32];
		GetMenuTitle(menu, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, param1);
		SetPanelTitle(Handle:param2, displayTranslated);
	}
	return 0;
}

public _:menuHandler_AboutMenu(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_DisplayItem)
	{
		decl String:info[5];
		decl String:display[64];
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
		new extCount = GetArraySize(extensionPlugins);
		if (extCount > 0)
		{
			new Handle:extMenu = CreateMenu(menuHandler_DisposeMenu, MenuAction_End);
			decl String:pName[64];
			decl String:subStr[64];
			for (new i = 0; i < extCount; i += 4)
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

public menuHandler_Cmds(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:cmd[64];
		GetMenuItem(menu, param2, cmd, sizeof(cmd));
		if (strlen(cmd) > 1)
		{
			FakeClientCommand(param1, cmd);
		}
	}
	else if (action == MenuAction_End)
	{ CloseHandle(menu); }
}

public menuHandler_RuleMenu(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Select)
	{
		decl String:info[64];
		decl String:display[64];
		GetMenuItem(menu, param2, info, sizeof(info), _, display, sizeof(display));
		if (strlen(info) > 0)
		{
			if (strlen(info) > 1)
			{
				if (info[0] == '3')
				{
					decl String:parts[2][64]; //<----
					if (ExplodeString(info, "\x01", parts, 2, 64) > 1)
					{
						ShowMOTDPanel(param1, display, parts[1], MOTDPANEL_TYPE_URL);
					}
				}
				else if (info[0] == '4')
				{
					decl String:sub_parts[2][64]; //that is pretty strange, it's seems, that the part variable some lines below is in the same code scope so that i cannot name it "parts"...
					if (ExplodeString(info, "\x01", sub_parts, 2, 64) > 1)
					{
						new arrayIndex = StringToInt(sub_parts[1]);
						new Handle:sMenu = Handle:GetArrayCell(ruleSubMenus, arrayIndex);
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

public menuHandler_SubRuleMenus(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_Cancel)
	{
		if (param2 == MenuCancel_ExitBack)
		{ DisplayMenu(ruleMenu, param1, 40); }
	}
}

public menuHandler_DisposeMenu(Handle:menu, MenuAction:action, param1, param2)
{
	if (action == MenuAction_End)
	{
		CloseHandle(menu);
	}
}

/* ***********************************************************************MENUHANDLER*********************************************************************** */




/* ********************************************************************SDKHOOKSCALLBACKS******************************************************************** */

public Action:SDKH_OnCanUseWeapon(client, weapon)
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

public Action:SDKH_OnTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype, &weapon, Float:damageForce[3], Float:damagePosition[3]) //Make us backwards until 2.0+
{
	if (!InWork) { return Plugin_Continue; }
	new Action:returnResult = Plugin_Continue;
	new Float:newDamage = Diced_SDKH_TakeDmg(victim, attacker, damage);
	if (newDamage != damage)
	{
		damage = newDamage;
		returnResult = Plugin_Changed;
	}
	return returnResult;
}

/* ********************************************************************SDKHOOKSCALLBACKS******************************************************************** */




/* *********************************************************************FILTER CALLBACKS******************************************************************** */

public bool:Filter_FilterOutData(ent, contentMask, any:data)
{
	if (ent == data) { return false; }
	return true;
}

/* *********************************************************************FILTER CALLBACKS******************************************************************** */