/*
**
** Aero Jail Controler Plugin
** Shared Error System
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
#if defined _AC_ERRORSYS_included
	#endinput
#endif
#define _AC_ERRORSYS_included

#define EL_Info 0				//Errorlevel: Informations for the server-admins.
#define EL_Message 1			//Errorlevel: Messages which provides informations about something which should be changed.
#define EL_Error 2			//Errorlevel: An error occured in the plugin which was catched. Don't affect the running plugin.
#define EL_Fatal 3			//Errorlevel: An error occured in the plugin which has fatal consequences. Stops execution of the plugin in the function.
#define EL_FatalEx 4			//Errorlevel: An error occured in the plugin which has fatal consequences. Try still running the plugin.
#define EL_Abort 5			//Errorlevel: An error occured in the plugin which has fatal consequences in a way, that the plugin cannot run anymore.

stock Log(_:EL_Level, const String:pluginName[], const String:message[], any:...)
{
	new eMsgLength = strlen(message) + 255;
	new String:errorMsg[eMsgLength];
	VFormat(errorMsg, eMsgLength, message, 4);
	switch (EL_Level)
	{
		case EL_Info: { LogMessage("[AeroControler (%s) - Info]: %s", pluginName, errorMsg); }
		case EL_Message: { LogMessage("[AeroControler (%s) - Attention]: %s", pluginName, errorMsg); }
		case EL_Error: { LogError("[AeroControler (%s) - ERROR]: %s | continue running plugin...", pluginName, errorMsg); }
		case EL_Fatal: { ThrowError("!!! [AeroControler (%s) - FATAL ERROR]: %s | interrupt running plugin...", pluginName, errorMsg); }
		case EL_FatalEx: { LogError("!!! [AeroControler (%s) - FATAL ERROR]: %s | try continue running plugin...", pluginName, errorMsg); }
		case EL_Abort:
		{
			new endMessageLength = eMsgLength + 255;
			decl String:endMsg[endMessageLength];
			Format(endMsg, endMessageLength, "!!! [AeroControler (%s) - FATAL ERROR]: %s | STOP PLUGIN EXECUTION", pluginName, errorMsg);
			SetFailState(endMsg);
		}
	}
}