/*
**
** Aero Jail Controler Plugin
** Dice Core-Extension of all dice logic
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

new g_offset_ammo = -1;
new g_offset_hasnv = -1;
new g_offset_nvon = -1;

new g_preoffset_he = 11;
new g_preoffset_flash = 12;
new g_preoffset_smoke = 13;

new bool:d_LongJump[MAXPLAYERS + 1];
new bool:d_FrogJump[MAXPLAYERS + 1];
new bool:d_DoubleJump[MAXPLAYERS + 1];
new d_JumpState[MAXPLAYERS + 1];
new bool:d_DoubleJumpExist = false; //optimization. We don't have to iterate trough the clients in the game frame, when noone have it.
new bool:d_GotModel[MAXPLAYERS + 1];
#define d_ModelCount 5
new String:d_ModelStringArray[d_ModelCount][PLATFORM_MAX_PATH] = 
{ "models/props/cs_office/vending_machine.mdl"
, "models/props/cs_office/sofa_chair.mdl"
, "models/props/de_prodigy/tirestack2.mdl"
, "models/props_pipes/valve002.mdl"
, "models/props_wasteland/kitchen_stove002a.mdl"};
new bool:d_GotBlindness[MAXPLAYERS + 1];
new Handle:d_EndNoclip[MAXPLAYERS + 1];
new Handle:d_EndFreeze[MAXPLAYERS + 1];
new Handle:d_HandleFire[MAXPLAYERS + 1];
new bool:d_GotGodMode[MAXPLAYERS + 1];
new Handle:d_EndGodMode[MAXPLAYERS + 1];
new Handle:d_ExpBombExplode[MAXPLAYERS + 1];
new d_ThrowingKnifeCount[MAXPLAYERS + 1];
new Handle:d_HologramShow[MAXPLAYERS + 1];
new Handle:d_CTBullyMenu = INVALID_HANDLE;
new Handle:d_GoodTMenu = INVALID_HANDLE;
new Handle:d_BadTMenu = INVALID_HANDLE;
new bool:d_GotMirrorDmg[MAXPLAYERS + 1];
new bool:d_GotVampireDmg[MAXPLAYERS + 1];
new bool:d_dmgFactorModified[MAXPLAYERS + 1];
new Float:d_dmgFactor[MAXPLAYERS + 1] = 1.0;
new d_DrugsSecondsLeft[MAXPLAYERS + 1];
new Handle:d_ApplyDrugs[MAXPLAYERS + 1];
new d_EntCountToShoot[MAXPLAYERS + 1];
#define d_EntModelCount 3
new String:d_EntModelStringArray[d_EntModelCount][PLATFORM_MAX_PATH] =
{ "models/props_junk/metal_paintcan001b.mdl"
, "models/props_junk/garbage_metalcan002a.mdl"
, "models/props_junk/propanecanister001a.mdl"};
new Handle:d_Regenerate[MAXPLAYERS + 1];
new Handle:d_Disease[MAXPLAYERS + 1];
new Handle:d_TeamHeal[MAXPLAYERS + 1];
new d_TeamHealCount[MAXPLAYERS + 1];
new Handle:d_DeathDegenerate[MAXPLAYERS + 1];
new bool:d_WpnRestricted[MAXPLAYERS + 1];
new d_MineCount[MAXPLAYERS + 1];

new Handle:d_MineCheck[MAXPLAYERS + 1];
#define MAX_MINES 32
new bool:d_MineActive[MAX_MINES];
new d_MineOwner[MAX_MINES];
new d_MineOwnerTeam[MAX_MINES];
new d_MineEnt[MAX_MINES];

stock Diced_PlayerJump_Detour(client)
{
	if (d_DoubleJump[client])
	{
		if (d_JumpState[client] == 0) { d_JumpState[client] = 1; }
	}
	new Float:vec[3];
	new bool:ChangesMade = false;
	if (d_LongJump[client])
	{
		vec[0] = 1.5 * GetEntPropFloat(client, Prop_Send, "m_vecVelocity[0]");
		vec[1] = 1.5 * GetEntPropFloat(client, Prop_Send, "m_vecVelocity[1]");
		ChangesMade = true;
	}
	if (d_FrogJump[client])
	{
		vec[2] += 180.0;
		ChangesMade = true;
	}
	if (ChangesMade)
	{
		SetEntPropVector(client, Prop_Send, "m_vecBaseVelocity", vec);
	}
}

stock Diced_GameFrame_Detour()
{
	if (d_DoubleJumpExist) //it's amazing ^^
	{
		for (new i = 1; i <= MaxClients; i++)
		{
			if (IsClientInGame(i)) //you asking why i don't use the IsClientValid stock? Well, timing is everything in the gameframe forward...^^
			{
				if (IsClientConnected(i))
				{
					if (IsPlayerAlive(i))
					{
						if (d_DoubleJump[i])
						{
							new keys = GetClientButtons(i);
							new flags = GetEntityFlags(i);
							if (flags & FL_ONGROUND)
							{
								d_JumpState[i] = 0;
							}
							else
							{
								if (keys & IN_JUMP)
								{
									if (d_JumpState[i] == 2)
									{
										d_JumpState[i] = -1;
										new Float:vec[3];
										GetAngleVectors(Float:{-90.0, 0.0, 0.0}, vec, NULL_VECTOR, NULL_VECTOR);
										NormalizeVector(vec, vec);
										ScaleVector(vec, 250.0);
										SetEntPropVector(i, Prop_Send, "m_vecBaseVelocity", vec);
									}
								}
								else
								{
									if (d_JumpState[i] == 1)
									{
										d_JumpState[i] = 2;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

stock Diced_EndRound_Detour()
{
	Clear_All_D_Vars();
	for (new i = 0; i < MAX_MINES; i++)
	{
		d_MineActive[i] = false;
		d_MineOwner[i] = 0;
		d_MineOwnerTeam[i] = 0;
		d_MineEnt[i] = 0;
		if (d_MineCheck[i] != INVALID_HANDLE)
		{
			new Handle:subHndl = d_MineCheck[i];
			d_MineCheck[i] = INVALID_HANDLE;
			CloseHandle(subHndl);
		}
	}
}

stock Diced_PlayerDeath_Detour(client)
{
	Clear_Client_D_Vars(client, true);
}

stock Diced_ResetClientVars_Detour(client)
{
	Clear_Client_D_Vars(client);
}

stock bool:Diced_CanUseWeapon_Detour(client, weapon)
{
	if (d_WpnRestricted[client])
	{
		decl String:className[MAX_NAME_LENGTH]; //MAX_NAME_LENGTH...appropriate?
		GetEntityClassname(weapon, className, sizeof(className));
		if (!StrEqual(className, "weapon_knife"))
		{
			return false;
		}
	}
	return true;
}

stock Float:Diced_SDKH_TakeDmg(victim, attacker, Float:dmg)
{
	if (IsClientValid(victim)) //We only handle clients
	{
		if (d_GotGodMode[victim])
		{
			dmg = 0.0;
		}
		if (IsClientValid(attacker)) //that's not obvious
		{
			if (GetClientTeam(victim) != GetClientTeam(attacker))
			{
				if (d_dmgFactorModified[attacker])
				{
					dmg *= d_dmgFactor[attacker];
				}
				if (d_GotMirrorDmg[attacker])
				{
					new amount = GetClientHealth(attacker) - RoundFloat(dmg / 5.0);
					if (amount < 1) { amount = 1; }
					SetEntityHealth(attacker, amount);
				}
				else if (d_GotVampireDmg[attacker])
				{
					new life = GetClientHealth(attacker);
					if (!(life > 250))
					{
						new amount = life + RoundFloat(dmg / 5.0);
						if (amount > 250) { amount = 250; }
						SetEntityHealth(attacker, amount);
					}
				}
			}
		}
	}
	return dmg;
}

stock Diced_WeaponFire_Detour(client, const String:weaponStr[])
{
	if (d_ThrowingKnifeCount[client] > 0)
	{
		if (StrEqual(weaponStr, "knife", true))
		{
			d_ThrowingKnifeCount[client]--;
			new Float:eyePos[3];
			new Float:eyeAng[3];
			new Float:fwd[3];
			new Float:origin[3];
			new Float:spinVec[3] = { 1080.0, 0.0, 0.0 }; //3 rounds per second - seems legit ^^
			GetClientEyePosition(client, eyePos);
			GetClientEyeAngles(client, eyeAng);
			GetAngleVectors(eyeAng, fwd, NULL_VECTOR, NULL_VECTOR);
			NormalizeVector(fwd, fwd);
			ScaleVector(fwd, 50.0);
			AddVectors(eyePos, fwd, origin);
			ScaleVector(fwd, 30.0); //50*30=1500
			new ent = CreateEntityByName("hegrenade_projectile");
			TeleportEntity(ent, origin, eyeAng, fwd);
			DispatchSpawn(ent);
			SetEntityModel(ent, "models/weapons/w_knife_ct.mdl");
			SetEntPropVector(ent, Prop_Data, "m_vecAngVelocity", spinVec);
			SetEntPropFloat(ent, Prop_Send, "m_flElasticity", 0.3);
			SetEntPropEnt(ent, Prop_Send, "m_hThrower", client);
			SDKHook(ent, SDKHook_StartTouch, SDKH_ThrowingKnifeTouch);
			EmitAmbientSound("weapons/slam/throw.wav", origin, SOUND_FROM_WORLD, SNDLEVEL_NORMAL, SND_NOFLAGS, 0.7);
			
		}
	}
	else if (d_EntCountToShoot[client] > 0)
	{
		if (StrEqual(weaponStr, "knife", true))
		{
			d_EntCountToShoot[client]--;
			new Float:eyePos[3];
			new Float:eyeAng[3];
			new Float:fwd[3];
			new Float:origin[3];
			new Float:spinVec[3] = { 720.0, -720.0, 360.0 };
			GetClientEyePosition(client, eyePos);
			GetClientEyeAngles(client, eyeAng);
			if (eyeAng[0] <= 62.0) // at 64.5° we'll get hit by our own entity.
			{
				GetAngleVectors(eyeAng, fwd, NULL_VECTOR, NULL_VECTOR);
				NormalizeVector(fwd, fwd);
				ScaleVector(fwd, 50.0);
				AddVectors(eyePos, fwd, origin);
				ScaleVector(fwd, 30.0); //50*30=1500
				new ent = CreateEntityByName("hegrenade_projectile");
				TeleportEntity(ent, origin, eyeAng, fwd);
				DispatchSpawn(ent);
				decl String:modelName[PLATFORM_MAX_PATH];
				modelName = d_EntModelStringArray[GetRandomInt(0, d_EntModelCount - 1)];
				if (!IsModelPrecached(modelName))
				{
					PrecacheModel(modelName);
				}
				SetEntityModel(ent, modelName);
				SetEntPropVector(ent, Prop_Data, "m_vecAngVelocity", spinVec);
				SetEntPropFloat(ent, Prop_Send, "m_flElasticity", 0.3);
				SetEntPropEnt(ent, Prop_Send, "m_hThrower", client);
				SDKHook(ent, SDKHook_StartTouch, SDKH_ThrowingEntTouch);
				EmitAmbientSound("weapons/slam/throw.wav", origin, SOUND_FROM_WORLD, SNDLEVEL_NORMAL, SND_NOFLAGS, 0.7);
			}
		}
	}
	else if (d_MineCount[client] > 0)
	{
		if (StrEqual(weaponStr, "knife", true))
		{
			new index = -1;
			for (new i  = 0; i < MAX_MINES; i++)
			{
				if (!d_MineActive[i]) { index = i; break; }
			}
			if (index == -1) { return; }
			new Float:EyePos[3];
			new Float:EyeAng[3];
			new Float:PlaneVec[3];
			new Float:ang[3];
			new Float:pos[3];
			new bool:DidHit = false;
			GetClientEyePosition(client, EyePos);
			GetClientEyeAngles(client, EyeAng);
			new Handle:traceRay = TR_TraceRayFilterEx(EyePos, EyeAng, MASK_SOLID, RayType_Infinite, Filter_FilterOutData, client);
			if ((DidHit = TR_DidHit(traceRay)))
			{
				TR_GetEndPosition(pos, traceRay);
				TR_GetPlaneNormal(traceRay, PlaneVec);
				GetVectorAngles(PlaneVec, ang);
				new Float:testDistance = GetVectorDistance(pos, EyePos);
				new Float:origin[3];
				GetClientAbsOrigin(client, origin);
				new Float:testDistance_Ogn = GetVectorDistance(pos, origin);
				if (testDistance > 120.0 || testDistance < 40.0 || testDistance_Ogn < 40.0)
				{
					DidHit = false;
				}
			}
			CloseHandle(traceRay);
			if (DidHit)
			{
				new ent = CreateEntityByName("prop_dynamic_override");
				SetEntityModel(ent, "models/items/battery.mdl");
				DispatchKeyValue(ent, "solid", "6");
				ang[0] += 90.0;
				TeleportEntity(ent, pos, ang, NULL_VECTOR);
				DispatchSpawn(ent);
				d_MineCount[client]--;
				AcceptEntityInput(ent, "TurnOn", ent, ent, 0);
				//AcceptEntityInput(ent, "DisableCollision");
				SetEntProp(ent, Prop_Data, "m_takedamage", 2);
				SetEntProp(ent, Prop_Data, "m_iMinHealthDmg", 50);
				SetEntProp(ent, Prop_Data, "m_iMaxHealth", 5000);
				SetEntProp(ent, Prop_Data, "m_iHealth", 5000);
				new teamIndex = GetClientTeam(client);
				if (teamIndex == CS_TEAM_T)
				{ SetEntityRenderColor(ent, 255, 128, 128, 255); }
				else if (teamIndex == CS_TEAM_CT)
				{ SetEntityRenderColor(ent, 128, 128, 255, 255); }
				SDKHook(ent, SDKHook_OnTakeDamagePost, SDKH_MineGetDamage);
				d_MineActive[index] = true;
				d_MineOwner[index] = GetClientUserId(client);
				d_MineOwnerTeam[index] = GetClientTeam(client);
				d_MineEnt[index] = ent;
				new Handle:pack;
				d_MineCheck[index] = CreateDataTimer(2.0, timer_CheckMines, pack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE | TIMER_REPEAT);
				WritePackCell(pack, index);
			}
		}
	}
}

stock Clear_Client_D_Vars(client, bool:OwningUpdate = false)
{
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			SetEntityRenderFx(client, RENDERFX_NONE);
		}
	}
	d_LongJump[client] = false;
	d_FrogJump[client] = false;
	d_DoubleJump[client] = false;
	d_GotModel[client] = false;
	d_JumpState[client] = 0;
	d_GotGodMode[client] = false;
	d_ThrowingKnifeCount[client] = 0;
	d_GotMirrorDmg[client] = false;
	d_GotVampireDmg[client] = false;
	d_dmgFactorModified[client] = false;
	d_dmgFactor[client] = 1.0;
	d_EntCountToShoot[client] = 0;
	d_TeamHealCount[client] = 0;
	d_WpnRestricted[client] = false;
	d_MineCount[client] = 0;
	if (d_EndNoclip[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_EndNoclip[client];
		d_EndNoclip[client] = INVALID_HANDLE; //so we can be sure, if an handle error occurs, it occurs just one time.
		CloseHandle(subHndl);
	}
	if (d_EndFreeze[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_EndFreeze[client];
		d_EndFreeze[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_HandleFire[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_HandleFire[client];
		d_HandleFire[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_EndGodMode[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_EndGodMode[client];
		d_EndGodMode[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_ExpBombExplode[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_ExpBombExplode[client];
		d_ExpBombExplode[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_HologramShow[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_HologramShow[client];
		d_HologramShow[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_ApplyDrugs[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_ApplyDrugs[client];
		d_ApplyDrugs[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_Regenerate[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_Regenerate[client];
		d_Regenerate[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_Disease[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_Disease[client];
		d_Disease[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_TeamHeal[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_TeamHeal[client];
		d_TeamHeal[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (d_DeathDegenerate[client] != INVALID_HANDLE)
	{
		new Handle:subHndl = d_DeathDegenerate[client];
		d_DeathDegenerate[client] = INVALID_HANDLE;
		CloseHandle(subHndl);
	}
	if (OwningUpdate)
	{
		d_DoubleJumpExist = false;
		for (new i = 1; i <= MaxClients; i++)
		{
			if (d_DoubleJump[i]) { d_DoubleJumpExist = true; break; }
		}
	}
}

stock Clear_All_D_Vars()
{
	d_DoubleJumpExist = false;
	for (new i = 1; i <= MaxClients; i++)
	{
		Clear_Client_D_Vars(i);
	}
}

stock d_SetupExplosion(Float:pos[3], damage, radius = 0, teamOverride = CS_TEAM_T, guiltyClient = -1)
{
	new ent = CreateEntityByName("env_explosion");
	DispatchKeyValue(ent, "spawnflags", "1088"); //no sound, no dlights
	DispatchSpawn(ent);
	SetEntProp(ent, Prop_Data, "m_iMagnitude", damage);
	SetEntProp(ent, Prop_Data, "m_iRadiusOverride", radius);
	if (guiltyClient != -1)
	{
		SetEntPropEnt(ent, Prop_Send, "m_hOwnerEntity", guiltyClient);
	}
	SetEntProp(ent, Prop_Send, "m_iTeamNum", teamOverride);
	TeleportEntity(ent, pos, NULL_VECTOR, NULL_VECTOR);
	AcceptEntityInput(ent, "Explode");
	EmitRandomExplosionSound(pos); //Core->Stocks
	AcceptEntityInput(ent, "kill");
}

stock CreateMenus()
{
	d_CTBullyMenu = CreateMenu(menuHandler_CTBullyMenu, MenuAction_Select | MenuAction_Display | MenuAction_DisplayItem);
	SetMenuTitle(d_CTBullyMenu, "d_ctbm_title");
	SetMenuExitButton(d_CTBullyMenu, true);
	AddMenuItem(d_CTBullyMenu, "ctbm_1", "d_ctbm_1");
	AddMenuItem(d_CTBullyMenu, "ctbm_2", "d_ctbm_2");
	AddMenuItem(d_CTBullyMenu, "ctbm_3", "d_ctbm_3");
	AddMenuItem(d_CTBullyMenu, "ctbm_4", "d_ctbm_4");
	d_GoodTMenu = CreateMenu(menuHandler_GoodTMenu, MenuAction_Select | MenuAction_Display | MenuAction_DisplayItem);
	SetMenuTitle(d_GoodTMenu, "d_gdtm_title");
	SetMenuExitButton(d_GoodTMenu, true);
	AddMenuItem(d_GoodTMenu, "gdtm_1", "d_gdtm_1");
	AddMenuItem(d_GoodTMenu, "gdtm_2", "d_gdtm_2");
	AddMenuItem(d_GoodTMenu, "gdtm_3", "d_gdtm_3");
	AddMenuItem(d_GoodTMenu, "gdtm_4", "d_gdtm_4");
	d_BadTMenu = CreateMenu(menuHandler_BadTMenu, MenuAction_Select | MenuAction_Display | MenuAction_DisplayItem | MenuAction_Cancel);
	SetMenuTitle(d_BadTMenu, "d_bdtm_title");
	SetMenuExitButton(d_BadTMenu, false);
	AddMenuItem(d_BadTMenu, "bdtm_1", "d_bdtm_1");
	AddMenuItem(d_BadTMenu, "bdtm_2", "d_bdtm_2");
	AddMenuItem(d_BadTMenu, "bdtm_3", "d_bdtm_3");
	AddMenuItem(d_BadTMenu, "bdtm_4", "d_bdtm_4");
}

/*
**
** Timer
**
*/

