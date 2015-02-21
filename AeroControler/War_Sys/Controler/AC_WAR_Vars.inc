/*
**
** Aero Jail Controler Plugin
** WAR Base Control System - partial variables file
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

#define COLLISION_GROUP_DEBRIS_TRIGGER 2

int ExtensionEntryIndex = -1;

bool Alw_warVoteCmd = true;
char Str_WarVoteCmd[] = "sm_war;sm_w";
char PStr_WarVoteCmd[] = "!war";

#if defined DEBUG
int VoteMinimumParticipate = 0;
float VoteAmount = 0.0;
#else
int VoteMinimumParticipate = 10;
float VoteAmount = 75.0;
#endif

int g_Offset_CollisionGroup = -1;

bool VoteIsOpen = true;
float ReAllowVoteTime = 7200.0; //7200seconds = 2 hours
Handle ReVoteTimer = INVALID_HANDLE;
int VoteTime = 30;
bool AddNoVoteOption = true;

bool HasVotedWar[MAXPLAYERS + 1];

int WarCount = 0;
#define MAX_WARS 32
#define MAX_WAR_NAME 64
bool War_Registered[MAX_WARS];
char War_Name[MAX_WARS][MAX_WAR_NAME];
Handle War_Plugin[MAX_WARS];
WarStart War_StartFunc[MAX_WARS];
WarEnd War_EndFunc[MAX_WARS];
bool War_HasFreezeTime[MAX_WARS];
WarFreezeTimeStart War_FT_Start[MAX_WARS];
WarFreezeTimeEnd War_FT_End[MAX_WARS];

bool WarOnline = false;
int WarIndex = 0;
bool WarHasFreezeTime = false;
bool WarInFreezeTime = false;
int WarFTCount = 0;
int WarFreezeTime = 40;
Handle WarFreezeTimer = INVALID_HANDLE;
int WarMaxTime = 1200; //20minutes
Handle WarMaxTimer = INVALID_HANDLE;
int WarRound = 0; int WarMaxRounds = 3;
float WarFreezePosition[3];
float WarFreezeEyeAng[3];