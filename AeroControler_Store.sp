#pragma semicolon 1

/*
**
** Aero Jail Controler Plugin
** Store System
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

#define DEBUG

/* γ = dev : α = canditae for control testing : β = proving ground/release candidate : λ = Final stable/RTM */
#define PLUGIN_VERSION "1.00λ"
#define PLUGIN_AUTHOR "Λeon"

/* std::includes */
#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <cstrike>
/* std::includes */

/* interface::includes */
#include "AeroControler\\aerocontroler_core_interface.inc" //interface to the core
/* interface::includes */

/* plugin::includes */
#include "AeroControler\\SharedPluginBase\\AC_SharedPluginBase.inc"

#include "AeroControler\\Store_Sys\\AC_STORE_Vars.inc"
#include "AeroControler\\Store_Sys\\AC_STORE_Stocks.inc"
#include "AeroControler\\Store_Sys\\AC_STORE_Configs.inc"
#include "AeroControler\\Store_Sys\\AC_STORE_HandlerCallbacks.inc"
#include "AeroControler\\Store_Sys\\AC_STORE_Events.inc"
#include "AeroControler\\Store_Sys\\AC_STORE_Cmds.inc"
#include "AeroControler\\Store_Sys\\AC_STORE_SQLCallbacks.inc"
#include "AeroControler\\Store_Sys\\AC_STORE_FeatureStocks.inc"
/* plugin::includes */

public Plugin myinfo = 
{
	name = "Aero Controler Store",
	author = PLUGIN_AUTHOR,
	description = "Providing a store system for the aerocontroler",
	version = PLUGIN_VERSION,
	url = "Julien.Kluge@gmail.com"
};

public void OnPluginStart()
{
	DetectGameMod();
	LoadStaticConfig();
	LoadTranslationFiles();
	ConnectToDatabase();
	
	ac_getCoreComTag(Tag, sizeof(Tag));
	ExtensionEntryIndex = ac_registerPluginExtension("Aero Store System", "_AeonOne_", PLUGIN_VERSION);
	ac_registerCMDMenuBuildForward(view_as<buildCMDMenuForward>AddStoreCmdsToCmdMenu);
	
	HookEvent("round_end", Event_RoundEnd, EventHookMode_Post);
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
	HookEvent("player_death", Event_PlayerDeath, EventHookMode_Post);
	HookEvent("hegrenade_detonate", Event_HEDetonate, EventHookMode_Post);
	
	if (Cmd_Store) { RegMultipleConsoleCmd(Cmd_Str_Store, Cmd_OpenStore, "Opens the store menu"); }
	if (Cmd_StoreTop) { RegMultipleConsoleCmd(Cmd_Str_StoreTop, Cmd_OpenStoreTop, "Opens the store-top menu"); }
	if (Cmd_StoreRank) { RegMultipleConsoleCmd(Cmd_Str_StoreRank, Cmd_OpenStoreRank, "Opens the store-rank menu"); }
	if (Cmd_SF) { RegMultipleConsoleCmd(Cmd_Str_SF, Cmd_SkillForce, "Opens the skillforce menu"); }
	if (Cmd_SFC) { RegMultipleConsoleCmd(Cmd_Str_SFC, Cmd_SFChat, "Chat with your skillforce"); }
}

public void OnPluginEnd()
{
	if (LibraryExists("ac_core"))
	{
		ac_unregisterPluginExtension(ExtensionEntryIndex);
	}
}

public void OnMapStart()
{
	g_BeamSprite = PrecacheModel("materials/sprites/laserbeam.vmt");
	g_HaloSprite = PrecacheModel("materials/sprites/glow01.vmt");
	skillTimer = CreateTimer(2.0, timer_CheckSkill, INVALID_HANDLE, TIMER_REPEAT);
}

public void OnMapEnd()
{
	CloseHandle(skillTimer);
}

public void OnClientPutInServer(client)
{
	SDKHook(client, SDKHook_OnTakeDamage, SDKH_OnTakeDamage);
}

public void ac_OnCoreStateChanged(bool indicator)
{
	InWork = indicator;
}

public void ac_OnCoreComTagChanged(const char[] tag)
{
	Format(Tag, sizeof(Tag), "%s", tag);
}

public void OnClientAuthorized(client, const char[] auth)
{
	#if defined DEBUG
	LoadClient(client, auth);
	#else
	if (IsClientReplay(client) || IsClientSourceTV(client) || IsFakeClient(client)) { return; }
	LoadClient(client, auth);
	#endif
}

public void OnClientDisconnect(client)
{
	if (IsClientValid(client))
	{ UpdateClientAsync(client); }
}

public Action OnClientSayCommand(client, const char[] command, const char[] sArgs)
{
	if (c_InSFNameChooseProcess[client])
	{
		if (StrEqual(command, "say", false))
		{
			c_InSFNameChooseProcess[client] = false;
			char name[32];
			char argument[64];
			Format(argument, sizeof(argument), "%s", sArgs);
			StripQuotes(argument);
			WhiteStripIllegalCharacters(argument);
			TrimString(argument);
			SQL_EscapeString(db, argument, name, sizeof(name));
			if (strlen(name) > 1)
			{
				Handle pack = CreateDataPack();
				WritePackCell(pack, GetClientUserId(client));
				WritePackString(pack, name);
				char query[255];
				Format(query, sizeof(query), "SELECT id FROM `%s` WHERE name='%s'", sfTableName, name);
				SQL_TQuery(db, TSQL_CheckUniqueSFName, query, pack);
			}
			else
			{ AC_PrintToChat(client, "%t", "sf_foundsf_nameshort", name); }
			return Plugin_Handled;
		}
	}
	return Plugin_Continue;
}