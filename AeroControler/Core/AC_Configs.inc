/*
**
** Aero Jail Controler Plugin - Configurations partial File
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

stock InitCVarSettings()
{
	new Handle:cvar = INVALID_HANDLE; //subhandle
	cvar = CreateConVar("ac_0_IntroducePlugin", "1.0", "Should the Plugin be introduced 90 seconds after every mapstart? You can set it to 0.0 to disable it, but please remember, I have invested much time and effort into this work...but I wont blame you if you set it to 0!", FCVAR_PLUGIN, true, 0.0, true, 1.0); //Wordwrap ftw
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_IntroducePlugin); }
	cvar = CreateConVar("ac_0_TagColor", "1188FF", "Defines the TagColor used ingame", FCVAR_PLUGIN, false, 0.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_TagColor); }
	cvar = CreateConVar("ac_0_TagString", "[Jail]:", "Defines the Tagstring used ingame", FCVAR_PLUGIN, false, 0.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_TagString); }
	cvar = CreateConVar("ac_1_NoblockSystemState", "2.0", "0 = block | 1 = noblock | 2 = block (noblock commands) | 3 = noblock (block commands)", FCVAR_PLUGIN, true, 0.0, true, 3.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_NoblockState); }
	cvar = CreateConVar("ac_2_CTSpawnWeapons", "weapon_m4a1;weapon_deagle;weapon_knife", "CT spawn weapons delimited with an ;", FCVAR_PLUGIN, false, 0.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_CTSpawnWeapons); }
	cvar = CreateConVar("ac_2_TSpawnWeapons", "weapon_knife", "T spawn weapons delimited with an ;", FCVAR_PLUGIN, false, 0.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_TSpawnWeapons); }
	cvar = CreateConVar("ac_0_GameDescription", "JailBreak CSS", "Leave this blank to disable. A string which should be set as game description.", FCVAR_PLUGIN, false, 0.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_GameDescription); }
	cvar = CreateConVar("ac_2_AnnounceDice", "1.0", "0.0 to diable. Should the dice command be announced on spawn.", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_AnnounceDice); }
	cvar = CreateConVar("ac_2_AnnounceCmds", "1.0", "0.0 to diable. Should the cmds command be announced. (gets merged when ac_2_AnnounceRules is on 1.0)", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_AnnounceCmds); }
	cvar = CreateConVar("ac_2_AnnounceRules", "1.0", "0.0 to diable. Should the rules command be announced. (gets merged when ac_2_AnnounceCmds is on 1.0)", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_AnnounceRules); }
	cvar = CreateConVar("ac_2_AnnounceDelay", "600.0", "How many seconds between 2 announcements.", FCVAR_PLUGIN, true, 1.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_AnnounceDelay); }
	cvar = CreateConVar("ac_FC_CountryFilter", "1.0", "Should the plugin enable the country filter? 1.0 to enable otherwise 0.0", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_fc_CountryFilter); }
	cvar = CreateConVar("ac_FC_FilterCaseSensitive", "0.0", "Filter with case sensitive rules? 1.0 to filter case sensitive otherwise 0.0", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_fc_CaseSensitive); }
	cvar = CreateConVar("ac_FC_KickMessage", "wrong language;falsche Sprache;неправильный язык;idioma incorrecto;mauvaise langue", "Message to display when the client got not trough the country filter.", FCVAR_PLUGIN, false, 0.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_fc_KickMsg); }
	cvar = CreateConVar("ac_FC_Countries", "us;um;ca;au;gb", "All countries allowed on the server. Seperated by a semicolon. Leave it blanc to disable. Keep attention on no whitespaces and when you have turned case sensitive on!", FCVAR_PLUGIN, false, 0.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_fc_Countries); }
	cvar = CreateConVar("ac_FC_Languages", "en;", "All game languages allowed on the server. Seperated by a semicolon. Leave it blanc to disable. Keep attention on no whitespaces and when you have turned case sensitive on!", FCVAR_PLUGIN, false, 0.0, false, 0.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_fc_Languages); }
	cvar = CreateConVar("ac_FC_MatchExact", "0.0", "1.0 to allow acces when both (language & country) are right; 0.0 to allow acces when one of them or both are right", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_fc_ExactMatch); }
	cvar = CreateConVar("ac_FC_AllowWorldIPs", "1.0", "Should players get acces where the plugin cannot determinate their country? (Language is still checked) 1.0 to give them acces otherwise 0.0", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	if (cvar != INVALID_HANDLE) { HookConVarChange(cvar, cvarchanged_fc_AllowWorldIPs); }
}

/* Format/Intendation Change for the CVar_Changed publics to save lines... */
public cvarchanged_IntroducePlugin(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	IntroduceMe = GetConVarBool(cvar);
}
public cvarchanged_TagColor(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	Format(serverChatColor, sizeof(serverChatColor), "%s", newValue);
	UpdateTag();
}
public cvarchanged_TagString(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	Format(serverTag, sizeof(serverTag), "%s", newValue);
	UpdateTag();
}
public cvarchanged_NoblockState(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	NoBlockState = GetConVarInt(cvar);
}
public cvarchanged_CTSpawnWeapons(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	ctSpawnWeaponCount = ExplodeString(newValue, ";", ctSpawnWeapons, 10, 32); //1 line ftw xD
}
public cvarchanged_TSpawnWeapons(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	tSpawnWeaponCount = ExplodeString(newValue, ";", tSpawnWeapons, 10, 32); //whuuuhuuuu xD
}
public cvarchanged_GameDescription(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	//GetConVarString(cvar, gameDescription, sizeof(gameDescription)); Well I don't know which line i should preferre...
	Format(gameDescription, sizeof(gameDescription), "%s", newValue); 
}
public cvarchanged_AnnounceDice(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	announce_dice = GetConVarBool(cvar);
}
public cvarchanged_AnnounceCmds(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	announce_cmds = GetConVarBool(cvar);
}
public cvarchanged_AnnounceRules(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	announce_rules = GetConVarBool(cvar);
}
public cvarchanged_AnnounceDelay(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	announce_delay = GetConVarFloat(cvar);
	if (announce_Timer != INVALID_HANDLE)
	{ CloseHandle(announce_Timer); }
	announce_Timer = CreateTimer(announce_delay, timer_Announce, INVALID_HANDLE, TIMER_REPEAT);
}
public cvarchanged_fc_CountryFilter(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	FC_FilterCountries = GetConVarBool(cvar);
}
public cvarchanged_fc_CaseSensitive(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	FC_FilterCaseSensitive = GetConVarBool(cvar);
}
public cvarchanged_fc_KickMsg(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	Format(FC_KickMessage, sizeof(FC_KickMessage), "%s", newValue);
}
public cvarchanged_fc_Countries(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	if (strlen(newValue) > 1)
	{ FC_GeoIPCodesCount = ExplodeString(newValue, ";", FC_GeoIPCodes, 32, 4); }
	else
	{ FC_GeoIPCodesCount = 0; }
}
public cvarchanged_fc_Languages(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	if (strlen(newValue) > 1)
	{ FC_LanguageInfosCount = ExplodeString(newValue, ";", FC_LanguageInfos, 32, 4); }
	else
	{ FC_LanguageInfosCount = 0; }
}
public cvarchanged_fc_ExactMatch(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	FC_FilterMatchExact = GetConVarBool(cvar);
}
public cvarchanged_fc_AllowWorldIPs(Handle:cvar, const String:oldValue[], const String:newValue[]) {
	FC_AllowWorldIps = GetConVarBool(cvar);
}

