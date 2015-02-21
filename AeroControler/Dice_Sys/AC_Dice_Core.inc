/*
**
** Aero Jail Controler Plugin
** Dice Core
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
** The following variables are not the dice probabilities.
** They hold their maximum percent limit. (That is easier to solve)
** Their probability can calculated via subtract the next with the current.
** At the moment, all 50 possibilities have a probability at 2% .
*/
float d_pistol = 2.0;
float d_he = 4.0;
float d_flash = 6.0;
float d_smoke = 8.0;
float d_inchp = 10.0;
float d_dechp = 12.0;
float d_incspeed = 14.0;
float d_decspeed = 16.0;
float d_longjump = 18.0;
float d_frogjump = 20.0;
float d_doublejump = 22.0;
float d_fire = 24.0;
float d_noclip = 26.0;
float d_freeze = 28.0;
float d_lowgrav = 30.0;
float d_highgrav = 32.0;
float d_godmode = 34.0;
float d_nv = 36.0;
float d_invis = 38.0;
float d_model = 40.0;
float d_kevlar = 42.0;
float d_cfour = 44.0;
float d_expcfour = 46.0;
float d_throwingknife = 48.0;
float d_incfrags = 50.0;
float d_decfrags = 52.0;
float d_hologram = 54.0;
float d_blind = 56.0;
float d_increfuses = 58.0;
float d_decrefuses = 60.0;
float d_ctbullymenu = 62.0;
float d_goodtmenu = 64.0;
float d_badtmenu = 66.0;
float d_mirrordmg = 68.0;
float d_vampiredmg = 70.0;
float d_incdmg = 72.0;
float d_decdmg = 74.0;
float d_drugs = 76.0;
float d_randtelechange = 78.0;
float d_entshoot = 80.0;
float d_regenerate = 82.0;
float d_disease = 84.0;
float d_teamheal = 86.0;
float d_death_exp = 88.0;
float d_death_degenerate = 90.0;
float d_death_lightning = 92.0;
float d_wpnrestrict = 94.0;
float d_mine = 96.0;
float d_inccapitulates = 98.0;
float d_deccapitulates = 100.0;

public Action Cmd_Dice(int client, int args)
{
	if (!InWork) { return Plugin_Handled; }
	ThrowDiceChecked(client);
	return Plugin_Handled;
}

public Action Cmd_DiceMenu(int client, int args)
{
	if (!InWork) { return Plugin_Handled; }
	if (GetClientTeam(client) != CS_TEAM_T) { AC_PrintToChat(client, "%t", "denied_not_t"); return Plugin_Handled; }
	DisplayDiceMenu(client);
	return Plugin_Handled;
}

stock void ThrowDiceChecked(int client, bool silentFail = false, float value = -1.0, bool forced = false)
{
	if (!InWork) { return; }
	if (GetClientTeam(client) != CS_TEAM_T)
	{
		if (!silentFail) { AC_PrintToChat(client, "%t", "denied_not_t"); }
		return;
	}
	if (!IsPlayerAlive(client))
	{
		if (!silentFail) { AC_PrintToChat(client, "%t", "denied_not_alive"); }
		return;
	}
	if (c_dices[client] < 1)
	{
		if (!silentFail) { AC_PrintToChat(client, "%t", "d_denied_count"); }
		return;
	}
	c_dices[client]--; //we are now diceable
	ThrowDice(client, value, forced);
}

