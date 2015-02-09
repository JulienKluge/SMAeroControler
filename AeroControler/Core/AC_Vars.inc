/*
**
** Aero Jail Controler Plugin - Variables partial File
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

#define CS_SLOT_KNIFE 2		//why the hell isn't that in the cstrike.inc ???????
//I mean, primary weapon is, secondary is, grenades are even the bomb is - why not the knife?

#define COLLISION_GROUP_PLAYER 5
#define COLLISION_GROUP_DEBRIS_TRIGGER 2

/* Server Vars */
new String:PLUGIN_VERSION[32];
new bool:IntroduceMe = true;
new String:serverChatColor[7] = "1188FF";
new String:serverStaticChatColor[7] = "1188FF";
new String:serverTag[32] = "[Jail]:";
new Handle:tagChangedForward = INVALID_HANDLE;
new Handle:clientRoundReadyForward = INVALID_HANDLE;
new Handle:cmdMenuBuildForward = INVALID_HANDLE;
new Handle:coreStateForward = INVALID_HANDLE;
new String:gameDescription[64] = "JailBreak CSS";
new Handle:extensionPlugins = INVALID_HANDLE;
/* Server Vars */

/* Misc Vars */
new bool:DropAll_Active = true;
new tCount = 0;
new tAliveCount = 0;
new ctCount = 0;
new ctAliveCount = 0;
new bool:OverrideWorkToFalse = false;
new bool:InWork = true;
new StateNotifyCounter = 0;
new NoBlockState = 2; new RoundStartedState = 2; //we need the second variable when changes are made in the round
new GunSafetyState = 0; //-1=disabled | 0=Not initialized | 1=initialized | 2=init failed
new ctSpawnWeaponCount = 3;
new String:ctSpawnWeapons[10][32] = { "weapon_m4a1", "weapon_deagle", "weapon_knife", "", "", "", "", "", "", "" };
new tSpawnWeaponCount = 1;
new String:tSpawnWeapons[10][32] = { "weapon_knife", "", "", "", "", "", "", "", "", "" };
new Handle:aboutMenu = INVALID_HANDLE;
new Handle:aboutBackMenu = INVALID_HANDLE;
new Handle:ruleMenu = INVALID_HANDLE;
new Handle:ruleSubMenus = INVALID_HANDLE;
new bool:announce_dice = true;
new bool:announce_rules = true;
new bool:announce_cmds = true;
new Float:announce_delay = 600.0;
new bool:autoshow_dicemenu = true;
new Handle:announce_Timer = INVALID_HANDLE;
/* Misc Vars */

/* CMDS */
new bool:Cmd_Alw_About = true;
new String:Cmd_Str_About[256] = "sm_ac;sm_aero;sm_aerocontroler";
new String:Cmd_PStr_About[32] = "!ac";
new bool:Cmd_Alw_Refuse = true;
new String:Cmd_Str_Refuse[256] = "sm_r;sm_refuse";
new String:Cmd_PStr_Refuse[32] = "!r";
new bool:Cmd_Alw_Capitulate = true;
new String:Cmd_Str_Capitulate[256] = "sm_c;sm_capitulate";
new String:Cmd_PStr_Capitulate[32] = "!c";
new bool:Cmd_Alw_Dice = true;
new String:Cmd_Str_Dice[256] = "sm_d;sm_dice;sm_rtd";
new String:Cmd_PStr_Dice[32] = "!d";
new bool:Cmd_Alw_DiceMenu = true;
new String:Cmd_Str_DiceMenu[256] = "sm_dicemenu";
new String:Cmd_PStr_DiceMenu[32] = "!dicemenu";
new bool:Cmd_Alw_Kill = true;
new String:Cmd_Str_Kill[256] = "sm_kill;sm_k";
new String:Cmd_PStr_Kill[32] = "!kill";
new bool:Cmd_Alw_ColCmds = true;
new String:Cmd_Str_SetCol[256] = "sm_sc;sm_setcolors";
new String:Cmd_PStr_SetCol[32] = "!sc";
new String:Cmd_Str_DelCol[256] = "sm_dc;sm_deletecolors";
new String:Cmd_PStr_DelCol[32] = "!dc";
new String:Cmd_Str_Noblock[256] = "sm_noblock;sm_nb";
new String:Cmd_PStr_Noblock[32] = "!noblock";
new String:Cmd_Str_Block[256] = "sm_block;sm_b";
new String:Cmd_PStr_Block[32] = "!block";
new bool:Cmd_Alw_Commands = true;
new String:Cmd_Str_Commands[256] = "sm_cmds;sm_commands";
new String:Cmd_PStr_Commands[32] = "!cmds";
new bool:Cmd_Alw_Rules = true;
new String:Cmd_Str_Rules[256] = "sm_rules";
new String:Cmd_PStr_Rules[32] = "!rules";
/* CMDS */

/* Client Vars */
new c_r[MAXPLAYERS + 1];							//available refuse count
new c_rBaseMax[MAXPLAYERS + 1];					//count the maximum of avaiable refuses
new c_rBase[MAXPLAYERS + 1];					//reset refuse count on roundstart
new c_rSubBase[MAXPLAYERS + 1];					//reset refuse count on roundstart one time
new Float:c_rTimeStamp[MAXPLAYERS + 1];		//last time stamp of refusing
new c_c[MAXPLAYERS + 1];							//available capitulate count
new bool:c_cReal[MAXPLAYERS + 1];				//determinates if the client really has capitulated
new bool:c_topC[MAXPLAYERS + 1];				//is client in the top-capitulated queue
new Handle:c_topCDisableTimer[MAXPLAYERS + 1] = { INVALID_HANDLE, ... }; //used for top-capitulating
new c_cBaseMax[MAXPLAYERS + 1];
new c_cBase[MAXPLAYERS + 1];					//reset capitulate count on roundstart
new c_cSubBase[MAXPLAYERS + 1];					//reset capitulate count on roundstart one time
new c_dices[MAXPLAYERS + 1];
new c_dicesBase[MAXPLAYERS + 1];
new c_dicesSubBase[MAXPLAYERS + 1];
new bool:c_rebell[MAXPLAYERS + 1];
new c_DiceState[MAXPLAYERS + 1]; //0=nothing choosen | 1=autodice | 2=no dicing
new Handle:c_DiceMenu[MAXPLAYERS + 1]; //no menu's will be placed here - the array will contain the recessive timers TODO: name it right!
new bool:c_StateNotifyOverride[MAXPLAYERS + 1];
new Handle:c_dropHandler[MAXPLAYERS + 1];
new bool:c_IsDropDenied[MAXPLAYERS + 1];
/* Client Vars */

/* Offsets */
new g_BeamSprite = -1;
new g_HaloSprite = -1;
new g_Offset_CollisionGroup = -1;
/* Offsets */

/* Menus */
new Handle:diceMenu = INVALID_HANDLE;
/* Menus */

/* Country Filter */
new bool:FC_FilterCountries = true;
new bool:FC_FilterCaseSensitive = false;
new bool:FC_FilterMatchExact = false; //true = geoip AND lang, false = geoip OR lang
new bool:FC_AllowWorldIps = true;
new String:FC_KickMessage[256] = "Wrong language to join this server";
new FC_GeoIPCodesCount = 5;
new String:FC_GeoIPCodes[32][4] = { "us", "um", "ca", "au", "gb", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
new FC_LanguageInfosCount = 1;
new String:FC_LanguageInfos[32][4] = { "en", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
/* Country Filter */