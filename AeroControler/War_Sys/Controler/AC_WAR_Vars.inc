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

new ExtensionEntryIndex = -1;

new bool:Alw_warVoteCmd = true;
new String:Str_WarVoteCmd[] = "sm_war;sm_w";
new String:PStr_WarVoteCmd[] = "!war";

#if defined DEBUG
new VoteMinimumParticipate = 0;
new Float:VoteAmount = 0.0;
#else
new VoteMinimumParticipate = 10;
new Float:VoteAmount = 75.0;
#endif

new g_Offset_CollisionGroup = -1;

new bool:VoteIsOpen = true;
new Float:ReAllowVoteTime = 7200.0; //7200seconds = 2 hours
new Handle:ReVoteTimer = INVALID_HANDLE;
new VoteTime = 30;
new bool:AddNoVoteOption = true;

new bool:HasVotedWar[MAXPLAYERS + 1];

new WarCount = 0;
#define MAX_WARS 32
#define MAX_WAR_NAME 64
new bool:War_Registered[MAX_WARS];
new String:War_Name[MAX_WARS][MAX_WAR_NAME];
new Handle:War_Plugin[MAX_WARS];
new WarStart:War_StartFunc[MAX_WARS];
new WarEnd:War_EndFunc[MAX_WARS];
new bool:War_HasFreezeTime[MAX_WARS];
new WarFreezeTimeStart:War_FT_Start[MAX_WARS];
new WarFreezeTimeEnd:War_FT_End[MAX_WARS];

new bool:WarOnline = false;
new WarIndex = 0;
new bool:WarHasFreezeTime = false; new bool:WarInFreezeTime = false; new WarFTCount = 0;
new WarFreezeTime = 40; new Handle:WarFreezeTimer = INVALID_HANDLE;
new WarMaxTime = 1200; //20minutes
new Handle:WarMaxTimer = INVALID_HANDLE;
new WarRound = 0; new WarMaxRounds = 3;
new Float:WarFreezePosition[3];
new Float:WarFreezeEyeAng[3];