stock void ThrowDice(int client, float val = -1.0, bool forced = false) //function without any checking
{
	float value = val;
	int iter = 0;
	bool passThrough;
	/*
	** Well, you should aksing why the dice actions are encapsulated by a while loop.
	** The dice system is recessive that means, if an action cannot be performed out
	** of several reasons (it's sensless to dice a gun if you already have one), the
	** loop will be began from the start. If it was succesfull, we jump out of the loop.
	** If we hit a special limit of iterations we'll going to say, that the user diced 
	** nothing. But i approximate it to an 1 to 1 trillion event. (see below)
	*/
	while (iter < 16)
	{
		passThrough = false;
		iter++;
		if (value < 0.0) { value = GetURandomFloat() * 100.0; }
		if (value <= d_pistol) { passThrough = Diced_Pistol(client); }
		else if (value <= d_he) { passThrough = Diced_HE(client); }
		else if (value <= d_flash) { passThrough = Diced_Flash(client); }
		else if (value <= d_smoke) { passThrough = Diced_Smoke(client); }
		else if (value <= d_inchp) { passThrough = Diced_IncHP(client); }
		else if (value <= d_dechp) { passThrough = Diced_DecHP(client); }
		else if (value <= d_incspeed) { passThrough = Diced_IncSpeed(client); }
		else if (value <= d_decspeed) { passThrough = Diced_DecSpeed(client); }
		else if (value <= d_longjump) { passThrough = Diced_LongJump(client); }
		else if (value <= d_frogjump) { passThrough = Diced_FrogJump(client); }
		else if (value <= d_doublejump) { passThrough = Diced_DoubleJump(client); }
		else if (value <= d_fire) { passThrough = Diced_Fire(client); }
		else if (value <= d_noclip) { passThrough = Diced_Noclip(client); }
		else if (value <= d_freeze) { passThrough = Diced_Freeze(client); }
		else if (value <= d_lowgrav) { passThrough = Diced_LowGrav(client); }
		else if (value <= d_highgrav) { passThrough = Diced_HighGrav(client); }
		else if (value <= d_godmode) { passThrough = Diced_GodMode(client); }
		else if (value <= d_nv) { passThrough = Diced_NV(client); }
		else if (value <= d_invis) { passThrough = Diced_Invis(client); }
		else if (value <= d_model) { passThrough = Diced_Model(client); }
		else if (value <= d_kevlar) { passThrough = Diced_Kevlar(client); }
		else if (value <= d_cfour) { passThrough = Diced_CFour(client); }
		else if (value <= d_expcfour) { passThrough = Diced_ExpCFour(client); }
		else if (value <= d_throwingknife) { passThrough = Diced_ThrowingKnife(client); }
		else if (value <= d_incfrags) { passThrough = Diced_IncFrags(client); }
		else if (value <= d_decfrags) { passThrough = Diced_DecFrags(client); }
		else if (value <= d_hologram) { passThrough = Diced_Hologram(client); }
		else if (value <= d_blind) { passThrough = Diced_Blind(client); }
		else if (value <= d_increfuses) { passThrough = Diced_IncRefuses(client); }
		else if (value <= d_decrefuses) { passThrough = Diced_DecRefuses(client); }
		else if (value <= d_ctbullymenu) { passThrough = Diced_CTBullyMenu(client); }
		else if (value <= d_goodtmenu) { passThrough = Diced_GoodTMenu(client); }
		else if (value <= d_badtmenu) { passThrough = Diced_BadTMenu(client); }
		else if (value <= d_mirrordmg) { passThrough = Diced_MirrorDmg(client); }
		else if (value <= d_vampiredmg) { passThrough = Diced_VampireDmg(client); }
		else if (value <= d_incdmg) { passThrough = Diced_IncDmg(client); }
		else if (value <= d_decdmg) { passThrough = Diced_DecDmg(client); }
		else if (value <= d_drugs) { passThrough = Diced_Drugs(client); }
		else if (value <= d_randtelechange) { passThrough = Diced_RandTeleChange(client); }
		else if (value <= d_entshoot) { passThrough = Diced_EntShoot(client); }
		else if (value <= d_regenerate) { passThrough = Diced_Regenerate(client); }
		else if (value <= d_disease) { passThrough = Diced_Disease(client); }
		else if (value <= d_teamheal) { passThrough = Diced_TeamHeal(client); }
		else if (value <= d_death_exp) { passThrough = Diced_Death_Exp(client); }
		else if (value <= d_death_degenerate) { passThrough = Diced_Death_Degenerate(client); }
		else if (value <= d_death_lightning) { passThrough = Diced_Death_Lightning(client); }
		else if (value <= d_wpnrestrict) { passThrough = Diced_WpnRestrict(client); }
		else if (value <= d_mine) { passThrough = Diced_Mine(client); }
		else if (value <= d_inccapitulates) { passThrough = Diced_IncCapitulates(client); }
		else if (value <= d_deccapitulates) { passThrough = Diced_DecCapitulates(client); }
		else { passThrough = Diced_Nothing(client); }
		value = -1.0;
		if (passThrough) 
		{ break; }
		else 
		{
			if (forced)
			{ Diced_Nothing(client); break; } //When an admin forced an dice on a client which could not performed, then block the redicing!
		}
	}
	if (iter > 14) { Diced_Nothing(client); } //TODO: calculate how big this possibility is in the worst case (i approximate it to a 1 to 1 trillion event)
}

