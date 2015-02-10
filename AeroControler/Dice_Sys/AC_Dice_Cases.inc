/*
**
** Aero Jail Controler Plugin
** Dice Core-Extension of all dice cases
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

stock bool:Diced_Nothing(client)
{
	new Float:perhapsPercent = 100.0 - d_deccapitulates;
	if (perhapsPercent > 0.0)
	{
		AC_Dice_PrintToChat(client, perhapsPercent, d_StateLevel_Neutral, "%t", "d_nothing");
	}
	else
	{
		AC_PrintToChat(client, "%t", "d_nothing");
	}
	return true;
}

stock bool:Diced_Pistol(client)
{
	if (GetPlayerWeaponSlot(client, CS_SLOT_SECONDARY) != -1) { return false; } //when the player have a secondary, he/she doesn't need a second one
	new pistolCase = GetRandomInt(0, 5);
	new ent = -1;
	new primaryAmmo = 1;
	new secondaryAmmo = GetRandomInt(0, 10);
	switch (pistolCase)
	{
		case 0: { ent = GivePlayerItem(client, "weapon_glock"); primaryAmmo = GetRandomInt(1, 20); }
		case 1: { ent = GivePlayerItem(client, "weapon_usp"); primaryAmmo = GetRandomInt(1, 12); }
		case 2: { ent = GivePlayerItem(client, "weapon_p228"); primaryAmmo = GetRandomInt(1, 13); }
		case 3: { ent = GivePlayerItem(client, "weapon_deagle"); primaryAmmo = GetRandomInt(1, 7); }
		case 4: { ent = GivePlayerItem(client, "weapon_fiveseven"); primaryAmmo = GetRandomInt(1, 20); }
		case 5: { ent = GivePlayerItem(client, "weapon_elite"); primaryAmmo = GetRandomInt(1, 30); }
	}
	if (g_offset_ammo == -1) { g_offset_ammo = FindDataMapOffs(client, "m_iAmmo"); }
	new offset = g_offset_ammo + (GetEntProp(ent, Prop_Data, "m_iPrimaryAmmoType") * 4);
	SetEntData(client, offset, primaryAmmo, 4, true);
	offset = g_offset_ammo + (GetEntProp(ent, Prop_Data, "m_iSecondaryAmmoType") * 4);
	SetEntData(client, offset, secondaryAmmo, 4, true);
	AC_Dice_PrintToChat(client, d_pistol, d_StateLevel_Good, "%t", "d_pistol");
	return true;
}

stock bool:Diced_HE(client)
{
	if (GetEntProp(client, Prop_Data, "m_iAmmo", _:4, g_preoffset_he) > 0) { return false; }
	GivePlayerItem(client, "weapon_hegrenade");
	AC_Dice_PrintToChat(client, d_he - d_pistol, d_StateLevel_Good, "%t", "d_he");
	return true;
}

stock bool:Diced_Flash(client)
{
	new currentCount = 0;
	if ((currentCount = GetEntProp(client, Prop_Data, "m_iAmmo", _:4, g_preoffset_flash)) > 1) { return false; }
	new count = GetRandomInt(1, 2);
	if (currentCount == 1) { count = 1; }
	if (count == 1)
	{ GivePlayerItem(client, "weapon_flashbang"); }
	else
	{ GivePlayerItem(client, "weapon_flashbang"); GivePlayerItem(client, "weapon_flashbang"); }
	AC_Dice_PrintToChat(client, d_flash - d_he, d_StateLevel_Good, "%t", "d_flash", count);
	return true;
}

stock bool:Diced_Smoke(client)
{
	if (GetEntProp(client, Prop_Data, "m_iAmmo", _:4, g_preoffset_smoke) > 0) { return false; }
	GivePlayerItem(client, "weapon_smokegrenade");
	AC_Dice_PrintToChat(client, d_smoke - d_flash, d_StateLevel_Good, "%t", "d_smoke");
	return true;
}

stock bool:Diced_IncHP(client)
{
	new amount = GetRandomInt(15, 40);
	SetEntityHealth(client, GetClientHealth(client) + amount);
	AC_Dice_PrintToChat(client, d_inchp - d_smoke, d_StateLevel_Good, "%t", "d_inchp", amount);
	return true;
}

stock bool:Diced_DecHP(client)
{
	new amount = GetRandomInt(15, 40);
	new health = GetClientHealth(client);
	new resultHP = health - amount;
	if (resultHP <= 0) { return false; }
	SetEntityHealth(client, resultHP);
	AC_Dice_PrintToChat(client, d_dechp - d_inchp, d_StateLevel_Bad, "%t", "d_dechp", amount);
	return true;
}

stock bool:Diced_IncSpeed(client)
{
	new Float:faktor = GetRandomFloat(1.2, 1.45);
	new amount = RoundFloat((faktor - 1.0) * 100.0);
	SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", GetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue") * faktor);
	AC_Dice_PrintToChat(client, d_incspeed - d_dechp, d_StateLevel_Good, "%t", "d_incspeed", amount);
	return true;
}

stock bool:Diced_DecSpeed(client)
{
	new Float:faktor = GetRandomFloat(0.8, 0.55);
	new amount = RoundFloat((1.0 - faktor) * 100.0);
	SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", GetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue") * faktor);
	AC_Dice_PrintToChat(client, d_decspeed - d_incspeed, d_StateLevel_Bad, "%t", "d_decspeed", amount);
	return true;
}

stock bool:Diced_LongJump(client)
{
	if (d_LongJump[client]) { return false; }
	d_LongJump[client] = true;
	AC_Dice_PrintToChat(client, d_longjump - d_decspeed, d_StateLevel_Good, "%t", "d_longjump");
	return true;
}

stock bool:Diced_FrogJump(client)
{
	if (d_FrogJump[client]) { return false; }
	d_FrogJump[client] = true;
	AC_Dice_PrintToChat(client, d_frogjump - d_longjump, d_StateLevel_Good, "%t", "d_frogjump");
	return true;
}

stock bool:Diced_DoubleJump(client)
{
	if (d_DoubleJump[client]) { return false; }
	d_DoubleJump[client] = d_DoubleJumpExist = true;
	AC_Dice_PrintToChat(client, d_doublejump - d_frogjump, d_StateLevel_Good, "%t", "d_doublejump");
	return true;
}

stock bool:Diced_Fire(client)
{
	//The time where a client loses 5hp is (in average) 0.975 seconds. So: (5 / 0.975) ≈ 5.128 && 0.975 * 4 = 3.9
	new Float:estimatedTimeToBurn = (float(GetClientHealth(client) + GetClientArmor(client)) / 5.1282051) - 3.9;
	if (estimatedTimeToBurn < 2.0) { return false; } //a burn time below 2 seconds is sensless
	IgniteEntity(client, 1.0);
	new Handle:pack;
	d_HandleFire[client] = CreateDataTimer(0.5, timer_HandleFire, pack, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
	WritePackCell(pack, client);
	WritePackCell(pack, GetClientUserId(client));
	AC_Dice_PrintToChat(client, d_fire - d_doublejump, d_StateLevel_Bad, "%t", "d_fire");
	return true;
}

stock bool:Diced_Noclip(client)
{
	new MoveType:cMoveType = GetEntityMoveType(client);
	if (cMoveType == MOVETYPE_NOCLIP) { return false; }
	if (cMoveType == MOVETYPE_NONE) { return false; }
	SetEntityMoveType(client, MOVETYPE_NOCLIP);
	new Handle:pack;
	d_EndNoclip[client] = CreateDataTimer(5.0, timer_endNoclip, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE); //I know the 2nd flag is not necessary but its fine to have it there :D
	WritePackCell(pack, client);
	WritePackCell(pack, GetClientUserId(client));
	AC_Dice_PrintToChat(client, d_noclip - d_fire, d_StateLevel_Good, "%t", "d_noclip");
	return true;
}

stock bool:Diced_Freeze(client) //it's interesting that the freeze function looks nearly the same as the noclip function
{
	new MoveType:cMoveType = GetEntityMoveType(client);
	if (cMoveType == MOVETYPE_NOCLIP) { return false; }
	if (cMoveType == MOVETYPE_NONE) { return false; }
	SetEntityMoveType(client, MOVETYPE_NONE);
	new Handle:pack;
	d_EndFreeze[client] = CreateDataTimer(30.0, timer_endFreeze, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
	WritePackCell(pack, client);
	WritePackCell(pack, GetClientUserId(client));
	AC_Dice_PrintToChat(client, d_freeze - d_noclip, d_StateLevel_Bad, "%t", "d_freeze");
	return true;
}

stock bool:Diced_LowGrav(client)
{
	new Float:factor = GetRandomFloat(0.5, 0.8);
	new amount = RoundFloat((1.0 - factor) * 100.0);
	SetEntityGravity(client, GetEntityGravity(client) * factor);
	AC_Dice_PrintToChat(client, d_lowgrav - d_freeze, d_StateLevel_Good, "%t", "d_lowgrav", amount);
	return true;
}

stock bool:Diced_HighGrav(client)
{
	new Float:factor = GetRandomFloat(1.2, 1.5);
	new amount = RoundFloat((factor - 1.0) * 100.0);
	SetEntityGravity(client, GetEntityGravity(client) * factor);
	AC_Dice_PrintToChat(client, d_highgrav - d_lowgrav, d_StateLevel_Bad, "%t", "d_highgrav", amount);
	return true;
}

stock bool:Diced_GodMode(client)
{
	if (d_GotGodMode[client]) { return false; }
	new Handle:pack;
	d_EndGodMode[client] = CreateDataTimer(5.0, timer_endGodMode, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
	WritePackCell(pack, client);
	WritePackCell(pack, GetClientUserId(client));
	d_GotGodMode[client] = true; //i know i could take m_takedamage but i prefer to take this kind of solution by using SDKHooks
	AC_Dice_PrintToChat(client, d_godmode - d_highgrav, d_StateLevel_Good, "%t", "d_godmode");
	return true;
}

stock bool:Diced_NV(client)
{
	if (g_offset_hasnv == -1) { g_offset_hasnv = FindSendPropOffs("CCSPlayer", "m_bHasNightVision"); }
	if (g_offset_nvon == -1) { g_offset_nvon = FindSendPropOffs("CCSPlayer", "m_bNightVisionOn"); }
	if (GetEntData(client, g_offset_hasnv, 1) == 1) { return false; }
	SetEntData(client, g_offset_hasnv, true, 1, true);
	SetEntData(client, g_offset_nvon, true, 1, true);
	AC_Dice_PrintToChat(client, d_nv - d_godmode, d_StateLevel_Neutral, "%t", "d_nv");
	return true;
}

stock bool:Diced_Invis(client)
{
	if (g_Game == Game_Csgo)
	{
		return false; //Invisibility is not working in csgo
	}
	new alphaFactor = GetRandomInt(0, 13);
	SetEntityRenderColor(client, 255, 255, 255, alphaFactor);
	SetEntityRenderMode(client, RENDER_TRANSALPHA);
	new amount = RoundFloat(100.0 - ((float(alphaFactor) / 255.0) * 100.0));
	AC_Dice_PrintToChat(client, d_invis - d_nv, d_StateLevel_Good, "%t", "d_invis", amount);
	/* I also have code pieces, for making the weapons invisible or transparent BUT it's funnier, when they keep visible. ^^ */
	return true;
}