public Action:timer_endNoclip(Handle:timer, Handle:pack)
{
	ResetPack(pack, false);
	new client = ReadPackCell(pack);
	d_EndNoclip[client] = INVALID_HANDLE;
	client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			SetEntityMoveType(client, MOVETYPE_WALK);
			new Float:pos[3];
			new Float:ePos[3];
			new Float:ringPos[3];
			GetClientAbsOrigin(client, pos);
			if (TR_PointOutsideWorld(pos)) { ForcePlayerSuicide(client); return Plugin_Stop; }
			GetClientEyePosition(client, ePos);
			if (TR_PointOutsideWorld(ePos)) { ForcePlayerSuicide(client); return Plugin_Stop; }
			ringPos[0] = pos[0];
			ringPos[1] = pos[1];
			ringPos[2] = (pos[2] + ePos[2]) / 2.0;
			TE_SetupBeamRingPoint(ringPos, 100.0, 25.0, g_BeamSprite, g_HaloSprite, 0, 15, 2.5, 20.0, 0.0, { 255, 255, 255, 255 }, 10, 0);
			TE_SendToClient(client);
			new Handle:newPack;
			d_EndNoclip[client] = CreateDataTimer(2.5, timer_finalNoclipSafe, newPack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
			WritePackCell(newPack, client);
			WritePackCell(newPack, GetClientUserId(client));
			WritePackFloat(newPack, pos[0]);
			WritePackFloat(newPack, pos[1]);
			WritePackFloat(newPack, pos[2]);
			WritePackFloat(newPack, ringPos[0]);
			WritePackFloat(newPack, ringPos[1]);
			WritePackFloat(newPack, ringPos[2]);
			AC_PrintToChat(client, "%t", "d_noclipendsafe");
		}
	}
	return Plugin_Stop;
}