stock float ResolveDicePhrase(const char[] phrase) //used for the admin cmd
{
	if (StrEqual(phrase, "pistol", true)) { return d_pistol; }
	if (StrEqual(phrase, "he")) { return d_he; }
	if (StrEqual(phrase, "flash")) { return d_flash; }
	if (StrEqual(phrase, "smoke")) { return d_smoke; }
	if (StrEqual(phrase, "inchp")) { return d_inchp; }
	if (StrEqual(phrase, "dechp")) { return d_dechp; }
	if (StrEqual(phrase, "incspeed")) { return d_incspeed; }
	if (StrEqual(phrase, "decspeed")) { return d_decspeed; }
	if (StrEqual(phrase, "longjump")) { return d_longjump; }
	if (StrEqual(phrase, "frogjump")) { return d_frogjump; }
	if (StrEqual(phrase, "doublejump")) { return d_doublejump; }
	if (StrEqual(phrase, "fire")) { return d_fire; }
	if (StrEqual(phrase, "noclip")) { return d_noclip; }
	if (StrEqual(phrase, "freeze")) { return d_freeze; }
	if (StrEqual(phrase, "lowgrav")) { return d_lowgrav; }
	if (StrEqual(phrase, "highgrav")) { return d_highgrav; }
	if (StrEqual(phrase, "godmode")) { return d_godmode; }
	if (StrEqual(phrase, "nv")) { return d_nv; }
	if (StrEqual(phrase, "invis")) { return d_invis; }
	if (StrEqual(phrase, "model")) { return d_model; }
	if (StrEqual(phrase, "kevlar")) { return d_kevlar; }
	if (StrEqual(phrase, "cfour")) { return d_cfour; }
	if (StrEqual(phrase, "expcfour")) { return d_expcfour; }
	if (StrEqual(phrase, "throwingknife")) { return d_throwingknife; }
	if (StrEqual(phrase, "incfrags")) { return d_incfrags; }
	if (StrEqual(phrase, "decfrags")) { return d_decfrags; }
	if (StrEqual(phrase, "hologram")) { return d_hologram; }
	if (StrEqual(phrase, "blind")) { return d_blind; }
	if (StrEqual(phrase, "increfuses")) { return d_increfuses; }
	if (StrEqual(phrase, "decrefuses")) { return d_decrefuses; }
	if (StrEqual(phrase, "ctbullymenu")) { return d_ctbullymenu; }
	if (StrEqual(phrase, "goodtmenu")) { return d_goodtmenu; }
	if (StrEqual(phrase, "badtmenu")) { return d_badtmenu; }
	if (StrEqual(phrase, "mirrordmg")) { return d_mirrordmg; }
	if (StrEqual(phrase, "vampiredmg")) { return d_vampiredmg; }
	if (StrEqual(phrase, "incdmg")) { return d_incdmg; }
	if (StrEqual(phrase, "decdmg")) { return d_decdmg; }
	if (StrEqual(phrase, "drugs")) { return d_drugs; }
	if (StrEqual(phrase, "randtelechange")) { return d_randtelechange; }
	if (StrEqual(phrase, "entshoot")) { return d_entshoot; }
	if (StrEqual(phrase, "regenerate")) { return d_regenerate; }
	if (StrEqual(phrase, "disease")) { return d_disease; }
	if (StrEqual(phrase, "teamheal")) { return d_teamheal; }
	if (StrEqual(phrase, "death_exp")) { return d_death_exp; }
	if (StrEqual(phrase, "death_degenerate")) { return d_death_degenerate; }
	if (StrEqual(phrase, "death_lightning")) { return d_death_lightning; }
	if (StrEqual(phrase, "wpnrestrict")) { return d_wpnrestrict; }
	if (StrEqual(phrase, "mine")) { return d_mine; }
	if (StrEqual(phrase, "inccapitulates")) { return d_inccapitulates; }
	if (StrEqual(phrase, "deccapitulates")) { return d_deccapitulates; }
	return -1.0; //if nothing matched
}

