/*
**
** Aero Jail Controler Plugin
** Standart WARs - Partial configuration file
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

stock LoadTranslationFiles()
{
	decl String:transFile[PLATFORM_MAX_PATH];
	decl String:transDestFile[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, transFile, sizeof(transFile), "/translations/_aerocontroler_stdwars.phrases.txt");
	BuildPath(Path_SM, transDestFile, sizeof(transDestFile), "/translations/aerocontroler_stdwars.phrases.txt");
	new transResult = ac_parseTransFile(transFile, transDestFile);
	if (transResult)
	{ LoadTranslations("aerocontroler_stdwars.phrases.txt"); }
	else
	{ Log(EL_FatalEx, "Standart WARs", "The template file %s could not be readed or the destination file %s be written.", transFile, transDestFile); }
}