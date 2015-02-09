/*
**
** Aero Jail Controler Plugin
** Standart WARs - Partial events file
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

public Action:Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (IsClientValid(client))
	{
		if (WarOnline)
		{
			if (NormalWar_Online || HCWar_Online || FunWar_Online)
			{
				SetClientDefaultValues(client);
			}
		}
	}
	return Plugin_Continue;
}