stock void LoadDiceProbabilities()
{
	Handle kv = CreateKeyValues("probabilities");
	char fileDir[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, fileDir, sizeof(fileDir), "configs/AC_DiceProbabilities.ini");
	if (FileToKeyValues(kv, fileDir))
	{
		d_pistol = LoadDiceProbabilitie(kv, "d_pistol");
		d_he = LoadDiceProbabilitie(kv, "d_he") + d_pistol;
		d_flash = LoadDiceProbabilitie(kv, "d_flash") + d_he;
		d_smoke = LoadDiceProbabilitie(kv, "d_smoke") + d_flash;
		d_inchp = LoadDiceProbabilitie(kv, "d_inchp") + d_smoke;
		d_dechp = LoadDiceProbabilitie(kv, "d_dechp") + d_inchp;
		d_incspeed = LoadDiceProbabilitie(kv, "d_incspeed") + d_dechp;
		d_decspeed = LoadDiceProbabilitie(kv, "d_decspeed") + d_incspeed;
		d_longjump = LoadDiceProbabilitie(kv, "d_longjump") + d_decspeed;
		d_frogjump = LoadDiceProbabilitie(kv, "d_frogjump") + d_longjump;
		d_doublejump = LoadDiceProbabilitie(kv, "d_doublejump") + d_frogjump;
		d_fire = LoadDiceProbabilitie(kv, "d_fire") + d_doublejump;
		d_noclip = LoadDiceProbabilitie(kv, "d_noclip") + d_fire;
		d_freeze = LoadDiceProbabilitie(kv, "d_freeze") + d_noclip;
		d_lowgrav = LoadDiceProbabilitie(kv, "d_lowgrav") + d_freeze;
		d_highgrav = LoadDiceProbabilitie(kv, "d_highgrav") + d_lowgrav;
		d_godmode = LoadDiceProbabilitie(kv, "d_godmode") + d_highgrav;
		d_nv = LoadDiceProbabilitie(kv, "d_nv") + d_godmode;
		d_invis = LoadDiceProbabilitie(kv, "d_invis") + d_nv;
		d_model = LoadDiceProbabilitie(kv, "d_model") + d_invis;
		d_kevlar = LoadDiceProbabilitie(kv, "d_kevlar") + d_model;
		d_cfour = LoadDiceProbabilitie(kv, "d_cfour") + d_kevlar;
		d_expcfour = LoadDiceProbabilitie(kv, "d_expcfour") + d_cfour;
		d_throwingknife = LoadDiceProbabilitie(kv, "d_throwingknife") + d_expcfour;
		d_incfrags = LoadDiceProbabilitie(kv, "d_incfrags") + d_throwingknife;
		d_decfrags = LoadDiceProbabilitie(kv, "d_decfrags") + d_incfrags;
		d_hologram = LoadDiceProbabilitie(kv, "d_hologram") + d_decfrags;
		d_blind = LoadDiceProbabilitie(kv, "d_blind") + d_hologram;
		d_increfuses = LoadDiceProbabilitie(kv, "d_increfuses") + d_blind;
		d_decrefuses = LoadDiceProbabilitie(kv, "d_decrefuses") + d_increfuses;
		d_ctbullymenu = LoadDiceProbabilitie(kv, "d_ctbullymenu") + d_decrefuses;
		d_goodtmenu = LoadDiceProbabilitie(kv, "d_goodtmenu") + d_ctbullymenu;
		d_badtmenu = LoadDiceProbabilitie(kv, "d_badtmenu") + d_goodtmenu;
		d_mirrordmg = LoadDiceProbabilitie(kv, "d_mirrordmg") + d_badtmenu;
		d_vampiredmg = LoadDiceProbabilitie(kv, "d_vampiredmg") + d_mirrordmg;
		d_incdmg = LoadDiceProbabilitie(kv, "d_incdmg") + d_vampiredmg;
		d_decdmg = LoadDiceProbabilitie(kv, "d_decdmg") + d_incdmg;
		d_drugs = LoadDiceProbabilitie(kv, "d_drugs") + d_decdmg;
		d_randtelechange = LoadDiceProbabilitie(kv, "d_randtelechange") + d_drugs;
		d_entshoot = LoadDiceProbabilitie(kv, "d_entshoot") + d_randtelechange;
		d_regenerate = LoadDiceProbabilitie(kv, "d_regenerate") + d_entshoot;
		d_disease = LoadDiceProbabilitie(kv, "d_disease") + d_regenerate;
		d_teamheal = LoadDiceProbabilitie(kv, "d_teamheal") + d_disease;
		d_death_exp = LoadDiceProbabilitie(kv, "d_death_exp") + d_teamheal;
		d_death_degenerate = LoadDiceProbabilitie(kv, "d_death_degenerate") + d_death_exp;
		d_death_lightning = LoadDiceProbabilitie(kv, "d_death_lightning") + d_death_degenerate;
		d_wpnrestrict = LoadDiceProbabilitie(kv, "d_wpnrestrict") + d_death_lightning;
		d_mine = LoadDiceProbabilitie(kv, "d_mine") + d_wpnrestrict;
		d_inccapitulates = LoadDiceProbabilitie(kv, "d_inccapitulates") + d_mine;
		d_deccapitulates = LoadDiceProbabilitie(kv, "d_deccapitulates") + d_inccapitulates;
	}
	else
	{
		Log(EL_Error, "dicecore", "File '%s' could not be loaded or parsed!", fileDir);
	}
}

