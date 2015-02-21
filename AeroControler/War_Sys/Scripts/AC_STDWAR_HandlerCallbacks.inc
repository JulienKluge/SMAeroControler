/*
**
** Aero Jail Controler Plugin
** Standart WARs - Partial handler, callbacks file
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

/*
**
** Start/End Callbacks
**
*/

//nothin' to do here..................

public warcallback_NormalWarStart()
{
	WarOnline = true;
	NormalWar_Online = true;
}
public warcallback_NormalWarEnd()
{
	WarOnline = false;
	NormalWar_Online = false;
}

public warcallback_HCWarStart()
{
	WarOnline = true;
	HCWar_Online = true;
}
public warcallback_HCWarEnd()
{
	WarOnline = false;
	HCWar_Online = false;
}

public warcallback_FunWarStart()
{
	WarOnline = true;
	FunWar_Online = true;
}
public warcallback_FunWarEnd()
{
	WarOnline = false;
	FunWar_Online = false;
}
public warcallback_FunWarFreezeTimeEnd()
{
	ApplyFunWarRules();
}
/*
**
** Start/End Callbacks
**
*/

/*
**
** SDKHooks
**
*/

public Action SDKH_OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3])
{
	if (WarOnline)
	{
		if (HCWar_Online)
		{
			if (IsClientValid(victim) && IsClientValid(attacker))
			{
				damage *= HCWar_DmgFactor;
				return Plugin_Changed;
			}
		}
	}
	return Plugin_Continue;
}

/*
**
** SDKHooks
**
*/