public Action:timer_finalNoclipSafe(Handle:timer, Handle:pack)
{
	ResetPack(pack, false);
	new client = ReadPackCell(pack);
	d_EndNoclip[client] = INVALID_HANDLE;
	client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			new Float:pos[3];
			new Float:refPos[3];
			new Float:ringPos[3];
			refPos[0] = ReadPackFloat(pack);
			refPos[1] = ReadPackFloat(pack);
			refPos[2] = ReadPackFloat(pack);
			ringPos[0] = ReadPackFloat(pack);
			ringPos[1] = ReadPackFloat(pack);
			ringPos[2] = ReadPackFloat(pack);
			GetClientAbsOrigin(client, pos);
			if (GetVectorDistance(pos, refPos) < 25.0)
			{
				ForcePlayerSuicide(client);
			}
			TE_SetupBeamRingPoint(ringPos, 25.0, 100.0, g_BeamSprite, g_HaloSprite, 0, 15, 0.5, 3.0, 0.0, { 255, 128, 128, 255 }, 10, 0);
			TE_SendToClient(client);
		}
	}
	return Plugin_Stop;
}

public Action:timer_endFreeze(Handle:timer, Handle:pack)
{
	ResetPack(pack, false);
	new client = ReadPackCell(pack);
	d_EndFreeze[client] = INVALID_HANDLE;
	client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			SetEntityMoveType(client, MOVETYPE_WALK);
		}
	}
}

