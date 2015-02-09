#pragma semicolon 1

/*
**
** Aero Jail Controler Plugin
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
//#define DEBUG

#define PLUGIN_VERSION_DEF "1.10λ" //RTM
#define PLUGIN_AUTHOR "Λeon"

/*
**---------------------------------------------------------------------------Roadmap---------------------------------------------------------------------------
** γ - gamma version - Not suitable for public server! Only for testservers.
** α - alpha version - Should be suitable for public servers. But should not used on it. 
** β - beta version  - Suitable for public servers. Use to detect and fix bugs.
** λ - lambda version - Official versions. Already tested, suitable for running on public servers (Should not contain bugs).
** 
** _________________________________________________________________________Done Tasks_________________________________________________________________________
** |date					|version		|description														|comments
** |21th July 2013		|0.00γ			|Begin of scripting												|2nd attempt, the 1st attempt was not well written
** |23th July 2013		|0.10γ			|refuse, capitulate, rebell - system ready						|additions on the 12th August 2013
** |11th August 2013		|0.30γ			|dice - system ready												|
** |13th August 2013		|0.40γ			|sc,dc,kill,state,noblock/block-system ready					|
** |14th August 2013		|0.50γ			|mute system, gun safety, drop all								|15th August 2013-->Fixxed duplicate nades bug
** |18th August 2013		|0.55γ			|rule,cmds menu ready												|possibility to add multiple cmds to one action added on 21th January 2014
** |07th October 2013		|0.60γ			|war system														|just 3 wars added
** |15th December 2013	|0.70α			|first tests														|
** |05th January 2014		|0.80β			|first beta tests - get wishes, suggestions, etc.				|added randomness to the warvote, maincheck for the mute system
** |12th January 2014		|0.85β			|interface heavily expanded										|
** |18th January 2014		|0.90β			|final beta test													|
** |20th January 2014		|0.95β			|Country Filter													|
** |25th January 2014		|1.00λ			|RTM Stable Version / First Release								|release canceled out of DoS attacks on our server, continuing developement
** |15th Februrary 2014	|1.10λ			|Storesystem+Skillforcesystem ready								|4th februrary 2014 - Storesystem ready
** |22th Februrary 2014	|1.10λ			|First Release													|~10500 loc
** |26th Februrary 2014	|1.10.000129λ	|bug & error fixed version - added some misc things			|Buildnumber: 000129
** _________________________________________________________________________Done Tasks_________________________________________________________________________
** 
** 
** 
** 
** ________________________________________________________________________Planed Tasks________________________________________________________________________
** |version	|description
** |1.11		|seperate modules (at least mute system)
** |1.15		|game cmds
** |1.20		|new WAR's
** |1.30		|CT Ban
** |1.40		|big cvar Update
** |1.45		|big admin-cmds update
** |1.50		|Deathmute
** |1.60		|deadgames
** |2.00		|lastrequest system
** |2.10		|official publication, approving (hopefully) & releasing
**
** ________________________________________________________________________Planed Tasks________________________________________________________________________
** 
**---------------------------------------------------------------------------Roadmap---------------------------------------------------------------------------
*/

//It have to be Sourcemod 1.5 or higher
/* std::includes */
#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <geoip>			//used for the country filter
#include <cstrike>		//Make me CSS Specific
#include <version>		//Make them able to see for which SM version this is compiled
/* std::includes */

/* plugin::includes */
#include "AeroControler\\SharedPluginBase\\AC_ErrorSys.inc"
#include "AeroControler\\SharedPluginBase\\AC_UISys.inc"
#include "AeroControler\\SharedPluginBase\\AC_ClientSys.inc"

#include "AeroControler\\Core\\AC_Vars.inc"
#include "AeroControler\\Core\\AC_InterfaceControl.inc"
#include "AeroControler\\Core\\AC_Configs.inc"
#include "AeroControler\\Core\\AC_Stocks.inc"
#include "AeroControler\\Core\\AC_Events.inc"
#include "AeroControler\\Core\\AC_Cmds.inc"
#include "AeroControler\\Core\\AC_HandlerCallbacks.inc"
/* plugin::includes */

