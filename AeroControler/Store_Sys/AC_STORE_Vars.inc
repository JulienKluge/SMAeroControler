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

bool InWork = true;

int ExtensionEntryIndex = -1;

Handle db = INVALID_HANDLE;
char tableName[64] = "ac_store";
char sfTableName[64] = "ac_store_skillforces";
bool db_useDecimals = true;
int db_decimalAbsolutPrecision = 17;
int db_decimalPrecision = 7;

char currentName[64] = "Points";

/*int tCount = 0;
int ctCount = 0;*/

float points[MAXPLAYERS + 1];
float newPoints[MAXPLAYERS + 1];
float virtualNewPoints[MAXPLAYERS + 1];

int sf_member[MAXPLAYERS + 1];
int sf_level[MAXPLAYERS + 1];
float sf_tax[MAXPLAYERS + 1];
bool sf_hasSkill[MAXPLAYERS + 1];
int sf_skill[MAXPLAYERS + 1];
int sf_skilllevel[MAXPLAYERS + 1];

int sf_maximumMemberCount = 32;

float sf_level_Base = 4.0;

#define	Skill_Medic		1
int Skill_Medic_HPPerLevel = 30;
#define	Skill_Fighter		2
float Skill_Fighter_PercentPerLevel = 0.015; //pre-divided by 100
#define	Skill_Defender	3
float Skill_Defender_PercentPerLevel = 0.015; //pre-divided by 100
#define	Skill_Crook		4
float Skill_Crook_PercentPerLevel = 15.0; //pre-multiplicated by 10

Handle skillTimer = INVALID_HANDLE;

float p_PointsMultiplierPublic = 1.0;

float p_FirstJoin = 5.0;
float p_JoinDay = 1.0;
float p_Survive_CT = 0.1;
float p_Survive_T = 0.3;
float p_Round = 0.01;
float p_T_Kill_CT = 0.2;
float p_CT_Kill_T = 0.05;

bool Cmd_Store = true;
char Cmd_Str_Store[256] = "sm_store;sm_shop";
char Cmd_PStr_Store[32] = "!store";
bool Cmd_StoreTop = true;
char Cmd_Str_StoreTop[256] = "sm_storetop;sm_st";
char Cmd_PStr_StoreTop[32] = "!storetop";
bool Cmd_StoreRank = true;
char Cmd_Str_StoreRank[256] = "sm_storerank;sm_sr";
char Cmd_PStr_StoreRank[32] = "!storerank";
bool Cmd_SF = true;
char Cmd_Str_SF[256] = "sm_sf;sm_skillforce";
char Cmd_PStr_SF[32] = "!skillforce";
bool Cmd_SFC = true;
char Cmd_Str_SFC[256] = "sm_sfc;";
char Cmd_PStr_SFC[32] = "!sfc";

float c_MultiplierPrivate[MAXPLAYERS + 1] = { 1.0, ... };
float c_MultiplierPublic[MAXPLAYERS + 1] = { 1.0, ... };
bool c_HasRespawn[MAXPLAYERS + 1];
Handle c_RespawnTimer[MAXPLAYERS + 1];
int c_PressureNadeCount[MAXPLAYERS + 1];
int c_InvisibleSeconds[MAXPLAYERS + 1];
Handle c_InvisCountTimer[MAXPLAYERS + 1];
bool c_InSFNameChooseProcess[MAXPLAYERS + 1];
int c_MedicSkillAmount[MAXPLAYERS + 1];

int g_offset_hasnv = -1;
int g_offset_nvon = -1;
int g_BeamSprite = -1;
int g_HaloSprite = -1;

/* Store Values */
float s_j_dice = 20.0;					//1
float s_j_refuse = 40.0;				//2
float s_j_capitulate = 8.0;			//3
float s_j_diceFive = 50.0;			//4
float s_j_dice_map = 60.0;			//5
float s_j_refuse_map = 100.0;			//6
float s_j_capitulate_map = 20.0;		//7

float s_p_gravity = 5.0;				//8
float s_p_speed = 20.0;				//9
float s_p_hp = 7.5;					//10
float s_p_invisfive = 10.0;			//11
float s_p_invisten = 18.0;			//12

float s_i_smoke = 4.0;				//13
float s_i_flash = 4.0;				//14
float s_i_armor = 3.0;				//15
float s_i_nv = 0.5;					//16
float s_i_pressurehe = 5.0;			//17

float s_m_respawn = 50.0;				//18
float s_m_threepointgain = 5.0;		//19
float s_m_enemypingpulse = 8.0;		//20
float s_m_enemyskin = 35.0;			//21
/* Store Values */