public Action:timer_HandleFire(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			new hp = GetClientHealth(client);
			new clientFlags = GetEntityFlags(client);
			if (hp > 20)
			{
				if (!(clientFlags & FL_INWATER))
				{
					IgniteEntity(client, 1.0);
					return Plugin_Continue;
				}
			}
			if (clientFlags & FL_ONFIRE)
			{ ExtinguishEntity(client); }
		}
	}
	d_HandleFire[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}	

public Action:timer_endGodMode(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new client = ReadPackCell(pack);
	d_EndGodMode[client] = INVALID_HANDLE;
	client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			d_GotGodMode[client] = false;
		}
	}
}

public Action:timer_expCFour(Handle:timer, Handle:pack)
{
	ResetPack(pack, false);
	new client = ReadPackCell(pack);
	d_ExpBombExplode[client] = INVALID_HANDLE;
	new ent = EntRefToEntIndex(ReadPackCell(pack));
	client = GetClientOfUserId(ReadPackCell(pack));
	new team = ReadPackCell(pack);
	if (ent > 0)
	{
		if (IsValidEdict(ent))
		{
			new guiltyClient = -1;
			if (client > 0)
			{
				if (IsClientValid(client)) { guiltyClient = client; }
			}
			new Float:pos[3];
			GetEntPropVector(ent, Prop_Data, "m_vecAbsOrigin", pos);
			AcceptEntityInput(ent, "kill");
			d_SetupExplosion(pos, 200, 250, team, guiltyClient);
		}
	}
}