stock bool:Diced_Model(client) //let's play a little bit hide&seek xD
{
	if (d_GotModel[client]) { return false; }
	new choose = GetRandomInt(0, d_ModelCount - 1);
	decl String:modelString[PLATFORM_MAX_PATH];
	modelString = d_ModelStringArray[choose];
	if (!IsModelPrecached(modelString))
	{
		PrecacheModel(modelString); //perhaps a little bit late, but its not big and the files should already be precached because they often used...
	}
	SetEntityModel(client, modelString);
	d_GotModel[client] = true;
	AC_Dice_PrintToChat(client, d_model - d_invis, d_StateLevel_Neutral, "%t", "d_model");
	return true;
}

stock bool:Diced_Kevlar(client)
{
	new randVal = GetRandomInt(0, 1);
	new armorVal = GetEntProp(client, Prop_Data, "m_ArmorValue");
	new hasHelm = GetEntProp(client, Prop_Send, "m_bHasHelmet", 4);
	//well, this codepiece, has a fucking secret mistery sense...you'll get it, with a big "ahhhhhhhhhhhhhhhh" if you understood what i did here...
	if (armorVal > 99)
	{
		if (hasHelm > 0)
		{ return false; }
		else
		{ randVal = 1; }
	}
	if (hasHelm == 1 && armorVal < 100) { randVal = 0; }
	if (randVal == 0)
	{
		SetEntProp(client, Prop_Send, "m_ArmorValue", 100);
		AC_Dice_PrintToChat(client, d_kevlar - d_model, d_StateLevel_Good, "%t", "d_kevlar");
	}
	else
	{
		if (armorVal < 100) { SetEntProp(client, Prop_Send, "m_ArmorValue", 100); }
		SetEntProp(client, Prop_Send, "m_bHasHelmet", 1);
		AC_Dice_PrintToChat(client, d_kevlar - d_model, d_StateLevel_Good, "%t", "d_kevlarhelm");
	}
	return true;
}

