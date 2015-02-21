/*
**
** Aero Jail Controler Plugin
** Shared Client System
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
#if defined _AC_CLIENTSYS_included
	#endinput
#endif
#define _AC_CLIENTSYS_included

stock bool IsClientValid(int client)
{
	if (client > 0 && client <= MaxClients)
	{
		if (IsClientInGame(client))
		{
			if (IsClientConnected(client))
			{
				return true;
			}
		}
	}
	return false;
}

stock void SetClientDefaultValues(int client, bool changed=false, int hp=100, float gravity=1.0, float speed=1.0)
{
	if (changed)
	{
		if (hp >= 0) { SetEntityHealth(client, hp); }
		if (gravity >= 0.0) { SetEntityGravity(client, gravity); }
		if (speed >= 0.0) { SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", speed); }
	}
	else
	{
		SetEntityHealth(client, 100);
		SetEntityGravity(client, 1.0);
		SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", 1.0);
	}
}

stock void RemoveAllClientItems(int client, int exception = -1)
{
	int entity = -1;
	for (int i = 0; i < 5; i++)
	{
		if (i != exception)
		{
			while ((entity = GetPlayerWeaponSlot(client, i)) != -1)
			{
				if (IsValidEntity(entity))
				{ RemovePlayerItem(client, entity); }
			}
		}
	}
}

stock void ApplyClientFade(int client, int duration, int color[4], bool fadeIn = true, bool stayOut = false, bool purge = true, int holdTime = 0)
{
	int flags = (fadeIn) ? 0x1 : 0x2; //i don't really often use ternary operators - but in this case, it seemed legit
	if (stayOut) { flags |= 0x8; }
	if (purge) { flags |= 0x10; }
	Handle fadeMsg = StartMessageOne("Fade", client);
	BfWriteShort(fadeMsg, duration);
	BfWriteShort(fadeMsg, holdTime);
	BfWriteShort(fadeMsg, flags);
	BfWriteByte(fadeMsg, color[0]); //R
	BfWriteByte(fadeMsg, color[1]); //G
	BfWriteByte(fadeMsg, color[2]); //B
	BfWriteByte(fadeMsg, color[3]); //A
	EndMessage();
}