public Action:timer_hologramShow(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			new teamIndex = GetClientTeam(client);
			new Float:pos_one[3];
			new Float:pos_two[3];
			GetClientAbsOrigin(client, pos_one);
			pos_one[2] += 35.0;
			for (new i = 1; i <= MaxClients; i++)
			{
				if (IsClientValid(i))
				{
					if (IsPlayerAlive(i) && GetClientTeam(i) != teamIndex)
					{
						GetClientAbsOrigin(i, pos_two);
						pos_two[2] += 35.0;
						TE_SetupBeamPoints(pos_one, pos_two, g_BeamSprite, g_HaloSprite, 0, 10, 0.5, 3.0, 3.0, 0, 0.0, { 255, 255, 255, 128 }, 10);
						TE_SendToClient(client);
					}
				}
			}
			return Plugin_Continue;
		}
	}
	d_HologramShow[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}

public Action:timer_ApplyDrugs(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			d_DrugsSecondsLeft[client]--;
			new Float:ang[3];
			GetClientEyeAngles(client, ang);
			if (d_DrugsSecondsLeft[client] != 0)
			{
				ang[2] = GetRandomFloat(-30.0, 30.0);
				TeleportEntity(client, NULL_VECTOR, ang, NULL_VECTOR);
				new color[4];
				color[0] = GetRandomInt(0, 255); color[1] = GetRandomInt(0, 255); color[2] = GetRandomInt(0, 255); 
				color[3] = 200;
				ApplyClientFade(client, 1000, color, false, false);
				return Plugin_Continue;
			}
			ang[2] = 0.0;
			TeleportEntity(client, NULL_VECTOR, ang, NULL_VECTOR);
			new color[4];
			color[0] = GetRandomInt(0, 255); color[1] = GetRandomInt(0, 255); color[2] = GetRandomInt(0, 255); 
			color[3] = 200;
			ApplyClientFade(client, 1000, color, true, false);
		}
	}
	d_DrugsSecondsLeft[handleIndex] = 0;
	d_ApplyDrugs[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}