stock UpdateTag()
{
	Format(Tag, sizeof(Tag), "\x07%s%s\x01", serverChatColor, serverTag);
	if (tagChangedForward != INVALID_HANDLE)
	{
		Call_StartForward(tagChangedForward);
		Call_PushString(Tag);
		Call_Finish();
	}
}

stock LoadStaticPluginConfig()
{
	decl String:FilePath[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, FilePath, sizeof(FilePath), "/configs/AC_StaticConfig.ini");
	if (FileExists(FilePath))
	{
		new Handle:kv = CreateKeyValues("StaticSettings");
		if (FileToKeyValues(kv, FilePath))
		{
			KvGetString(kv, "Server_Phrase_Color", serverChatColor, sizeof(serverChatColor), "1188FF");
			Format(serverStaticChatColor, sizeof(serverStaticChatColor), "%s", serverChatColor);
			Cmd_Alw_Refuse = IntToBool(KvGetNum(kv, "Cmd_Refuse", 1));
			KvGetString(kv, "Cmd_Str_Refuse", Cmd_Str_Refuse, sizeof(Cmd_Str_Refuse), "sm_r;sm_refuse");
			KvGetString(kv, "Cmd_PStr_Refuse", Cmd_PStr_Refuse, sizeof(Cmd_PStr_Refuse), "!r");
			Cmd_Alw_Capitulate = IntToBool(KvGetNum(kv, "Cmd_Capitulate", 1));
			KvGetString(kv, "Cmd_Str_Capitulate", Cmd_Str_Capitulate, sizeof(Cmd_Str_Capitulate), "sm_c;sm_capitulate");
			KvGetString(kv, "Cmd_PStr_Capitulate", Cmd_PStr_Capitulate, sizeof(Cmd_PStr_Capitulate), "!c");
			Cmd_Alw_Dice = IntToBool(KvGetNum(kv, "Cmd_Dice", 1));
			KvGetString(kv, "Cmd_Str_Dice", Cmd_Str_Dice, sizeof(Cmd_Str_Dice), "sm_d;sm_dice;sm_rtd");
			KvGetString(kv, "Cmd_PStr_Dice", Cmd_PStr_Dice, sizeof(Cmd_PStr_Dice), "!d");
			Cmd_Alw_DiceMenu = IntToBool(KvGetNum(kv, "Cmd_DiceMenu", 1));
			KvGetString(kv, "Cmd_Str_DiceMenu", Cmd_Str_DiceMenu, sizeof(Cmd_Str_DiceMenu), "sm_dicemenu");
			KvGetString(kv, "Cmd_PStr_DiceMenu", Cmd_PStr_DiceMenu, sizeof(Cmd_PStr_DiceMenu), "!dicemenu");
			Cmd_Alw_Kill = IntToBool(KvGetNum(kv, "Cmd_Kill", 1));
			KvGetString(kv, "Cmd_Str_Kill", Cmd_Str_Kill, sizeof(Cmd_Str_Kill), "sm_kill;sm_k");
			KvGetString(kv, "Cmd_PStr_Kill", Cmd_PStr_Kill, sizeof(Cmd_PStr_Kill), "!kill");
			Cmd_Alw_ColCmds = IntToBool(KvGetNum(kv, "Cmds_ColorCmds", 1));
			KvGetString(kv, "Cmd_Str_SetColors", Cmd_Str_SetCol, sizeof(Cmd_Str_SetCol), "sm_sc;sm_setcolors");
			KvGetString(kv, "Cmd_PStr_SetColors", Cmd_PStr_SetCol, sizeof(Cmd_PStr_SetCol), "!sc");
			KvGetString(kv, "Cmd_Str_DeleteColors", Cmd_Str_DelCol, sizeof(Cmd_Str_DelCol), "sm_dc;sm_deletecolors");
			KvGetString(kv, "Cmd_PStr_DeleteColors", Cmd_PStr_DelCol, sizeof(Cmd_PStr_DelCol), "!dc");
			KvGetString(kv, "Cmd_Str_Noblock", Cmd_Str_Noblock, sizeof(Cmd_Str_Noblock), "sm_noblock;sm_nb");
			KvGetString(kv, "Cmd_PStr_Noblock", Cmd_PStr_Noblock, sizeof(Cmd_PStr_Noblock), "!noblock");
			KvGetString(kv, "Cmd_Str_Block", Cmd_Str_Block, sizeof(Cmd_Str_Block), "sm_block;sm_b");
			KvGetString(kv, "Cmd_PStr_Block", Cmd_PStr_Block, sizeof(Cmd_PStr_Block), "!block");
			Cmd_Alw_Commands = IntToBool(KvGetNum(kv, "Cmd_Commands", 1));
			KvGetString(kv, "Cmd_Str_Commands", Cmd_Str_Commands, sizeof(Cmd_Str_Commands), "sm_cmds;sm_commands");
			KvGetString(kv, "Cmd_PStr_Commands", Cmd_PStr_Commands, sizeof(Cmd_PStr_Commands), "!cmds");
			Cmd_Alw_About = IntToBool(KvGetNum(kv, "Cmd_About", 1));
			KvGetString(kv, "Cmd_Str_About", Cmd_Str_About, sizeof(Cmd_Str_About), "sm_ac;sm_aero;sm_aerocontroler");
			KvGetString(kv, "Cmd_PStr_About", Cmd_PStr_About, sizeof(Cmd_PStr_About), "!ac");
			KvGetString(kv, "Cmd_Str_Rules", Cmd_Str_Rules, sizeof(Cmd_Str_Rules), "sm_rules");
			KvGetString(kv, "Cmd_PStr_Rules", Cmd_PStr_Rules, sizeof(Cmd_PStr_Rules), "!rules");
			if (KvGetNum(kv, "Enable_WeaponSafety", 1) == 0) { GunSafetyState = -1; }
		}
		else
		{
			Log(EL_Error, "core", "The static configuration file '%s' has an invalid syntax.", FilePath);
		}
		CloseHandle(kv);
	}
	else
	{
		Log(EL_Message, "core", "The static configuration file '%s' was not found! Using standart settings.", FilePath);
	}
}