stock bool:Diced_CFour(client)
{
	if (GetPlayerWeaponSlot(client, CS_SLOT_C4) != -1) { return false; }
	GivePlayerItem(client, "weapon_c4");
	AC_Dice_PrintToChat(client, d_cfour - d_kevlar, d_StateLevel_Neutral, "%t", "d_cfour");
	return true;
}

stock bool:Diced_ExpCFour(client)
{
	if (GetPlayerWeaponSlot(client, CS_SLOT_C4) != -1) { return false; }
	new ent = GivePlayerItem(client, "weapon_c4");
	if (ent > 0)
	{
		if (IsValidEdict(ent))
		{
			new Handle:pack;
			new Float:timeToExplode_f = GetRandomFloat(20.0, 40.0);
			new timeToExplode = RoundFloat(timeToExplode_f) + GetRandomInt(-2, 2); //spray a little bit random into the displayed time ^^
			d_ExpBombExplode[client] = CreateDataTimer(timeToExplode_f, timer_expCFour, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
			/*
			** I know, it is possible that the handles are getting overridden. but that is not bad. Because we overload an entityreference.
			** The Bombs are getting cleaned on a new round so that the references getting invalid. The timer wont setup an explosion.
			*/
			WritePackCell(pack, client);
			WritePackCell(pack, EntIndexToEntRef(ent));
			WritePackCell(pack, GetClientUserId(client));
			WritePackCell(pack, GetClientTeam(client));
			AC_Dice_PrintToChat(client, d_expcfour - d_cfour, d_StateLevel_Neutral, "%t", "d_expcfour", timeToExplode);
			return true;
		}
	}
	return false;
}

stock bool:Diced_ThrowingKnife(client)
{
	new ent = GetPlayerWeaponSlot(client, CS_SLOT_KNIFE);
	if (ent > 0) { RemoveEdict(ent); } //just to give the client a kind of animation that this is a 'new' knife
	GivePlayerItem(client, "weapon_knife");
	d_ThrowingKnifeCount[client]++;
	AC_Dice_PrintToChat(client, d_throwingknife - d_expcfour, d_StateLevel_Good, "%t", "d_throwingknife");
	return true;
}

stock bool:Diced_IncFrags(client)
{
	new amount = GetRandomInt(1, 5);
	SetEntProp(client, Prop_Data, "m_iFrags", (GetClientFrags(client) + amount));
	AC_Dice_PrintToChat(client, d_incfrags - d_throwingknife, d_StateLevel_Good, "%t", "d_incfrags", amount);
	return true;
}

stock bool:Diced_DecFrags(client)
{
	new amount = GetRandomInt(1, 5);
	SetEntProp(client, Prop_Data, "m_iFrags", (GetClientFrags(client) - amount));
	AC_Dice_PrintToChat(client, d_decfrags - d_incfrags, d_StateLevel_Bad, "%t", "d_decfrags", amount);
	return true;
}

stock bool:Diced_Hologram(client)
{
	if (d_HologramShow[client] != INVALID_HANDLE) { return false; }
	new Handle:pack;
	d_HologramShow[client] = CreateDataTimer(0.5, timer_hologramShow, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_REPEAT | TIMER_DATA_HNDL_CLOSE);
	WritePackCell(pack, client);
	WritePackCell(pack, GetClientUserId(client));
	SetEntityRenderFx(client, RENDERFX_DISTORT); //Don't use RENDERFX_HOLOGRAM because it's fade the client out in an special distance
	AC_Dice_PrintToChat(client, d_hologram - d_decfrags, d_StateLevel_Good, "%t", "d_hologram");
	return true;
}

stock bool:Diced_Blind(client)
{
	if (d_GotBlindness[client]) { return false; }
	new color[4];
	color[0] = GetRandomInt(0, 255); color[1] = GetRandomInt(0, 255); color[2] = GetRandomInt(0, 255);
	color[3] = GetRandomInt(210, 235);
	ApplyClientFade(client, 2000, color, false, true);
	d_GotBlindness[client] = true;
	AC_Dice_PrintToChat(client, d_blind - d_hologram, d_StateLevel_Bad, "%t", "d_blind");
	return true;
}

stock bool:Diced_IncRefuses(client)
{
	if (c_c[client] < c_cBase[client]) { return false; }
	c_r[client]++;
	c_rBaseMax[client]++;
	AC_Dice_PrintToChat(client, d_increfuses - d_blind, d_StateLevel_Good, "%t", "d_increfuses");
	return true;
}

stock bool:Diced_DecRefuses(client)
{
	if (c_c[client] < c_cBase[client]) { return false; }
	if (c_r[client] < 1) { return false; }
	c_r[client]--;
	AC_Dice_PrintToChat(client, d_decrefuses - d_increfuses, d_StateLevel_Bad, "%t", "d_decrefuses");
	return true;
}

stock bool:Diced_CTBullyMenu(client)
{
	if (d_CTBullyMenu == INVALID_HANDLE) { CreateMenus(); }
	if (GetClientTeam(client) != CS_TEAM_T) { return false; }
	if (GetClientMenu(client) != MenuSource_None) { return false; }
	DisplayMenu(d_CTBullyMenu, client, 20);
	AC_Dice_PrintToChat(client, d_ctbullymenu - d_decrefuses, d_StateLevel_Good, "%t", "d_ctbullymenu");
	return true;
}

stock bool:Diced_GoodTMenu(client)
{
	if (d_GoodTMenu == INVALID_HANDLE) { CreateMenus(); }
	if (GetClientTeam(client) != CS_TEAM_T) { return false; }
	if (GetClientMenu(client) != MenuSource_None) { return false; }
	DisplayMenu(d_GoodTMenu, client, 10);
	AC_Dice_PrintToChat(client, d_goodtmenu - d_ctbullymenu, d_StateLevel_Good, "%t", "d_goodtmenu");
	return true;
}

stock bool:Diced_BadTMenu(client)
{
	if (d_BadTMenu == INVALID_HANDLE) { CreateMenus(); }
	if (GetClientTeam(client) != CS_TEAM_T) { return false; }
	if (GetClientMenu(client) != MenuSource_None) { return false; }
	DisplayMenu(d_BadTMenu, client, 20);
	AC_Dice_PrintToChat(client, d_badtmenu - d_goodtmenu, d_StateLevel_Bad, "%t", "d_badtmenu");
	return true;
}

stock bool:Diced_MirrorDmg(client)
{
	if (d_GotMirrorDmg[client] || d_GotVampireDmg[client]) { return false; }
	d_GotMirrorDmg[client] = true;
	AC_Dice_PrintToChat(client, d_mirrordmg - d_badtmenu, d_StateLevel_Bad, "%t", "d_mirrordmg");
	return true;
}

stock bool:Diced_VampireDmg(client)
{
	if (d_GotVampireDmg[client] || d_GotMirrorDmg[client]) { return false; }
	d_GotVampireDmg[client] = true;
	AC_Dice_PrintToChat(client, d_vampiredmg - d_mirrordmg, d_StateLevel_Good, "%t", "d_vampiredmg");
	return true;
}

stock bool:Diced_IncDmg(client)
{
	d_dmgFactorModified[client] = true;
	new Float:factor = GetRandomFloat(1.1, 1.25);
	new amount = RoundFloat((factor - 1.0) * 100.0);
	d_dmgFactor[client] *= factor;
	AC_Dice_PrintToChat(client, d_incdmg - d_vampiredmg, d_StateLevel_Good, "%t", "d_incdmg", amount);
	return true;
}

stock bool:Diced_DecDmg(client)
{
	d_dmgFactorModified[client] = true;
	new Float:factor = GetRandomFloat(0.9, 0.75);
	new amount = RoundFloat((1.0 - factor) * 100.0);
	d_dmgFactor[client] *= factor;
	AC_Dice_PrintToChat(client, d_decdmg - d_incdmg, d_StateLevel_Bad, "%t", "d_decdmg", amount);
	return true;
}

stock bool:Diced_Drugs(client)
{
	new amount = GetRandomInt(50, 60);
	d_DrugsSecondsLeft[client] += amount;
	if (d_ApplyDrugs[client] == INVALID_HANDLE)
	{
		new Handle:pack;
		d_ApplyDrugs[client] = CreateDataTimer(1.0, timer_ApplyDrugs, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
		WritePackCell(pack, client);
		WritePackCell(pack, GetClientUserId(client));
	}
	AC_Dice_PrintToChat(client, d_drugs - d_decdmg, d_StateLevel_Bad, "%t", "d_drugs");
	return true;
}

stock bool:Diced_RandTeleChange(client)
{
	new teamIndex = GetClientTeam(client);
	new victim = -1;
	new victims[46]; new victimCount = 0;
	for (new i = 1; i <= MaxClients; i++)
	{
		if (i != client)
		{
			if (IsClientValid(i))
			{
				if (IsPlayerAlive(i) && GetClientTeam(i) == teamIndex)
				{
					victims[victimCount] = i;
					victimCount++;
				}
			}
		}
	}
	if (victimCount == 0) { return false; }
	if (victimCount == 1)
	{ victim = victims[0]; }
	else
	{ victim = victims[GetRandomInt(0, victimCount - 1)]; }
	new MoveType:mType_1 = GetEntityMoveType(client);
	new MoveType:mType_2 = GetEntityMoveType(victim);
	if (mType_1 == MOVETYPE_NOCLIP || mType_2 == MOVETYPE_NOCLIP || mType_1 == MOVETYPE_NONE || mType_2 == MOVETYPE_NONE)
	{ return false; }
	new Float:pos_1[3]; new Float:pos_2[3];
	new Float:ang_1[3]; new Float:ang_2[3];
	new Float:vel_1[3]; new Float:vel_2[3];
	GetClientAbsOrigin(client, pos_1);
	GetClientAbsOrigin(victim, pos_2);
	GetClientEyeAngles(client, ang_1);
	GetClientEyeAngles(victim, ang_2);
	GetEntPropVector(client, Prop_Data, "m_vecVelocity", vel_1);
	GetEntPropVector(victim, Prop_Data, "m_vecVelocity", vel_2);
	new isDucked_1 = GetEntProp(client, Prop_Send, "m_bDucked", 1);
	new isDucked_2 = GetEntProp(victim, Prop_Send, "m_bDucked", 1);
	new inDucking_1 = GetEntProp(client, Prop_Send, "m_bDucking", 1);
	new inDucking_2 = GetEntProp(victim, Prop_Send, "m_bDucking", 1);
	TeleportEntity(client, pos_2, ang_2, vel_2);
	TeleportEntity(victim, pos_1, ang_1, vel_1);
	SetEntProp(client, Prop_Send, "m_bDucking", inDucking_2, 1);
	SetEntProp(victim, Prop_Send, "m_bDucking", inDucking_1, 1);
	SetEntProp(client, Prop_Send, "m_bDucked", isDucked_2, 1);
	SetEntProp(victim, Prop_Send, "m_bDucked", isDucked_1, 1);
	AC_Dice_PrintToChat(client, d_randtelechange - d_drugs, d_StateLevel_Neutral, "%t", "d_randtelechange", victim);
	AC_PrintToChat(victim, "%t", "d_randtelechange_victim", client);
	return true;
}

stock bool:Diced_EntShoot(client)
{
	new amount = GetRandomInt(1, 2);
	d_EntCountToShoot[client] += amount;
	AC_Dice_PrintToChat(client, d_entshoot - d_randtelechange, d_StateLevel_Neutral, "%t", "d_entshoot", amount);
	return true;
}

stock bool:Diced_Regenerate(client)
{
	if (d_Regenerate[client] != INVALID_HANDLE) { return false; }
	new Handle:pack;
	d_Regenerate[client] = CreateDataTimer(2.0, timer_Regenerate, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
	WritePackCell(pack, client);
	WritePackCell(pack, GetClientUserId(client));
	AC_Dice_PrintToChat(client, d_regenerate - d_entshoot, d_StateLevel_Good, "%t", "d_regenerate");
	return true;
}

stock bool:Diced_Disease(client)
{
	if (d_Disease[client] != INVALID_HANDLE) { return false; }
	new Handle:pack;
	d_Disease[client] = CreateDataTimer(2.0, timer_Disease, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
	WritePackCell(pack, client);
	WritePackCell(pack, GetClientUserId(client));
	AC_Dice_PrintToChat(client, d_disease - d_regenerate, d_StateLevel_Bad, "%t", "d_disease");
	return true;
}

stock bool:Diced_TeamHeal(client)
{
	if (d_TeamHeal[client] != INVALID_HANDLE) { return false; }
	if (d_TeamHealCount[client] < 1)
	{
		new Handle:pack;
		d_TeamHeal[client] = CreateDataTimer(2.0, timer_TeamHeal, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
		WritePackCell(pack, client);
		WritePackCell(pack, GetClientUserId(client));
	}
	d_TeamHealCount[client] += GetRandomInt(3, 15);
	AC_Dice_PrintToChat(client, d_teamheal - d_disease, d_StateLevel_Neutral, "%t", "d_teamheal");
	return true;
}

stock bool:Diced_Death_Exp(client)
{
	new Float:pos[3];
	new hp = GetClientHealth(client);
	GetClientAbsOrigin(client, pos);
	SetupExplosion(pos, hp + 50, 0, client);
	if (IsPlayerAlive(client))
	{
		ForcePlayerSuicide(client);
	}
	AC_Dice_PrintToChat(client, d_death_exp - d_teamheal, d_StateLevel_Bad, "%t", "d_death_exp");
	return true;
}

stock bool:Diced_Death_Degenerate(client)
{
	if (d_DeathDegenerate[client] != INVALID_HANDLE) { return false; }
	new Handle:pack;
	d_DeathDegenerate[client] = CreateDataTimer(2.0, timer_Degenerate, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
	WritePackCell(pack, client);
	WritePackCell(pack, GetClientUserId(client));
	AC_Dice_PrintToChat(client, d_death_degenerate - d_death_exp, d_StateLevel_Bad, "%t", "d_death_degenerate");
	return true;
}

stock bool:Diced_Death_Lightning(client)
{
	new Float:origin[3]; new Float:pos[3]; new Float:ang[3];
	GetClientAbsOrigin(client, origin);
	GetClientEyePosition(client, pos);
	ang[0] = -90.0 + GetRandomFloat(-5.0, 5.0);
	ang[1] = GetRandomFloat(-180.0, 180.0);
	ang[2] = 0.0;
	new Handle:tr = TR_TraceRayFilterEx(pos, ang, MASK_SOLID, RayType_Infinite, Filter_FilterOutData, client);
	if (TR_DidHit(tr))
	{
		TR_GetEndPosition(pos, tr);
	}
	else
	{
		pos[2] += 120.0;
	}
	CloseHandle(tr);
	TE_SetupBeamPoints(pos, origin, g_BeamSprite, g_HaloSprite, 0, 10, 0.5, 8.0, 5.0, 0, 15.0, { 255, 255, 255, 255 }, 10);
	TE_SendToAll();
	EmitAmbientSound("ambient/explosions/explode_9.wav", origin, SOUND_FROM_WORLD, SNDLEVEL_RAIDSIREN);
	ForcePlayerSuicide(client);
	AC_Dice_PrintToChat(client, d_death_lightning - d_death_degenerate, d_StateLevel_Bad, "%t", "d_death_lightning");
	return true;
}

stock bool:Diced_WpnRestrict(client)
{
	if (d_WpnRestricted[client]) { return false; }
	RemoveAllClientItems(client, CS_SLOT_KNIFE);
	d_WpnRestricted[client] = true;
	AC_Dice_PrintToChat(client, d_wpnrestrict - d_death_lightning, d_StateLevel_Bad, "%t", "d_wpnrestrict");
	return true;
}

stock bool:Diced_Mine(client)
{
	d_MineCount[client]++;
	AC_Dice_PrintToChat(client, d_mine - d_wpnrestrict, d_StateLevel_Good, "%t", "d_mine");
	return true;
}

stock bool:Diced_IncCapitulates(client)
{
	c_c[client]++;
	c_cBaseMax[client]++;
	AC_Dice_PrintToChat(client, d_inccapitulates - d_mine, d_StateLevel_Good, "%t", "d_inccapitulates");
	return true;
}

stock bool:Diced_DecCapitulates(client)
{
	if (c_c[client] < 1) { return false; }
	c_c[client]--;
	AC_Dice_PrintToChat(client, d_deccapitulates - d_inccapitulates, d_StateLevel_Bad, "%t", "d_deccapitulates");
	return true;
}