public Action:timer_Regenerate(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			new hp = GetClientHealth(client);
			if (hp < 100)
			{
				hp++;
				SetEntityHealth(client, hp);
			}
			return Plugin_Continue;
		}
	}
	d_Regenerate[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}

public Action:timer_Disease(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			new hp = GetClientHealth(client);
			if (hp > 1)
			{
				hp--;
				SetEntityHealth(client, hp);
			}
			return Plugin_Continue;
		}
	}
	d_Disease[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}

public Action:timer_TeamHeal(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			new Float:pos[3]; new Float:ang[3];
			GetClientEyePosition(client, pos);
			GetClientEyeAngles(client, ang);
			new Handle:tr = TR_TraceRayFilterEx(pos, ang, MASK_SOLID, RayType_Infinite, Filter_FilterOutData, client);
			if (TR_DidHit(tr))
			{
				new victim = TR_GetEntityIndex(tr);
				if (IsClientValid(victim))
				{
					new Float:dest[3];
					TR_GetEndPosition(dest, tr);
					if (GetVectorDistance(pos, dest) <= 250.0)
					{
						new victim_hp = GetClientHealth(victim);
						if (victim_hp < 100 && GetClientTeam(client) == GetClientTeam(victim))
						{
							d_TeamHealCount[client]--;
							SetEntityHealth(victim, 100);
							pos[2] -= 25.0;
							TE_SetupBeamPoints(pos, dest, g_BeamSprite, g_HaloSprite, 0, 10, 2.0, 25.0, 25.0, 0, 0.0, { 10, 128, 255, 255}, 10);
							TE_SendToAll();
							TE_SetupEnergySplash(pos, NULL_VECTOR, false);
							TE_SendToAll();
							TE_SetupEnergySplash(dest, NULL_VECTOR, false);
							TE_SendToAll();
						}
					}
				}
			}
			CloseHandle(tr);
			if (d_TeamHealCount[client] > 0)
			{
				return Plugin_Continue;
			}
		}
	}
	d_TeamHeal[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}

public Action:timer_Degenerate(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new handleIndex = ReadPackCell(pack);
	new client = GetClientOfUserId(ReadPackCell(pack));
	if (IsClientValid(client))
	{
		if (IsPlayerAlive(client))
		{
			new hp = GetClientHealth(client);
			if (hp == 1)
			{
				d_DeathDegenerate[handleIndex] = INVALID_HANDLE;
				ForcePlayerSuicide(client);
				return Plugin_Stop;
			}
			else
			{
				hp--;
				SetEntityHealth(client, hp);
				return Plugin_Continue;
			}
		}
	}
	d_DeathDegenerate[handleIndex] = INVALID_HANDLE;
	return Plugin_Stop;
}

public Action:timer_CheckMines(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new index = ReadPackCell(pack);
	new hasWorked = false;
	if (d_MineActive[index])
	{
		hasWorked = true;
		new ent = d_MineEnt[index];
		if (!IsValidEdict(ent))
		{ d_MineCheck[index] = INVALID_HANDLE; return Plugin_Stop; }
		new mineTeam = d_MineOwnerTeam[index];
		new Float:pos[3];
		GetEntPropVector(ent, Prop_Data, "m_vecAbsOrigin", pos);
		for (new j = 1; j <= MaxClients; j++)
		{
			if (IsClientValid(j))
			{
				if (IsPlayerAlive(j) && GetClientTeam(j) != mineTeam)
				{
					new Float:origin[3];
					GetClientAbsOrigin(j, origin);
					if (GetVectorDistance(pos, origin) <= 300.0)
					{
						d_MineActive[index] = false;
						d_MineOwner[index] = 0;
						d_MineOwnerTeam[index] = 0;
						d_MineEnt[index] = 0;
						d_MineCheck[index] = INVALID_HANDLE;
						AcceptEntityInput(ent, "kill");
						new guiltyClient = GetClientOfUserId(d_MineOwner[index]);
						if (!IsClientValid(guiltyClient)) { guiltyClient = -1; }
						SetupExplosion(pos, 200, 300, guiltyClient, mineTeam);
						return Plugin_Stop;
					}
				}
			}
		}
	}
	if (!hasWorked) { d_MineCheck[index] = INVALID_HANDLE; return Plugin_Stop; }
	return Plugin_Continue;
}

/*
**
** Menuhandler
**
*/