/*
** 
** Okay, this function, is a little bit more complicated.
** The AC provides the possibility to add a color to the
** standart phrases. So before we load the phrases, we'll
** parse them and add the color codes and safes the result
** in an extra file. These extra file will be loaded while
** the template file keeps untouched.
** The user also have the possibility to decide, if his file
** should be loaded. that is helpfull if he splits a file
** into his languages. In this case, only the english-main-
** file has to be loaded. The others not!
** The config File is located in Path_SM//configs/AC_TranslationFiles.ini
** 
*/
stock LoadTranslationsFiles()
{
	LoadTranslations("common.phrases.txt"); //necessary
	decl String:FilePath[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, FilePath, sizeof(FilePath), "/configs/AC_TranslationFiles.ini");
	if (FileExists(FilePath))
	{
		new Handle:kv = CreateKeyValues("translationfiles");
		if (FileToKeyValues(kv, FilePath))
		{
			KvGotoFirstSubKey(kv);
			new bool:FileIsImportant = false;
			new bool:FileIsLoadIntended = false;
			new String:targetString[PLATFORM_MAX_PATH];
			new String:templateString[PLATFORM_MAX_PATH];
			new Handle:loadArray = CreateArray(PLATFORM_MAX_PATH);
			do
			{
				targetString = ""; templateString = ""; FileIsImportant = false; FileIsLoadIntended = false; //Prevents eventually bugs. Safety first.
				KvGetSectionName(kv, templateString, sizeof(templateString));
				FileIsImportant = IntToBool(KvGetNum(kv, "Important", 0));
				FileIsLoadIntended = IntToBool(KvGetNum(kv, "Loading", 0));
				KvGetString(kv, "TranslatedFile", targetString, sizeof(targetString), "");
				new String:targetFile[PLATFORM_MAX_PATH];
				BuildPath(Path_SM, targetFile, sizeof(targetFile), "translations/%s", targetString);
				new String:templateFile[PLATFORM_MAX_PATH];
				BuildPath(Path_SM, templateFile, sizeof(templateFile), "translations/%s", templateString);
				if (FileExists(targetFile))
				{
					DeleteFile(targetFile);
				}
				if (FileExists(templateFile))
				{
					if (!ParseTransFile(templateFile, targetFile))
					{
						Log(EL_FatalEx, "core", "Error in parsing %s or writing %s!", templateFile, targetFile);
					}
					else if (FileIsLoadIntended)
					{
						PushArrayString(loadArray, targetString);
					}
				}
				else if (FileIsImportant)
				{
					Log(EL_FatalEx, "core", "Language file '%s' not found!", templateFile);
				}
				else
				{
					Log(EL_Message, "core", "Unimportant language file '%s' not found!", templateFile);
				}
			}
			while (KvGotoNextKey(kv)); //the first time i think, i have an usability of do-while :D
			new arrayCount = GetArraySize(loadArray);
			if (arrayCount > 0)
			{
				for (new i = 0; i < arrayCount; i++)
				{
					decl String:loadFile[PLATFORM_MAX_PATH];
					GetArrayString(loadArray, i, loadFile, sizeof(loadFile));
					LoadTranslations(loadFile); //That is the function, why we are here :D
				}
			}
			CloseHandle(loadArray);
		}
		else
		{
			Log(EL_Fatal, "core", "The configuration file '%s' was not parseable. No translations loaded!", FilePath);
		}
	}
	else
	{
		Log(EL_Fatal, "core", "The configuration file '%s' was not found. No translations loaded!", FilePath);
	}
}

