/*
**
** Aero Jail Controler Plugin - Stocks partial File
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

stock RespawnClientVars(client) //Used to set the variables of a client new. Used in player spawn. 
{
	c_r[client] = c_rBase[client] + c_rSubBase[client];
	c_rBaseMax[client] = c_r[client];
	c_rSubBase[client] = 0;
	c_c[client] = c_cBase[client] + c_cSubBase[client];
	c_cBaseMax[client] = c_c[client];
	c_cSubBase[client] = 0;
	c_dices[client] = c_dicesBase[client] + c_dicesSubBase[client];
	c_dicesSubBase[client] = 0;
	c_topC[client] = c_cReal[client] = c_rebell[client] = c_IsDropDenied[client] = false;
	c_StateNotifyOverride[client] = false;
	c_rTimeStamp[client] = 0.0;
	Diced_ResetClientVars_Detour(client);
	SetEntityRenderColor(client, 255, 255, 255, 255);
	SetEntityRenderFx(client, RENDERFX_NONE);
	SetEntityRenderMode(client, RENDER_NORMAL);
	SetClientDefaultValues(client, true, -1, 1.0, 1.0);
}

stock ResetClientVars(client) //Used to set the variables of a client to default. Used in player connect.
{
	c_r[client] = c_c[client] = c_dices[client] = c_rBase[client] = c_cBase[client] = c_dicesBase[client] = 1;
	c_rSubBase[client] = c_cSubBase[client] = c_dicesSubBase[client] = c_DiceState[client] = 0;
	c_topC[client] = c_rebell[client] = c_cReal[client] = c_StateNotifyOverride[client] = c_IsDropDenied[client] = false;
}

stock UpdateClientColor(client, bool:KeepAlpha = true)
{
	if(!InWork) { return; }
	new r = 255; new g = 255; new b = 255; new a = 255;
	if (KeepAlpha)
	{ a = GetClientAlphaRenderColor(client); }
	if (c_cReal[client])
	{ r = 48; g = 48; b = 48; }
	else if (c_r[client] < c_rBaseMax[client])
	{
		r = 0;
		g = 128;
		b = 191 + RoundFloat(63.0 - ((float(c_r[client]) / float(c_rBaseMax[client])) * 64.0)); //year the difference is tiny, but there is one xD
		if (c_rebell[client])
		{ r = 255; g = 32; }
	} 
	else if (c_rebell[client])
	{ r = 255; g = 0; b = 0; }
	SetEntityRenderColor(client, r, g, b, a);
}

stock SetupExplosion(Float:pos[3], damage, radius = 0, guiltyClient = -1, guiltyTeamOverride = -1)
{
	new ent = CreateEntityByName("env_explosion");
	DispatchKeyValue(ent, "spawnflags", "1088"); //no sound, no dlights
	DispatchSpawn(ent);
	SetEntProp(ent, Prop_Data, "m_iMagnitude", damage);
	SetEntProp(ent, Prop_Data, "m_iRadiusOverride", radius);
	if (guiltyClient != -1)
	{
		if (IsClientValid(guiltyClient))
		{
			SetEntPropEnt(ent, Prop_Send, "m_hOwnerEntity", guiltyClient);
			SetEntProp(ent, Prop_Send, "m_iTeamNum", GetClientTeam(guiltyClient));
		}
	}
	if (guiltyTeamOverride > 0) { SetEntProp(ent, Prop_Send, "m_iTeamNum", guiltyTeamOverride); }
	TeleportEntity(ent, pos, NULL_VECTOR, NULL_VECTOR);
	AcceptEntityInput(ent, "Explode");
	EmitRandomExplosionSound(pos);
	AcceptEntityInput(ent, "kill");
}

stock EmitRandomExplosionSound(Float:pos[3])
{
	new rand = GetRandomInt(0, 8);
	switch (rand)
	{
		case 0: { EmitAmbientSound("ambient/explosions/explode_1.wav", pos);  }
		case 1: { EmitAmbientSound("ambient/explosions/explode_2.wav", pos); }
		case 2: { EmitAmbientSound("ambient/explosions/explode_4.wav", pos); }
		case 3: { EmitAmbientSound("weapons/explode3.wav", pos); }
		case 4: { EmitAmbientSound("weapons/explode4.wav", pos); }
		case 5: { EmitAmbientSound("weapons/explode5.wav", pos); }
		case 6: { EmitAmbientSound("weapons/hegrenade/explode3.wav", pos); }
		case 7: { EmitAmbientSound("weapons/hegrenade/explode4.wav", pos); }
		case 8: { EmitAmbientSound("weapons/hegrenade/explode5.wav", pos); }
	}
}

stock DisplayDiceMenu(client)
{
	if (GetClientMenu(client) == MenuSource_None) //make our menu recessive
	{ DisplayMenu(diceMenu, client, 10); }
	else
	{
		if (c_DiceMenu[client] != INVALID_HANDLE)
		{
			new Handle:subhandle = c_DiceMenu[client];
			c_DiceMenu[client] = INVALID_HANDLE;
			CloseHandle(subhandle);
		}
		new Handle:pack;
		c_DiceMenu[client] = CreateDataTimer(5.0, timer_CheckForDiceMenu, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
		WritePackCell(pack, client);
		WritePackCell(pack, GetClientUserId(client));
	}
}

stock GetClientAimingClient(client)
{
	new target = -1;
	new Float:pos[3]; new Float:ang[3];
	GetClientEyePosition(client, pos);
	GetClientEyeAngles(client, ang);
	new Handle:tr = TR_TraceRayFilterEx(pos, ang, MASK_SOLID, RayType_Infinite, Filter_FilterOutData, client);
	if (TR_DidHit(tr))
	{
		new ent = TR_GetEntityIndex(tr);
		if (IsClientValid(ent))
		{
			target = ent;
		}
	}
	CloseHandle(tr);
	return target;
}

stock PrintStateString(client, teamIndex = -1)
{
	if (teamIndex == -1) { teamIndex = GetClientTeam(client); }
	SetGlobalTransTarget(client);
	decl String:msg[255];
	Format(msg, sizeof(msg), "%t\n%t\n", "statestring_line_1", tAliveCount, "statestring_line_2", ctAliveCount);
	if (teamIndex == CS_TEAM_CT)
	{
		new target = GetClientAimingClient(client);
		if (IsClientValid(target))
		{
			if (GetClientTeam(target) == CS_TEAM_T)
			{
				Format(msg, sizeof(msg), "%s\n%N", msg, target);
				if (c_r[target] < c_rBaseMax[target])
				{
					new clientRefuseTimeStamp = RoundFloat(c_rTimeStamp[target]);
					new timeAgo = RoundFloat(GetGameTime()) - clientRefuseTimeStamp;
					if (timeAgo >= 0 && clientRefuseTimeStamp > 0)
					{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_refusings_time", "Yes", c_r[target], timeAgo); }
					else
					{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_refusings", "Yes", c_r[target]); }
				}
				else
				{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_refusings", "No", c_r[target]); }
				if (!c_cReal[client])
				{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_capitulatings", "Yes", c_c[target]); }
				else
				{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_capitulatings", "No", c_c[target]); }
				if (c_rebell[target])
				{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_rebell", "Yes"); }
				else
				{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_rebell", "No"); }
			}
		}
	}
	else
	{
		if (c_r[client] < c_rBaseMax[client])
		{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_refusings", "Yes", c_r[client]); }
		else
		{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_refusings", "No", c_r[client]); }
		if (c_c[client] < c_cBaseMax[client])
		{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_capitulatings", "Yes", c_c[client]); }
		else
		{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_capitulatings", "No", c_c[client]); }
		if (c_rebell[client])
		{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_rebell", "Yes"); }
		else
		{ Format(msg, sizeof(msg), "%s\n%t", msg, "statestring_line_rebell", "No"); }
	}
	AC_PrintKeyText(client, msg);
}

stock UpdatePlayerCount()
{
	tCount = 0; tAliveCount = 0;
	ctCount = 0; ctAliveCount = 0;
	new teamIndex;
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			teamIndex = GetClientTeam(i);
			if (teamIndex == CS_TEAM_CT)
			{
				ctCount++;
				if (IsPlayerAlive(i))
				{ ctAliveCount++; }
			}
			else if (teamIndex == CS_TEAM_T)
			{
				tCount++;
				if (IsPlayerAlive(i))
				{ tAliveCount++; }
			}
		}
	}
	if (OverrideWorkToFalse)
	{ SetCoreState(false); }
	else
	{
		if (tCount > 0 && ctCount > 0)
		{ SetCoreState(true); }
		else
		{ SetCoreState(false); }
	}
}

stock Core_GameFrame_Detour()
{
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			c_StateNotifyOverride[i] = false;
			if (IsPlayerAlive(i) && GetClientTeam(i) == CS_TEAM_CT)
			{
				new buttons = GetClientButtons(i);
				if (buttons & IN_USE)
				{
					c_StateNotifyOverride[i] = true;
				}
			}
		}
	}
}

stock CreateStaticMenus()
{
	diceMenu = CreateMenu(menuHandler_DiceMenu, MenuAction_Display | MenuAction_DisplayItem | MenuAction_Select);
	SetMenuExitButton(diceMenu, true);
	SetMenuTitle(diceMenu, "d_menu_title");
	AddMenuItem(diceMenu, "dice", "d_menu_1");
	AddMenuItem(diceMenu, "nodc", "d_menu_2");
	AddMenuItem(diceMenu, "awdc", "d_menu_3");
	AddMenuItem(diceMenu, "nvdc", "d_menu_4");
	aboutMenu = CreateMenu(menuHandler_AboutMenu, MenuAction_DisplayItem | MenuAction_Select);
	SetMenuExitButton(aboutMenu, true);
	SetMenuTitle(aboutMenu, "Aero Jail Controler");
	AddMenuItem(aboutMenu, "auth", "", ITEMDRAW_DISABLED);
	AddMenuItem(aboutMenu, "vers", "", ITEMDRAW_DISABLED);
	AddMenuItem(aboutMenu, "cont", "", ITEMDRAW_DISABLED);
	AddMenuItem(aboutMenu, "smvs", "", ITEMDRAW_DISABLED);
	AddMenuItem(aboutMenu, "exts", "", ITEMDRAW_DEFAULT);
	aboutBackMenu = CreateMenu(menuHandler_AboutMenu, MenuAction_DisplayItem | MenuAction_Cancel | MenuAction_Select);
	SetMenuExitButton(aboutBackMenu, true);
	SetMenuExitBackButton(aboutBackMenu, true);
	SetMenuTitle(aboutBackMenu, "Aero Jail Controler");
	AddMenuItem(aboutBackMenu, "auth", "", ITEMDRAW_DISABLED);
	AddMenuItem(aboutBackMenu, "vers", "", ITEMDRAW_DISABLED);
	AddMenuItem(aboutBackMenu, "cont", "", ITEMDRAW_DISABLED);
	AddMenuItem(aboutBackMenu, "smvs", "", ITEMDRAW_DISABLED);
	AddMenuItem(aboutBackMenu, "exts", "", ITEMDRAW_DEFAULT);
}

stock CreateAndDisplayCmdMenu(client, time = 45)
{
	new Handle:hndl = CreateMenu(menuHandler_Cmds, MenuAction_End);
	new teamIndex = GetClientTeam(client);
	decl String:str[64];
	decl String:realCmd[32];
	if (Cmd_Alw_About)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_About, "cmds_about", client);
		GetFirstDelimitedCmd(Cmd_Str_About, realCmd);
		AddMenuItem(hndl, realCmd, str);
	}
	if (Cmd_Alw_Rules)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_Rules, "cmds_rules", client);
		GetFirstDelimitedCmd(Cmd_Str_Rules, realCmd);
		AddMenuItem(hndl, realCmd, str);
	}
	if (Cmd_Alw_Refuse)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_Refuse, "cmds_refuse", client);
		GetFirstDelimitedCmd(Cmd_Str_Refuse, realCmd);
		AddMenuItem(hndl, realCmd, str, (teamIndex == CS_TEAM_T) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED); //again ternary :D
	}
	if (Cmd_Alw_Capitulate)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_Capitulate, "cmds_capitulate", client);
		GetFirstDelimitedCmd(Cmd_Str_Capitulate, realCmd);
		AddMenuItem(hndl, realCmd, str, (teamIndex == CS_TEAM_T) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	}
	if (Cmd_Alw_Dice)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_Dice, "cmds_dice", client);
		GetFirstDelimitedCmd(Cmd_Str_Dice, realCmd);
		AddMenuItem(hndl, realCmd, str, (teamIndex == CS_TEAM_T) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	}
	if (Cmd_Alw_DiceMenu)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_DiceMenu, "cmds_dicemenu", client);
		GetFirstDelimitedCmd(Cmd_Str_DiceMenu, realCmd);
		AddMenuItem(hndl, realCmd, str, (teamIndex == CS_TEAM_T) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	}
	if (Cmd_Alw_Kill)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_Kill, "cmds_kill", client);
		GetFirstDelimitedCmd(Cmd_Str_Kill, realCmd);
		AddMenuItem(hndl, realCmd, str);
	}
	if (Cmd_Alw_ColCmds)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_SetCol, "cmds_setcol", client);
		GetFirstDelimitedCmd(Cmd_Str_SetCol, realCmd);
		AddMenuItem(hndl, realCmd, str, (teamIndex == CS_TEAM_CT) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_DelCol, "cmds_delcol", client);
		GetFirstDelimitedCmd(Cmd_Str_DelCol, realCmd);
		AddMenuItem(hndl, realCmd, str, (teamIndex == CS_TEAM_CT) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	}
	if (NoBlockState > 1)
	{
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_Noblock, "cmds_noblock", client);
		GetFirstDelimitedCmd(Cmd_Str_Noblock, realCmd);
		AddMenuItem(hndl, realCmd, str, (teamIndex == CS_TEAM_CT) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
		Format(str, sizeof(str), "%s - %T", Cmd_PStr_Block, "cmds_block", client);
		GetFirstDelimitedCmd(Cmd_Str_Block, realCmd);
		AddMenuItem(hndl, realCmd, str, (teamIndex == CS_TEAM_CT) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	}
	Call_StartForward(cmdMenuBuildForward);
	Call_PushCell(client);
	Call_PushCell(hndl);
	Call_Finish();
	DisplayMenu(hndl, client, time);
}

stock InitMapStart()
{
	PrecacheSound("ambient/explosions/explode_1.wav", true);
	PrecacheSound("ambient/explosions/explode_2.wav", true);
	PrecacheSound("ambient/explosions/explode_4.wav", true);
	PrecacheSound("ambient/explosions/explode_9.wav", true);
	PrecacheSound("weapons/explode3.wav", true);
	PrecacheSound("weapons/explode4.wav", true);
	PrecacheSound("weapons/explode5.wav", true);
	PrecacheSound("weapons/hegrenade/explode3.wav", true);
	PrecacheSound("weapons/hegrenade/explode4.wav", true);
	PrecacheSound("weapons/hegrenade/explode5.wav", true);
	PrecacheSound("weapons/slam/throw.wav", true);
	
	PrecacheModel("models/weapons/w_knife_ct.mdl", true);
	PrecacheModel("models/items/battery.mdl", true);
	
	g_BeamSprite = PrecacheModel("materials/sprites/laser.vmt");
	if (g_BeamSprite <= 0) { Log(EL_Error, "core", "Material 'materials/sprites/laser.vmt' could not be precached."); }
	g_HaloSprite = PrecacheModel("materials/sprites/halo01.vmt");
	if (g_HaloSprite <= 0) { Log(EL_Error, "core", "Material 'materials/sprites/halo01.vmt' could not be precached."); }
	g_Offset_CollisionGroup = FindSendPropOffs("CBaseEntity", "m_CollisionGroup");
	if (g_Offset_CollisionGroup <= 0)
	{
		if (NoBlockState > 0)
		{ Log(EL_Error, "core", "The offset 'm_CollisionGroup' could not be found. Noblock system will be offline."); }
		else
		{ Log(EL_Message, "core", "The offset 'm_CollisionGroup' could not be found."); }
	}
	LoadRuleMenu();
}

stock GetClientAlphaRenderColor(client)
{
	static bool:gotconfig = false;
	static String:prop[32];
	if (!gotconfig)
	{
		new Handle:gc = LoadGameConfigFile("core.games");
		new bool:exists = GameConfGetKeyValue(gc, "m_clrRender", prop, sizeof(prop));
		CloseHandle(gc);
		if (!exists)
		{ strcopy(prop, sizeof(prop), "m_clrRender"); }
		gotconfig = true;
	}
	new offset = GetEntSendPropOffs(client, prop);
	new a = 255;
	if (offset > 0)
	{
		a = GetEntData(client, offset + 3, 1);
	}
	return a;
}

stock FilterCountry_Filter(client)
{
	if (IsFakeClient(client) || IsClientReplay(client) || IsClientSourceTV(client))
	{ return; } //allow bots, etc.
	new AdminId:aId = GetUserAdmin(client);
	if (aId != INVALID_ADMIN_ID)
	{ return; } //allow admins
	new bool:Passed_by_ip = false;
	new bool:Passed_by_lang = false;
	decl String:code[4];
	if (FC_GeoIPCodesCount > 0)
	{
		decl String:ip[16];
		GetClientIP(client, ip, sizeof(ip), true);
		if (GeoipCode2(ip, code))
		{
			for (new i = 0; i < FC_GeoIPCodesCount; i++)
			{
				if (StrEqual(code, FC_GeoIPCodes[i], FC_FilterCaseSensitive))
				{
					Passed_by_ip = true;
					break;
				}
			}
		}
		else
		{
			if (FC_AllowWorldIps)
			{ Passed_by_ip = true; }
		}
	}
	if (FC_LanguageInfosCount > 0)
	{
		new languageCode = GetClientLanguage(client);
		GetLanguageInfo(languageCode, code, sizeof(code));
		for (new i = 0; i < FC_LanguageInfosCount; i++)
		{
			if (StrEqual(code, FC_LanguageInfos[i], FC_FilterCaseSensitive))
			{
				Passed_by_lang = true;
				break;
			}
		}
	}
	new bool:Passed;
	if (FC_LanguageInfosCount > 0 && FC_GeoIPCodesCount > 0)
	{
		if (FC_FilterMatchExact)
		{ Passed = Passed_by_ip && Passed_by_lang; }
		else
		{ Passed = Passed_by_ip || Passed_by_lang; }
	}
	else
	{
		Passed = Passed_by_ip || Passed_by_lang;
	}
	if (!Passed)
	{ KickClient(client, FC_KickMessage); }
}

stock SetCoreState(bool:control)
{
	if (control == InWork) { return; }
	InWork = control;
	Call_StartForward(coreStateForward);
	Call_PushCell(control);
	Call_Finish();
}

stock Float:Math_Max(Float:x1, Float:x2) //well i don't know, it should be a std stock
{
	if (x1 > x2)
	{ return x1; }
	else
	{ return x2;}
}