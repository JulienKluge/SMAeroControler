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

new bool:IncludeTag = true;
new String:Tag[32] = "\x071188FF[Jail]:\x01";

stock AC_PrintToChat(client, const String:msg[], any:...)
{
	decl String:message[255]; //not 192 because we perhaps need more without display more than 192
	SetGlobalTransTarget(client);
	VFormat(message, sizeof(message), msg, 3);
	if (IncludeTag)
	{ Format(message, sizeof(message), "\x01%s %s", Tag, message); }
	else
	{ Format(message, sizeof(message), "\x01%s", message); }
	new Handle:buffer = StartMessageOne("SayText2", client, USERMSG_RELIABLE | USERMSG_BLOCKHOOKS);
	BfWriteByte(buffer, client);
	BfWriteByte(buffer, true);
	BfWriteString(buffer, message);
	EndMessage();
}

stock AC_PrintToChatAll(const String:msg[], any:...)
{
	decl String:message[255]; //not 192 because we perhaps need more without display more than 192
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			SetGlobalTransTarget(i);
			VFormat(message, sizeof(message), msg, 2);
			if (IncludeTag)
			{ Format(message, sizeof(message), "\x01%s %s", Tag, message); }
			else
			{ Format(message, sizeof(message), "\x01%s", message); }
			new Handle:buffer = StartMessageOne("SayText2", i, USERMSG_RELIABLE | USERMSG_BLOCKHOOKS); //why I don't use AC_PrintToChat? Because 2 times a VFormat, Format & SetGlobalTransTarget is sensless!
			BfWriteByte(buffer, i);
			BfWriteByte(buffer, true);
			BfWriteString(buffer, message);
			EndMessage();
		}
	}
}

stock AC_PrintKeyText(client, const String:msg[], any:...)
{
	decl String:message[255];
	SetGlobalTransTarget(client);
	VFormat(message, sizeof(message), msg, 3);
	new Handle:userMessage = StartMessageOne("KeyHintText", client);
	BfWriteByte(userMessage, 1);
	BfWriteString(userMessage, message);
	EndMessage();
}

stock AC_Notify(client, const String:transPhrase[], const String:targetPhrase[], bool:isMl)
{
	decl String:originName[MAX_NAME_LENGTH];
	for (new i = 1; i <= MaxClients; i++)
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

stock AC_RawNotify(client, const String:transPhrase[])
{
	decl String:originName[MAX_NAME_LENGTH];
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			FormatActivitySource(client, i, originName, sizeof(originName));
			AC_PrintToChat(i, "%s %t", originName, transPhrase);
		}
	}
}

stock RegMultipleConsoleCmd(const String:cmd[], ConCmd:callback, const String:description[]="", flags=0)
{
	new String:cmd_parts[16][32];
	new stringCount = ExplodeString(cmd, ";", cmd_parts, 16, 32);
	if (stringCount > 0)
	{
		for (new i = 0; i < stringCount; i++)
		{
			if (strlen(cmd_parts[i]) > 0)
			{ RegConsoleCmd(cmd_parts[i], callback, description, flags); }
		}
	}
}

stock GetFirstDelimitedCmd(const String:cmds[], String:cmd[])
{
	decl String:cmd_parts[2][32];
	new stringCount = ExplodeString(cmds, ";", cmd_parts, 2, 32);
	if (stringCount > 0)
	{
		Format(cmd, 32, "%s", cmd_parts[0]);
	}
}

stock WhiteStripIllegalCharacters(String:str[])
{
	new strlength = strlen(str) - 1;
	for (new i = 0; i < strlength; i++)
	{
		if (str[i] < 32 || str[i] > 126)
		{ str[i] = ' '; }
	}
}