public _:menuHandler_CTBullyMenu(Handle:menu, MenuAction:action, client, param)
{
	if (action == MenuAction_Select)
	{
		if (!IsPlayerAlive(client)) { return 0; }
		decl String:choosen[16];
		GetMenuItem(menu, param, choosen, sizeof(choosen));
		new victim = 0;
		new victims[24]; new victimCount = 0;
		for (new i = 1; i <= MaxClients; i++)
		{
			if (IsClientValid(i))
			{
				if (IsPlayerAlive(i) && GetClientTeam(i) == CS_TEAM_CT)
				{
					victims[victimCount] = i;
					victimCount++;
					if (victimCount == 23)
					{
						break;
					}
				}
			}
		}
		if (victimCount == 0) { AC_PrintToChat(client, "%t", "d_noctavaiable"); return 0; }
		if (victimCount == 1)
		{ victim = victims[0]; }
		else
		{ victim = victims[GetRandomInt(0, victimCount - 1)]; }
		if (StrEqual(choosen, "ctbm_1", true))
		{
			SetEntPropFloat(victim, Prop_Data, "m_flLaggedMovementValue", GetEntPropFloat(victim, Prop_Data, "m_flLaggedMovementValue") * 0.8);
			AC_PrintToChat(client, "%t", "d_ctbm_1_client", victim);
			AC_PrintToChat(victim, "%t", "d_ctbm_1_victim", client);
		}
		else if (StrEqual(choosen, "ctbm_2", true))
		{
			new amount = GetClientHealth(victim) - 50;
			if (amount < 1) { amount = 1; }
			SetEntityHealth(victim, amount);
			AC_PrintToChat(client, "%t", "d_ctbm_2_client", victim);
			AC_PrintToChat(victim, "%t", "d_ctbm_2_victim", client);
		}
		else if (StrEqual(choosen, "ctbm_3", true))
		{
			new color[4];
			color[0] = GetRandomInt(0, 255); color[1] = GetRandomInt(0, 255); color[2] = GetRandomInt(0, 255);
			color[3] = GetRandomInt(210, 235);
			ApplyClientFade(victim, 2000, color, false, true);
			AC_PrintToChat(client, "%t", "d_ctbm_3_client", victim);
			AC_PrintToChat(victim, "%t", "d_ctbm_3_victim", client);
		}
		else if (StrEqual(choosen, "ctbm_4", true))
		{
			SetEntityGravity(victim, GetEntityGravity(victim) * 1.4);
			AC_PrintToChat(client, "%t", "d_ctbm_4_client", victim);
			AC_PrintToChat(victim, "%t", "d_ctbm_4_victim", client);
		}
	}
	else if (action == MenuAction_DisplayItem)
	{
		decl String:display[32];
		decl String:displayTranslated[32];
		GetMenuItem(menu, param, "", _:0, _, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, client);
		return RedrawMenuItem(displayTranslated);
	}
	else if (action == MenuAction_Display)
	{
		decl String:display[32];
		decl String:displayTranslated[32];
		GetMenuTitle(menu, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, client);
		SetPanelTitle(Handle:param, displayTranslated);
	}
	return 0;
}

public _:menuHandler_GoodTMenu(Handle:menu, MenuAction:action, client, param)
{
	if (action == MenuAction_Select)
	{
		if (!IsPlayerAlive(client)) { return 0; }
		decl String:choosen[16];
		GetMenuItem(menu, param, choosen, sizeof(choosen));
		if (StrEqual(choosen, "gdtm_1", true))
		{
			SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", GetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue") * 1.2);
		}
		else if (StrEqual(choosen, "gdtm_2", true))
		{
			SetEntityGravity(client, GetEntityGravity(client) * 0.6);
		}
		else if (StrEqual(choosen, "gdtm_3", true))
		{
			SetEntityHealth(client, GetClientHealth(client) + 40);
		}
		else if (StrEqual(choosen, "gdtm_4", true))
		{
			SetEntityRenderColor(client, 255, 255, 255, 13);
			SetEntityRenderMode(client, RENDER_TRANSALPHA);
		}
	}
	else if (action == MenuAction_DisplayItem)
	{
		decl String:display[32];
		decl String:displayTranslated[32];
		GetMenuItem(menu, param, "", _:0, _, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, client);
		return RedrawMenuItem(displayTranslated);
	}
	else if (action == MenuAction_Display)
	{
		decl String:display[32];
		decl String:displayTranslated[32];
		GetMenuTitle(menu, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, client);
		SetPanelTitle(Handle:param, displayTranslated);
	}
	return 0;
}

