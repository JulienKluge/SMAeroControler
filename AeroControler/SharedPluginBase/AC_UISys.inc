/*
**
** Aero Jail Controler Plugin
** Shared UI System
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
#if defined _AC_UISYS_included
	#endinput
#endif
#define _AC_UISYS_included

#include "AC_GameSys.inc"

bool IncludeTag = true;
char Tag[32] = "\x071188FF[Jail]:\x01";

stock void AC_PrintToChat(int client, const char[] msg, any ...)
{
	char message[255]; //not 192 because we perhaps need more without display more than 192
	SetGlobalTransTarget(client);
	VFormat(message, sizeof(message), msg, 3);
	if (IncludeTag)
	{ Format(message, sizeof(message), "\x01%s %s", Tag, message); }
	else
	{ Format(message, sizeof(message), "\x01%s", message); }
	Handle buffer = StartMessageOne("SayText2", client, USERMSG_RELIABLE | USERMSG_BLOCKHOOKS);
	if (g_Game == Game_Css)
	{
		BfWriteByte(buffer, client);
		BfWriteByte(buffer, true);
		BfWriteString(buffer, message);
	}
	else if (g_Game == Game_Csgo)
	{
		//TODO
	}
	EndMessage();
}

stock void AC_PrintToChatAll(char[] msg, any ...)
{
	char message[255]; //not 192 because we perhaps need more without display more than 192
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			SetGlobalTransTarget(i);
			VFormat(message, sizeof(message), msg, 2);
			if (IncludeTag)
			{ Format(message, sizeof(message), "\x01%s %s", Tag, message); }
			else
			{ Format(message, sizeof(message), "\x01%s", message); }
			Handle buffer = StartMessageOne("SayText2", i, USERMSG_RELIABLE | USERMSG_BLOCKHOOKS); //why I don't use AC_PrintToChat? Because 2 times a VFormat, Format & SetGlobalTransTarget is sensless!
			if (g_Game == Game_Css)
			{
				BfWriteByte(buffer, i);
				BfWriteByte(buffer, true);
				BfWriteString(buffer, message);
			}
			else if (g_Game == Game_Csgo)
			{
				//TODO
			}
			EndMessage();
		}
	}
}

stock void AC_PrintKeyText(int client, char[] msg, any ...)
{
	char message[255];
	SetGlobalTransTarget(client);
	VFormat(message, sizeof(message), msg, 3);
	Handle userMessage = StartMessageOne("KeyHintText", client);
	if (g_Game == Game_Css)
	{
		BfWriteByte(userMessage, 1);
		BfWriteString(userMessage, message);
	}
	else if (g_Game == Game_Csgo)
	{
		//TODO
	}
	EndMessage();
}

stock void AC_Notify(int client, const char[] transPhrase, const char[] targetPhrase, bool isMl)
{
	char originName[MAX_NAME_LENGTH];
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			FormatActivitySource(client, i, originName, sizeof(originName));
			if (isMl)
			{
				AC_PrintToChat(i, "%s %t", originName, transPhrase, targetPhrase);
			}
			else
			{
				AC_PrintToChat(i, "%s %t", originName, transPhrase, "_s", targetPhrase);
			}
		}
	}
}

stock void AC_RawNotify(int client, const char[] transPhrase)
{
	char originName[MAX_NAME_LENGTH];
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			FormatActivitySource(client, i, originName, sizeof(originName));
			AC_PrintToChat(i, "%s %t", originName, transPhrase);
		}
	}
}

stock void RegMultipleConsoleCmd(const char[] cmd, ConCmd callback, const char [] description = "", int flags=0)
{
	char cmd_parts[16][32];
	int stringCount = ExplodeString(cmd, ";", cmd_parts, 16, 32);
	if (stringCount > 0)
	{
		for (int i = 0; i < stringCount; i++)
		{
			if (strlen(cmd_parts[i]) > 0)
			{ RegConsoleCmd(cmd_parts[i], callback, description, flags); }
		}
	}
}

stock void GetFirstDelimitedCmd(char[] cmds, char[] cmd)
{
	char cmd_parts[2][32];
	int stringCount = ExplodeString(cmds, ";", cmd_parts, 2, 32);
	if (stringCount > 0)
	{
		Format(cmd, 32, "%s", cmd_parts[0]);
	}
}

stock void WhiteStripIllegalCharacters(char[] str)
{
	int strlength = strlen(str) - 1;
	for (int i = 0; i < strlength; i++)
	{
		if (str[i] < 32 || str[i] > 126)
		{ str[i] = ' '; }
	}
}