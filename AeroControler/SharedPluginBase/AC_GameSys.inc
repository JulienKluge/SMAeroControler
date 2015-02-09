/*
**
** Aero Jail Controler Plugin
** Shared Game-Detection System
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
#if defined _AC_GAMESYS_included
	#endinput
#endif
#define _AC_GAMESYS_included

#include "AC_ErrorSys.inc"

#define Game_Css 1
#define Game_Csgo 2
#define Game_None 0

int g_Game = Game_None;

stock DetectGameMod()
{
	char gameName[MAX_NAME_LENGTH]; //name length is enough...
	GetGameFolderName(gameName, MAX_NAME_LENGTH);
	if (StrEqual(gameName, "cstrike", false))
	{
		g_Game = Game_Css;
	}
	else if (StrEqual(gameName, "csgo", false))
	{
		g_Game = Game_Csgo;
	}
	else
	{
		Log(EL_Abort, "core", "Could not retrieve running gamemod.");
	}
}