public _:menuHandler_BadTMenu(Handle:menu, MenuAction:action, client, param)
{
	if (action == MenuAction_Select)
	{
		if (!IsPlayerAlive(client)) { return 0; }
		decl String:choosen[16];
		GetMenuItem(menu, param, choosen, sizeof(choosen));
		if (StrEqual(choosen, "bdtm_1", true))
		{
			SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", GetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue") * 0.8);
		}
		else if (StrEqual(choosen, "bdtm_2", true))
		{
			new amount = GetClientHealth(client);
			amount -= 50;
			if (amount < 1)
			{ ForcePlayerSuicide(client); } //his own fault...keep more attention ^^
			else
			{ SetEntityHealth(client, amount); }
		}
		else if (StrEqual(choosen, "bdtm_3", true))
		{
			SetEntityGravity(client, GetEntityGravity(client) * 1.4);
		}
		else if (StrEqual(choosen, "bdtm_4", true))
		{
			new color[4];
			color[0] = GetRandomInt(0, 255); color[1] = GetRandomInt(0, 255); color[2] = GetRandomInt(0, 255);
			color[3] = GetRandomInt(210, 235);
			ApplyClientFade(client, 2000, color, false, true);
		}
	}
	else if (action == MenuAction_DisplayItem)
	{
		decl String:display[32];
		decl String:displayTranslated[32];
		GetMenuItem(menu, param, "", _:0, _, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, client);
		return RedrawMenuItem(displayTranslated);
	}
	else if (action == MenuAction_Display)
	{
		decl String:display[32];
		decl String:displayTranslated[32];
		GetMenuTitle(menu, display, sizeof(display));
		Format(displayTranslated, sizeof(displayTranslated), "%T", display, client);
		SetPanelTitle(Handle:param, displayTranslated);
	}
	else if (action == MenuAction_Cancel)
	{
		if (IsClientValid(client))
		{
			if (IsPlayerAlive(client))
			{
				new amount = GetClientHealth(client);
				amount -= 50;
				if (amount > 0)
				{
					SetEntityHealth(client, amount);
					new color[4];
					color[0] = GetRandomInt(0, 255); color[1] = GetRandomInt(0, 255); color[2] = GetRandomInt(0, 255);
					color[3] = GetRandomInt(210, 235);
					ApplyClientFade(client, 2000, color, false, true);
					SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", GetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue") * 0.8);
					SetEntityGravity(client, GetEntityGravity(client) * 1.4);
				}
				else
				{
					ForcePlayerSuicide(client); //it wouldn't make sense if his speed, gravity and blindness is set, when he/she is dying.
				}
				AC_PrintToChat(client, "%t", "d_bdtm_penaltie");
			}
		}
	}
	return 0;
}
/*
**
** SDKHooks
**
*/

public Action:SDKH_ThrowingKnifeTouch(entity, other)
{
	if (IsClientValid(other))
	{
		new tempKnife = CreateEntityByName("weapon_knife");
		new attacker = GetEntPropEnt(entity, Prop_Send, "m_hThrower");
		new Float:dmgMultiply = 1.0;
		if (IsClientValid(attacker))
		{
			SetEntPropEnt(tempKnife, Prop_Send, "m_hOwnerEntity", attacker);
			SetEntProp(tempKnife, Prop_Send, "m_iTeamNum", GetClientTeam(attacker));
			if (d_dmgFactorModified[attacker])
			{
				dmgMultiply = d_dmgFactor[attacker];
			}
		}
		else
		{ attacker = -1; }
		SDKHooks_TakeDamage(other, tempKnife, attacker, (65.0 * dmgMultiply), DMG_CLUB, attacker);
		AcceptEntityInput(entity, "kill");
		AcceptEntityInput(tempKnife, "kill");
	}
	else
	{
		new Float:vec[3];
		GetEntPropVector(entity, Prop_Data, "m_vecVelocity", vec);
		if (GetVectorLength(vec) <= 100.0)
		{
			AcceptEntityInput(entity, "kill"); //we wont let it stay in the map out of several reasons beginning with: it interferes
			//i asking myself, is RemoveEdict not a better choose for this?
		}
	}
	return Plugin_Continue;
}

public Action:SDKH_ThrowingEntTouch(entity, other)
{
	if (other > 0)
	{
		if (IsClientValid(other) || IsValidEdict(other))
		{
			new Float:pos[3];
			new Float:vel[3];
			GetEntPropVector(entity, Prop_Data, "m_vecAbsOrigin", pos);
			GetEntPropVector(entity, Prop_Data, "m_vecVelocity", vel);
			AcceptEntityInput(entity, "kill");
			TE_SetupEnergySplash(pos, NULL_VECTOR, false);
			TE_SendToAll();
			TeleportEntity(other, NULL_VECTOR, NULL_VECTOR, vel); //i know - not the yellow from the egg but i dont have to make the boring normalvector calculations xD
			return Plugin_Continue;
		}
	}
	new Float:pos[3];
	GetEntPropVector(entity, Prop_Data, "m_vecAbsOrigin", pos);
	AcceptEntityInput(entity, "kill");
	TE_SetupEnergySplash(pos, NULL_VECTOR, false);
	TE_SendToAll();
	EmitAmbientSound("weapons/slam/throw.wav", pos, SOUND_FROM_WORLD, SNDLEVEL_NORMAL, SND_NOFLAGS, 1.0);
	return Plugin_Continue;
}

public SDKH_MineGetDamage(victim, attacker, inflictor, Float:damage, damagetype)
{
	for (new i = 0; i < MAX_MINES; i++)
	{
		if (d_MineActive[i])
		{
			if (victim == d_MineEnt[i])
			{
				new enemyTeam = -2;
				if (IsClientValid(attacker))
				{ enemyTeam = GetClientTeam(attacker); }
				new mineTeam = d_MineOwnerTeam[i];
				if (mineTeam != enemyTeam)
				{
					new ent = d_MineEnt[i];
					new Float:pos[3];
					GetEntPropVector(ent, Prop_Data, "m_vecAbsOrigin", pos);
					d_MineActive[i] = false;
					d_MineOwner[i] = 0;
					d_MineOwnerTeam[i] = 0;
					d_MineEnt[i] = 0;
					d_MineCheck[i] = INVALID_HANDLE;
					AcceptEntityInput(ent, "kill");
					new guiltyClient = GetClientOfUserId(d_MineOwner[i]);
					if (!IsClientValid(guiltyClient)) { guiltyClient = -1; }
					SetupExplosion(pos, 200, 300, guiltyClient, mineTeam);
				}
			}
		}
	}
}