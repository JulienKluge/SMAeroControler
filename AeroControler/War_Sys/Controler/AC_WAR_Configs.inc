/*
**
** Aero Jail Controler Plugin
** WAR Base Control System - partial configuration file
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

stock void LoadStaticConfig()
{
	char file[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, file, sizeof(file), "configs/AC_WAR_StaticConfig.ini");
	Handle kv = CreateKeyValues("StaticSettings");
	if (FileToKeyValues(kv, file))
	{
		KvGetString(kv, "Cmd_Str_WarVote", Str_WarVoteCmd, sizeof(Str_WarVoteCmd), "sm_war;sm_w");
		KvGetString(kv, "Cmd_PStr_WarVote", PStr_WarVoteCmd, sizeof(PStr_WarVoteCmd), "!war");
		Alw_warVoteCmd = IntToBool(KvGetNum(kv, "Cmd_WarVote", 1));
	}
	CloseHandle(kv);
}

stock void LoadTranslationFiles()
{
	char transFile[PLATFORM_MAX_PATH];
	char transDestFile[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, transFile, sizeof(transFile), "/translations/_aerocontroler_war.phrases.txt");
	BuildPath(Path_SM, transDestFile, sizeof(transDestFile), "/translations/aerocontroler_war.phrases.txt");
	int transResult = ac_parseTransFile(transFile, transDestFile);
	if (transResult)
	{ LoadTranslations("aerocontroler_war.phrases.txt"); }
	else
	{ Log(EL_FatalEx, "WARSys", "The template file %s could not be readed or the destination file %s be written.", transFile, transDestFile); }
}

stock bool IntToBool(int integer)
{
	if (integer == 0) { return false; }
	return true;
}