stock float LoadDiceProbabilitie(Handle kv, const char[] phrase)
{
	float value = KvGetFloat(kv, phrase, -1.0);
	if (value == -1.0)
	{
		Log(EL_Message, "dicecore", "Diceprobabilitie-Key '%s' not found! Using 0.0%% for it.", phrase);
		return 0.0;
	}
	return value;
}

#define d_StateLevel_Good 0
#define d_StateLevel_Neutral 1
#define d_StateLevel_Bad 2

stock void AC_Dice_PrintToChat(int client, float probability, int stateLevel, const char[] msg, any ...)
{
	char message[255]; //not 192 because we perhaps need more
	char colStateTag[7];
	colStateTag = "FFFF44";
	if (stateLevel == d_StateLevel_Good)
	{ colStateTag = "44FF44"; }
	else if (stateLevel == d_StateLevel_Bad)
	{ colStateTag = "FF4444"; }
	SetGlobalTransTarget(client);
	VFormat(message, sizeof(message), msg, 5);
	if (c_dices[client] > 0)
	{ Format(message, sizeof(message), "\x01%s %s \x07%s[%0.1f %%] \x01- %t", Tag, message, colStateTag, Math_Max(probability, 0.0), "d_left_dices", c_dices[client]); }
	else
	{ Format(message, sizeof(message), "\x01%s %s \x07%s[%0.1f %%]", Tag, message, colStateTag, Math_Max(probability, 0.0)); }
	Handle buffer = StartMessageOne("SayText2", client, USERMSG_RELIABLE | USERMSG_BLOCKHOOKS);
	BfWriteByte(buffer, client);
	BfWriteByte(buffer, true);
	BfWriteString(buffer, message);
	EndMessage();
}

public Action ACmd_ForceDice(int client, int args)
{
	if (!(args == 1 || args == 2))
	{ ReplyToCommand(client, "[AC]: Usage: sm_forcedice <#userid|name> [value|phrase]"); return Plugin_Handled; }
	char target[MAX_NAME_LENGTH];
	char value[32] = "-1.0";
	float val = -1.0;
	GetCmdArg(1, target, sizeof(target));
	if (args == 2)
	{
		GetCmdArg(2, value, sizeof(value));
		val = ResolveDicePhrase(value);
		if (val == -1.0)
		{
			int count = StringToFloatEx(value, val);
			if (count < 1)
			{
				ReplyToCommand(client, "[AC]: [value|phrase]: A percent value (without the %% charachter) or a dicephrase.");
				return Plugin_Handled;
			}
		}
	}
	char buffer[MAX_NAME_LENGTH];
	int targets[MAXPLAYERS+1];
	bool ml = false;
	int count = ProcessTargetString(target, client, targets, sizeof(targets), COMMAND_FILTER_ALIVE | COMMAND_FILTER_CONNECTED, buffer, sizeof(buffer), ml);
	if (count <= 0) { ReplyToTargetError(client, count); return Plugin_Handled; }
	bool isForced = true;
	if (val == -1.0) { isForced = false; }
	for (int i = 0; i < count; i++)
	{
		ThrowDice(targets[i], val, isForced);
		LogAction(client, targets[i], "[AC]: %L forced dice (%s) on %L", client, value, targets[i]);
	}
	/*if (ml)
	{ ShowActivity2(client, "\x04[AC]\x01 ", "%t", "acmd_forcedice_activity", buffer); }
	else
	{ ShowActivity2(client, "\x04[AC]\x01 ", "%t", "acmd_forcedice_activity", "_s", buffer); }*/
	AC_Notify(client, "acmd_forcedice_activity", buffer, ml);
	return Plugin_Handled;
}

/* List of all cases
pistol
he
flash
smoke
inchp
dechp
incspeed
decspeed
longjump
frogjump
doublejump
fire
noclip
freeze
lowgrav
highgrav
godmode
nv
invis
model
kevlar
cfour
expcfour
throwingknife
incfrags
decfrags
hologram
blind
increfuses
decrefuses
ctbullymenu
goodtmenu
badtmenu
mirrordmg
vampiredmg
incdmg
decdmg
drugs
randtelechange
entshoot
regenerate
disease
teamheal
death_exp
death_degenerate
death_lightning
wpnrestrict
mine
inccapitulates
deccapitulates
*/