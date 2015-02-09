/*
**
** Aero Jail Controler Plugin
** Standart WARs - Partial stocks file
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

stock RegisterWars()
{
	if (!Ready)
	{
		new String:transPhrase[64];
		if (NormalWar_Enabled)
		{
			Format(transPhrase, sizeof(transPhrase), "%T", "STDWARS_NormalWar", LANG_SERVER);
			ac_war_RegWar(warcallback_NormalWarStart, warcallback_NormalWarEnd, transPhrase, true);
		}
		if (HCWar_Enabled)
		{
			Format(transPhrase, sizeof(transPhrase), "%T", "STDWARS_HardcoreWar", LANG_SERVER);
			ac_war_RegWar(warcallback_HCWarStart, warcallback_HCWarEnd, transPhrase, true);
		}
		if (FunWar_Enabled)
		{
			Format(transPhrase, sizeof(transPhrase), "%T", "STDWARS_FunWar", LANG_SERVER);
			ac_war_RegWar(warcallback_FunWarStart, warcallback_FunWarEnd, transPhrase, true, WarFreezeTimeStart:INVALID_FUNCTION, WarFreezeTimeEnd:warcallback_FunWarFreezeTimeEnd);
		}
		Ready = true;
	}
}

stock ApplyFunWarRules() /* It's in a stock, because it would mess the Callback File */
{
	new _:fun_hp = GetRandomInt(FunWar_MinHp, FunWar_MaxHp);
	new Float:fun_speed = GetRandomFloat(1.05, FunWar_MaxSpeed);
	new Float:fun_grav = GetRandomFloat(FunWar_MinGrav, 0.95);
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientValid(i))
		{
			if (IsPlayerAlive(i))
			{
				RemoveAllClientItems(i, CS_SLOT_KNIFE);
				SetClientDefaultValues(i, true, fun_hp, fun_grav, fun_speed);
				new wpnIndex = GetRandomInt(0, 3);
				switch (wpnIndex) /* I fucking hate switch case... */
				{
					case 0: { GivePlayerItem(i, "weapon_m4a1"); }
					case 1: { GivePlayerItem(i, "weapon_awp"); }
					case 2: { GivePlayerItem(i, "weapon_m249"); }
					case 3: { GivePlayerItem(i, "weapon_ak47"); }
				}
				wpnIndex = GetRandomInt(0, 2);
				switch (wpnIndex) /* ...I really hate it! */
				{
					case 0: { GivePlayerItem(i, "weapon_deagle"); }
					case 1: { GivePlayerItem(i, "weapon_p228"); }
					case 2: { GivePlayerItem(i, "weapon_elite"); }
				}
				GivePlayerItem(i, "weapon_flashbang");
				GivePlayerItem(i, "weapon_flashbang");
				GivePlayerItem(i, "weapon_smokegrenade");
				GivePlayerItem(i, "weapon_hegrenade");
			}
		}
	}
	AC_PrintToChatAll("%t", "STDWARS_FunWar_ApplyProperties", fun_hp, RoundFloat((fun_speed - 1.0) * 100), RoundFloat((1.0 - fun_grav) * 100));
}