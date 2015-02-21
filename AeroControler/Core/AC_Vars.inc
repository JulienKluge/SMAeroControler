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
char PLUGIN_VERSION[32];
bool IntroduceMe = true;
char serverChatColor[7] = "1188FF";
char serverStaticChatColor[7] = "1188FF";
char serverTag[32] = "[Jail]:";
Handle tagChangedForward = INVALID_HANDLE;
Handle clientRoundReadyForward = INVALID_HANDLE;
Handle cmdMenuBuildForward = INVALID_HANDLE;
Handle coreStateForward = INVALID_HANDLE;
char gameDescription[64] = "JailBreak CSS";
Handle extensionPlugins = INVALID_HANDLE;
/* Server Vars */

/* Misc Vars */
bool DropAll_Active = true;
int tCount = 0;
int tAliveCount = 0;
int ctCount = 0;
int ctAliveCount = 0;
bool OverrideWorkToFalse = false;
bool InWork = true;
int StateNotifyCounter = 0;
int NoBlockState = 2; new RoundStartedState = 2; //we need the second variable when changes are made in the round
int GunSafetyState = 0; //-1=disabled | 0=Not initialized | 1=initialized | 2=init failed
int ctSpawnWeaponCount = 3;
char ctSpawnWeapons[10][32] = { "weapon_m4a1", "weapon_deagle", "weapon_knife", "", "", "", "", "", "", "" };
int tSpawnWeaponCount = 1;
char tSpawnWeapons[10][32] = { "weapon_knife", "", "", "", "", "", "", "", "", "" };
Handle aboutMenu = INVALID_HANDLE;
Handle aboutBackMenu = INVALID_HANDLE;
Handle ruleMenu = INVALID_HANDLE;
Handle ruleSubMenus = INVALID_HANDLE;
bool announce_dice = true;
bool announce_rules = true;
bool announce_cmds = true;
float announce_delay = 600.0;
bool autoshow_dicemenu = true;
Handle announce_Timer = INVALID_HANDLE;
/* Misc Vars */

/* CMDS */
bool Cmd_Alw_About = true;
char Cmd_Str_About[256] = "sm_ac;sm_aero;sm_aerocontroler";
char Cmd_PStr_About[32] = "!ac";
bool Cmd_Alw_Refuse = true;
char Cmd_Str_Refuse[256] = "sm_r;sm_refuse";
char Cmd_PStr_Refuse[32] = "!r";
bool Cmd_Alw_Capitulate = true;
char Cmd_Str_Capitulate[256] = "sm_c;sm_capitulate";
char Cmd_PStr_Capitulate[32] = "!c";
bool Cmd_Alw_Dice = true;
char Cmd_Str_Dice[256] = "sm_d;sm_dice;sm_rtd";
char Cmd_PStr_Dice[32] = "!d";
bool Cmd_Alw_DiceMenu = true;
char Cmd_Str_DiceMenu[256] = "sm_dicemenu";
char Cmd_PStr_DiceMenu[32] = "!dicemenu";
bool Cmd_Alw_Kill = true;
char Cmd_Str_Kill[256] = "sm_kill;sm_k";
char Cmd_PStr_Kill[32] = "!kill";
bool Cmd_Alw_ColCmds = true;
char Cmd_Str_SetCol[256] = "sm_sc;sm_setcolors";
char Cmd_PStr_SetCol[32] = "!sc";
char Cmd_Str_DelCol[256] = "sm_dc;sm_deletecolors";
char Cmd_PStr_DelCol[32] = "!dc";
char Cmd_Str_Noblock[256] = "sm_noblock;sm_nb";
char Cmd_PStr_Noblock[32] = "!noblock";
char Cmd_Str_Block[256] = "sm_block;sm_b";
char Cmd_PStr_Block[32] = "!block";
bool Cmd_Alw_Commands = true;
char Cmd_Str_Commands[256] = "sm_cmds;sm_commands";
char Cmd_PStr_Commands[32] = "!cmds";
bool Cmd_Alw_Rules = true;
char Cmd_Str_Rules[256] = "sm_rules";
char Cmd_PStr_Rules[32] = "!rules";
/* CMDS */

/* Client Vars */
int c_r[MAXPLAYERS + 1];							//available refuse count
int c_rBaseMax[MAXPLAYERS + 1];					//count the maximum of avaiable refuses
int c_rBase[MAXPLAYERS + 1];					//reset refuse count on roundstart
int c_rSubBase[MAXPLAYERS + 1];					//reset refuse count on roundstart one time
float c_rTimeStamp[MAXPLAYERS + 1];		//last time stamp of refusing
int c_c[MAXPLAYERS + 1];							//available capitulate count
bool c_cReal[MAXPLAYERS + 1];				//determinates if the client really has capitulated
bool c_topC[MAXPLAYERS + 1];				//is client in the top-capitulated queue
Handle c_topCDisableTimer[MAXPLAYERS + 1] = { INVALID_HANDLE, ... }; //used for top-capitulating
int c_cBaseMax[MAXPLAYERS + 1];
int c_cBase[MAXPLAYERS + 1];					//reset capitulate count on roundstart
int c_cSubBase[MAXPLAYERS + 1];					//reset capitulate count on roundstart one time
int c_dices[MAXPLAYERS + 1];
int c_dicesBase[MAXPLAYERS + 1];
int c_dicesSubBase[MAXPLAYERS + 1];
bool c_rebell[MAXPLAYERS + 1];
int c_DiceState[MAXPLAYERS + 1]; //0=nothing choosen | 1=autodice | 2=no dicing
Handle c_DiceMenu[MAXPLAYERS + 1]; //no menu's will be placed here - the array will contain the recessive timers TODO: name it right!
bool c_StateNotifyOverride[MAXPLAYERS + 1];
Handle c_dropHandler[MAXPLAYERS + 1];
bool c_IsDropDenied[MAXPLAYERS + 1];
/* Client Vars */

/* Offsets */
int g_BeamSprite = -1;
int g_HaloSprite = -1;
int g_Offset_CollisionGroup = -1;
/* Offsets */

/* Menus */
Handle diceMenu = INVALID_HANDLE;
/* Menus */

/* Country Filter */
bool FC_FilterCountries = true;
bool FC_FilterCaseSensitive = false;
bool FC_FilterMatchExact = false; //true = geoip AND lang, false = geoip OR lang
bool FC_AllowWorldIps = true;
char FC_KickMessage[256] = "Wrong language to join this server";
int FC_GeoIPCodesCount = 5;
char FC_GeoIPCodes[32][4] = { "us", "um", "ca", "au", "gb", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
int FC_LanguageInfosCount = 1;
char FC_LanguageInfos[32][4] = { "en", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
/* Country Filter */