/* mod::includes */
#include "AeroControler\\Dice_Sys\\AC_Dice_Core.inc"
#include "AeroControler\\Dice_Sys\\AC_Dice_Logic.inc"
#include "AeroControler\\Dice_Sys\\AC_Dice_Cases.inc"

#include "AeroControler\\Mute_Sys\\AC_Mute_Core.inc"
/* mod::includes */

/* 3rdParty::includes */
#undef REQUIRE_EXTENSIONS
#include <steamtools>
#define REQUIRE_EXTENSIONS
/* 3rdParty::includes */

public Plugin:myinfo = 
{
	name = "Aero Controler",
	author = PLUGIN_AUTHOR,
	description = "Jail Control Plugin which provides many jail specific functions.",
	version = PLUGIN_VERSION_DEF,
	url = "Julien.Kluge@gmail.com"
};

public OnPluginStart()
{
	CreateConVar("ac_version", PLUGIN_VERSION, "Version of the AeroControler",  FCVAR_PLUGIN | FCVAR_SPONLY | FCVAR_REPLICATED | FCVAR_NOTIFY | FCVAR_DONTRECORD);
	
	LoadVersionNumber();
	InitCVarSettings();
	CreateStaticMenus();
	
	if (Cmd_Alw_About) { RegMultipleConsoleCmd(Cmd_Str_About, Cmd_About, "Shows the about menu."); }
	if (Cmd_Alw_Refuse) { RegMultipleConsoleCmd(Cmd_Str_Refuse, Cmd_Refuse, "Refusing a jail-minigame."); }
	if (Cmd_Alw_Capitulate) { RegMultipleConsoleCmd(Cmd_Str_Capitulate, Cmd_Capitulate, "Capitulate in jail."); }
	if (Cmd_Alw_Dice) { RegMultipleConsoleCmd(Cmd_Str_Dice, Cmd_Dice, "Dice in jail."); }
	if (Cmd_Alw_DiceMenu) { RegMultipleConsoleCmd(Cmd_Str_DiceMenu, Cmd_DiceMenu, "Open the dice-menu in jail."); }
	if (Cmd_Alw_Kill) { RegMultipleConsoleCmd(Cmd_Str_Kill, Cmd_Kill, "Commit suicide."); }
	if (Cmd_Alw_ColCmds)
	{
		RegMultipleConsoleCmd(Cmd_Str_SetCol, Cmd_SetColors, "Restore the terrorist colors.");
		RegMultipleConsoleCmd(Cmd_Str_DelCol, Cmd_DeleteColors, "Hide the terrorist colors.");
	}
	if (Cmd_Alw_Commands) { RegMultipleConsoleCmd(Cmd_Str_Commands, Cmd_Commands, "List all commands."); }
	if (Cmd_Alw_Rules) { RegMultipleConsoleCmd(Cmd_Str_Rules, Cmd_Rules, "Shows the rule menu."); }
	RegMultipleConsoleCmd(Cmd_Str_Noblock, Cmd_Noblock, "Turns noblock on.");
	RegMultipleConsoleCmd(Cmd_Str_Block, Cmd_Block, "Turns block on.");
	
	if (!AddCommandListener(Listener_Drop, "drop")) { RegConsoleCmd("drop", Cmd_Drop); }
	
	RegAdminCmd("sm_forcedice", ACmd_ForceDice, ADMFLAG_CHEATS, "sm_forcedice <#userid|name> [value|phrase]");
	
	HookEvent("player_spawn", Event_Pre_PlayerSpawn, EventHookMode_Pre);
	HookEvent("player_spawn", Event_Post_PlayerSpawn, EventHookMode_Post);
	HookEvent("player_death", Event_PlayerDeath, EventHookMode_Post);
	HookEvent("round_start", Event_RoundStart, EventHookMode_Post);
	HookEvent("round_end", Event_RoundEnd, EventHookMode_Post);
	HookEvent("player_jump", Event_PlayerJump, EventHookMode_Post);
	HookEvent("weapon_fire", Event_WeaponFire, EventHookMode_Post);
	HookEvent("player_hurt", Event_PlayerHurt, EventHookMode_Post);
	HookEvent("player_activate", Event_ActivatePlayer, EventHookMode_Post);
	
	if (announce_Timer == INVALID_HANDLE)
	{ announce_Timer = CreateTimer(announce_delay, timer_Announce, INVALID_HANDLE, TIMER_REPEAT); }
	
	for (new i = 0; i < (MAXPLAYERS + 1); i++) { ResetClientVars(i); } //init my client variables
	
	AutoExecConfig(true, "aerocontroler");
}