stock bool:ParseTransFile(const String:source[], const String:dest[])
{
	new bufferSize = FileSize(source) + 8;
	new String:buff[bufferSize]; //use 'new' because when Readfile will fail, we want to check that and not initialized data can lead to a false negative
	new Handle:file = OpenFile(source, "r");
	if (file == INVALID_HANDLE) { return false; }
	ReadFileString(file, buff, bufferSize);
	CloseHandle(file);
	if (strlen(buff) < 2) { return false; } //that's what i ment
	ReplaceString(buff, bufferSize, "{CLID}", serverStaticChatColor, false);
	file = OpenFile(dest, "w+");
	if (file == INVALID_HANDLE) { return false; }
	new bool:retResult = WriteFileString(file, buff, false);
	CloseHandle(file);
	return retResult;
}

stock LoadRuleMenu()
{
	if (ruleMenu != INVALID_HANDLE) { CloseHandle(ruleMenu); }
	if (ruleSubMenus != INVALID_HANDLE)
	{
		new count = GetArraySize(ruleSubMenus);
		for (new i = 0; i < count; i++)
		{
			CloseHandle(Handle:GetArrayCell(ruleSubMenus, i));
		}
		ClearArray(ruleSubMenus);
		CloseHandle(ruleSubMenus);
		ruleSubMenus = CreateArray(32);
	}
	else
	{
		ruleSubMenus = CreateArray(32);
	}
	ruleMenu = CreateMenu(menuHandler_RuleMenu, MenuAction_Select);
	SetMenuExitButton(ruleMenu, true);
	decl String:file[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, file, sizeof(file), "configs/AC_RuleMenu.ini");
	new Handle:kv = CreateKeyValues("RuleMenu");
	if (FileToKeyValues(kv, file))
	{
		new arrayIndex = -1;
		decl String:str[64];
		decl String:sub_str[64];
		KvGetString(kv, "title", str, sizeof(str), "");
		SetMenuTitle(ruleMenu, str);
		if (KvGotoFirstSubKey(kv))
		{
			do
			{
				KvGetSectionName(kv, str, sizeof(str));
				KvGetString(kv, "type", sub_str, sizeof(sub_str), "5");
				if (StrEqual(sub_str, "3"))
				{
					KvGetString(kv, "address", sub_str, sizeof(sub_str), "_");
					decl String:newInfo[64];
					Format(newInfo, sizeof(newInfo), "3\x01%s", sub_str);
					AddMenuItem(ruleMenu, newInfo, str);
				}
				else if (StrEqual(sub_str, "4"))
				{
					arrayIndex++;
					new Handle:subMenu = CreateMenu(menuHandler_SubRuleMenus, MenuAction_Cancel);
					SetMenuExitButton(subMenu, true); SetMenuExitBackButton(subMenu, true);
					SetMenuTitle(subMenu, str);
					for (new i = 0; i < 32; i++)
					{
						decl String:keyName[11];
						Format(keyName, sizeof(keyName), "rule%i", (i + 1));
						KvGetString(kv, keyName, sub_str, sizeof(sub_str), "\x01");
						if (StrEqual(sub_str, "\x01"))
						{ break; }
						else
						{ AddMenuItem(subMenu, "", sub_str); }
					}
					PushArrayCell(ruleSubMenus, _:subMenu);
					Format(sub_str, sizeof(sub_str), "4\x01%i", arrayIndex);
					AddMenuItem(ruleMenu, sub_str, str);
				}
				else if (StrEqual(sub_str, "5"))
				{
					AddMenuItem(ruleMenu, sub_str, str, ITEMDRAW_DISABLED);
				}
				else
				{
					AddMenuItem(ruleMenu, sub_str, str);
				}
			}
			while (KvGotoNextKey(kv));
		}
		else
		{
			AddMenuItem(ruleMenu, "null", "-");
		}
	}
	else
	{
		AddMenuItem(ruleMenu, "null", "-");
	}
	CloseHandle(kv);
	
}

