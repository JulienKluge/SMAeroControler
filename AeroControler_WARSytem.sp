#pragma semicolon 1

/*
**
** Aero Jail Controler Plugin
** WAR Base Control System
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
#define PLUGIN_VERSION "1.03λ"

#include <sourcemod>
#include <sdktools>
#include <cstrike>
#include <sdkhooks>

typeset WarStart
{
	function void ();
	function void (int index);
	function void (int index, int maxrounds, int maxtime);
};
typeset WarEnd
{
	function void ();
	function void (int index);
};
typedef WarFreezeTimeStart = function void (int time);
typedef WarFreezeTimeEnd = function void ();

#include "AeroControler\\aerocontroler_core_interface.inc" //interface to the core

#include "AeroControler\\SharedPluginBase\\AC_SharedPluginBase.inc"

#include "AeroControler\\War_Sys\\Controler\\AC_WAR_Vars.inc"
#include "AeroControler\\War_Sys\\Controler\\AC_WAR_Stocks.inc"
#include "AeroControler\\War_Sys\\Controler\\AC_WAR_HandlerCallbacks.inc"
#include "AeroControler\\War_Sys\\Controler\\AC_WAR_Events.inc"
#include "AeroControler\\War_Sys\\Controler\\AC_WAR_Cmds.inc"
#include "AeroControler\\War_Sys\\Controler\\AC_WAR_Configs.inc"

public Plugin myinfo = 
{
	name = "Aero Controler - Extension: WAR Base Control System",
	author = "_AeonOne_",
	description = "Controls the basic functions in a war.",
	version = PLUGIN_VERSION,
	url = "Julien.Kluge@gmail.com"
};

public void OnPluginStart()
{
	DetectGameMod();
	LoadStaticConfig();
	LoadTranslationFiles();
	
	ac_getCoreComTag(Tag, sizeof(Tag));
	ExtensionEntryIndex = ac_registerPluginExtension("Aero WAR Base Control System", "_AeonOne_", PLUGIN_VERSION);
	ac_registerCMDMenuBuildForward(view_as<buildCMDMenuForward>AddWarCmdToCmdMenu);
	
	if (Alw_warVoteCmd) { RegMultipleConsoleCmd(Str_WarVoteCmd, Cmd_VoteWar, "Vote for a war"); }
	
	RegAdminCmd("sm_forcewarvote", ACmd_ForceWarMenu, ADMFLAG_CHEATS, "sm_forcewarvote [#userid|name]");
	RegAdminCmd("sm_stopwar", ACmd_StopWar, ADMFLAG_CHEATS, "sm_stopwar");
	
	HookEvent("round_end", Event_RoundEnd, EventHookMode_Post);
	HookEvent("round_start", Event_RoundStart, EventHookMode_Post);
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
}

public void OnPluginEnd()
{
	if (LibraryExists("ac_core")) //alibi check
	{
		ac_unregisterPluginExtension(ExtensionEntryIndex);
	}
}

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, err_max)
{
	CreateNative("ac_war_RegWar", view_as<NativeCall>AC_Native_RegWar);
	CreateNative("ac_war_UnRegWar", view_as<NativeCall>AC_Native_UnRegWar);
	CreateNative("ac_war_EndWar", view_as<NativeCall>AC_Native_EndWar);
	RegPluginLibrary("ac_war_sys");
	return APLRes_Success;
}

public void OnMapStart()
{
	g_Offset_CollisionGroup = FindSendPropOffs("CBaseEntity", "m_CollisionGroup");
}

public void ac_OnCoreComTagChanged(const char[] tag)
{
	Format(Tag, sizeof(Tag), "%s", tag);
}

public void OnClientPutInServer(client)
{
	SDKHook(client, SDKHook_OnTakeDamage, SDKH_OnTakeDamage);
}

public void OnClientDisconnect_Post(client)
{
	HasVotedWar[client] = false;
}