public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
	MarkNativeAsOptional("Steam_SetGameDescription"); //the include didn't do it, so we have to...
	extensionPlugins = CreateArray(64, 0);
	LoadStaticPluginConfig(); //it have to be here, because we must have the static color codes when the natives are registered
	LoadTranslationsFiles(); //it have to be here, because the same as above without the natives xD
	AC_InterfaceControlRegister();
	return APLRes_Success;
}

public OnAllPluginsLoaded()
{
	if (GunSafetyState == 0)
	{
		new bool:gunSafetyWillWork = false;
		if (CanTestFeatures())
		{
			if (GetFeatureStatus(FeatureType_Capability, FEATURECAP_COMMANDLISTENER) == FeatureStatus_Available)
			{
				gunSafetyWillWork = true;
				AddCommandListener(Listener_ShouldStripped, "sm_slay"); //Only try to add the listener because, we need to get the command before the 
				AddCommandListener(Listener_ShouldStripped, "sm_kick"); //original plugin handle it.
				AddCommandListener(Listener_ShouldStripped, "sm_ban");  //Only in that way, we are able to be sure that the gun-safety is able to work.
			}
		}
		if (gunSafetyWillWork)
		{ GunSafetyState = 1; }
		else
		{ GunSafetyState = 2; Log(EL_Message, "core", "The gun safety functions wont working! (Not able to establish the commandlisteners)"); }
	}
}

public OnLibraryAdded(const String:name[])
{
	MuteSys_LibraryAdded_Detour(name);
}

public OnLibraryRemoved(const String:name[])
{
	MuteSys_LibraryRemoved_Detour(name);
}

public OnConfigsExecuted()
{
	if (strlen(gameDescription) > 0)
	{
		if (CanTestFeatures())
		{
			if (GetFeatureStatus(FeatureType_Native, "Steam_SetGameDescription") == FeatureStatus_Available)
			{
				Steam_SetGameDescription(gameDescription);
			}
			else
			{
				Log(EL_Error, "core", "Steamtools is not available. The Game description wont be set.");
			}
		}
		else
		{
			Log(EL_Message, "core", "Could not test if steamtools is available. Game description wont be set. Update your sourcemod!");
		}
	}
}

public OnMapStart()
{
	LoadDiceProbabilities();
	InitMapStart();
	if (IntroduceMe) { CreateTimer(90.0, timer_Introduce, INVALID_HANDLE, TIMER_FLAG_NO_MAPCHANGE); }//Fire and forget Bamm,Bamm,Bamm,Bamm,Bamm
	CreateTimer(1.0, timer_NotifyOfStates, INVALID_HANDLE, TIMER_FLAG_NO_MAPCHANGE | TIMER_REPEAT);
}

public OnClientPutInServer(client)
{
	SDKHook(client, SDKHook_WeaponCanUse, SDKH_OnCanUseWeapon);
	SDKHook(client, SDKHook_OnTakeDamage, SDKH_OnTakeDamage);
}

public OnClientDisconnect_Post(client)
{
	ResetClientVars(client); //yearh, i lied when i said that I will place this in the connect-forward xD
	UpdatePlayerCount();
}

public OnClientPostAdminCheck(client)
{
	/*if (FC_FilterCountries)
	{ FilterCountry_Filter(client); } TODO: BUGGY*/
	MuteSys_LoadClient(client);
}

public OnGameFrame()
{
	Core_GameFrame_Detour();
	Diced_GameFrame_Detour();
}