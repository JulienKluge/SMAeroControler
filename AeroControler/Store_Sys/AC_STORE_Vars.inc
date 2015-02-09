/*
**
** Aero Jail Controler Plugin
** Store System - partial variables file
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

new bool:InWork = true;

new ExtensionEntryIndex = -1;

new Handle:db = INVALID_HANDLE;
new String:tableName[64] = "ac_store";
new String:sfTableName[64] = "ac_store_skillforces";
new bool:db_useDecimals = true;
new db_decimalAbsolutPrecision = 17;
new db_decimalPrecision = 7;

new String:currentName[64] = "Points";

/*new tCount = 0;
new ctCount = 0;*/

new Float:points[MAXPLAYERS + 1];
new Float:newPoints[MAXPLAYERS + 1];
new Float:virtualNewPoints[MAXPLAYERS + 1];

new sf_member[MAXPLAYERS + 1];
new sf_level[MAXPLAYERS + 1];
new Float:sf_tax[MAXPLAYERS + 1];
new bool:sf_hasSkill[MAXPLAYERS + 1];
new sf_skill[MAXPLAYERS + 1];
new sf_skilllevel[MAXPLAYERS + 1];

new sf_maximumMemberCount = 32;

new Float:sf_level_Base = 4.0;

#define	Skill_Medic		1
new Skill_Medic_HPPerLevel = 30;
#define	Skill_Fighter		2
new Float:Skill_Fighter_PercentPerLevel = 0.015; //pre-divided by 100
#define	Skill_Defender	3
new Float:Skill_Defender_PercentPerLevel = 0.015; //pre-divided by 100
#define	Skill_Crook		4
new Float:Skill_Crook_PercentPerLevel = 15.0; //pre-multiplicated by 10

new Handle:skillTimer = INVALID_HANDLE;

new Float:p_PointsMultiplierPublic = 1.0;

new Float:p_FirstJoin = 5.0;
new Float:p_JoinDay = 1.0;
new Float:p_Survive_CT = 0.1;
new Float:p_Survive_T = 0.3;
new Float:p_Round = 0.01;
new Float:p_T_Kill_CT = 0.2;
new Float:p_CT_Kill_T = 0.05;

new bool:Cmd_Store = true;
new String:Cmd_Str_Store[256] = "sm_store;sm_shop";
new String:Cmd_PStr_Store[32] = "!store";
new bool:Cmd_StoreTop = true;
new String:Cmd_Str_StoreTop[256] = "sm_storetop;sm_st";
new String:Cmd_PStr_StoreTop[32] = "!storetop";
new bool:Cmd_StoreRank = true;
new String:Cmd_Str_StoreRank[256] = "sm_storerank;sm_sr";
new String:Cmd_PStr_StoreRank[32] = "!storerank";
new bool:Cmd_SF = true;
new String:Cmd_Str_SF[256] = "sm_sf;sm_skillforce";
new String:Cmd_PStr_SF[32] = "!skillforce";
new bool:Cmd_SFC = true;
new String:Cmd_Str_SFC[256] = "sm_sfc;";
new String:Cmd_PStr_SFC[32] = "!sfc";

new Float:c_MultiplierPrivate[MAXPLAYERS + 1] = { 1.0, ... };
new Float:c_MultiplierPublic[MAXPLAYERS + 1] = { 1.0, ... };
new bool:c_HasRespawn[MAXPLAYERS + 1];
new Handle:c_RespawnTimer[MAXPLAYERS + 1];
new c_PressureNadeCount[MAXPLAYERS + 1];
new c_InvisibleSeconds[MAXPLAYERS + 1];
new Handle:c_InvisCountTimer[MAXPLAYERS + 1];
new bool:c_InSFNameChooseProcess[MAXPLAYERS + 1];
new c_MedicSkillAmount[MAXPLAYERS + 1];

new g_offset_hasnv = -1;
new g_offset_nvon = -1;
new g_BeamSprite = -1;
new g_HaloSprite = -1;

/* Store Values */
new Float:s_j_dice = 20.0;					//1
new Float:s_j_refuse = 40.0;				//2
new Float:s_j_capitulate = 8.0;			//3
new Float:s_j_diceFive = 50.0;			//4
new Float:s_j_dice_map = 60.0;			//5
new Float:s_j_refuse_map = 100.0;			//6
new Float:s_j_capitulate_map = 20.0;		//7

new Float:s_p_gravity = 5.0;				//8
new Float:s_p_speed = 20.0;				//9
new Float:s_p_hp = 7.5;					//10
new Float:s_p_invisfive = 10.0;			//11
new Float:s_p_invisten = 18.0;			//12

new Float:s_i_smoke = 4.0;				//13
new Float:s_i_flash = 4.0;				//14
new Float:s_i_armor = 3.0;				//15
new Float:s_i_nv = 0.5;					//16
new Float:s_i_pressurehe = 5.0;			//17

new Float:s_m_respawn = 50.0;				//18
new Float:s_m_threepointgain = 5.0;		//19
new Float:s_m_enemypingpulse = 8.0;		//20
new Float:s_m_enemyskin = 35.0;			//21
/* Store Values */