stock bool:IntToBool(_:integer)
{
	if (integer == 0)
	{
		return false;
	}
	return true;
}

stock LoadVersionNumber()
{
	#if defined DEBUG
	/*
	** I know, the snapshot gots increased when the script is started.
	** In first sight that doesn't seems very legit but it is!
	** I never test the same script more than one time :D 
	** If you are a scripter, and you want to change this script,
	** you still should disable this. It's only usefull for me to make
	** a valid version management.
	*/
	new snapshot = 0;
	decl String:file[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, file, sizeof(file), "configs/AC_Version.dat");
	if (FileExists(file))
	{
		decl String:v_parts[2][32];
		if (ExplodeString(PLUGIN_VERSION_DEF, ".", v_parts, 2, 32) == 2)
		{
			decl String:versionMajor[32] = "0";
			decl String:versionMinor[32] = "00";
			decl String:versionSnapshot[32] = "000000";
			new Handle:f = OpenFile(file, "r");
			ReadFileLine(f, versionMajor, sizeof(versionMajor)); TrimString(versionMajor);
			ReadFileLine(f, versionMinor, sizeof(versionMinor)); TrimString(versionMinor);
			ReadFileLine(f, versionSnapshot, sizeof(versionSnapshot)); TrimString(versionSnapshot);
			CloseHandle(f);
			if (StrEqual(versionMajor, v_parts[0]) && StrEqual(versionMinor, v_parts[1]))
			{
				snapshot = StringToInt(versionSnapshot);
				snapshot++;
				Format(versionSnapshot, sizeof(versionSnapshot), "%06i", snapshot);
				Log(EL_Info, "core", "Snapshot updated to: %s (%i)", versionSnapshot, snapshot);
			}
			else
			{
				versionMajor = v_parts[0];
				versionMinor = v_parts[1];
				Log(EL_Info, "core", "Created new Snapshot", versionSnapshot, snapshot);
			}
			DeleteFile(file);
			f = OpenFile(file, "w");
			WriteFileLine(f, versionMajor);
			WriteFileLine(f, versionMinor);
			WriteFileLine(f, versionSnapshot);
			CloseHandle(f);
		}
	}
	Format(PLUGIN_VERSION, sizeof(PLUGIN_VERSION), "%s.%06i[DEBUG]", PLUGIN_VERSION_DEF, snapshot);
	#else
	PLUGIN_VERSION = PLUGIN_VERSION_DEF;
	#endif
}