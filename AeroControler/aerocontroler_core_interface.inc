#if defined _AC_CORE_Included_
	#endinput
#endif
#define _AC_CORE_Included_

public SharedPlugin:__pl_ac_core =
{
	name = "ac_core",
	file = "AeroControler.smx",
	#if defined REQUIRE_PLUGIN
	required = 1,
	#else
	required = 0,
	#endif
};

public __pl_ac_core_SetNTVOptional()
{
	MarkNativeAsOptional("ac_setCoreOffline");
	MarkNativeAsOptional("ac_setCoreOnline");
	MarkNativeAsOptional("ac_getCoreComTag");
	MarkNativeAsOptional("ac_parseTransFile");
	MarkNativeAsOptional("ac_setClientRefuseVars");
	MarkNativeAsOptional("ac_getClientRefuseVars");
	MarkNativeAsOptional("ac_setClientCapitulateVars");
	MarkNativeAsOptional("ac_getClientCapitulateVars");
	MarkNativeAsOptional("ac_setClientDiceVars");
	MarkNativeAsOptional("ac_getClientDiceVars");
	MarkNativeAsOptional("ac_isClientRebell");
	MarkNativeAsOptional("ac_registerPluginExtension");
	MarkNativeAsOptional("ac_unregisterPluginExtension");
	MarkNativeAsOptional("ac_performDiceOnClient");
	MarkNativeAsOptional("ac_registerCMDMenuBuildForward");
}

/*
Prototype for a function, called when the cmd menu is build for a client
@param client		Client to show the menu to
@param menu		The menu handle (if you add a item, then make the info string empty or pass a commandstring to execute on click)
@noreturn
*/
functag public buildCMDMenuForward(client, Handle:menu);

/*
Set the core functions offline so that the core doesn't control the game-flow.
Including dicing, rebell, refuse, capitulate, gunsafety, mute control etc.
@noreturn
*/
native ac_setCoreOffline();

/*
Set the core functions online back so that the game-flow is controled by the core.
@noreturn
*/
native ac_setCoreOnline();

/*
Writes the actual tag in the given string.
@param tag				The string where the tag should written to.
@param length			Length of the string. 32 is the max possible length.
@noreturn
*/
native ac_getCoreComTag(const String:tag[], length);

/*
Parses a translation file, adds the color tags and saves it.
@param sourceFile			The template file to read from.
@param destinationFile	The file to save the result in. (If still existing it will be deleted)
@return 					true on succes otherwise false
*/
native bool:ac_parseTransFile(const String:sourceFile[], const String:destinationFile[]);

/*
Set the refuse variables for a client.
@param client				the client to set the variables to
@param refuses			refuse count for a client for the current round (-1 for no change)
@param maxRefuses			maximum refuse count for the current round (-1 for no change)
@param baseRefuses		maximum refuses every round (takes effect next round) (-1 for no change)
@param oneBaseRefuses		temporary maximum refuses for the next round - reset when the next round is over (-1 for no change)
@noreturn
*/
native ac_setClientRefuseVars(_:client, _:refuses = -1, _:maxRefuses = -1, _:baseRefuses = -1, _:oneBaseRefuses = -1);

/*
Get the refuse variables for a client
@param		client				the client to get the variables from
@outparam	refuses			refuse count for the current round
@outparam	maxRefuses			maximum refusing count for the current round
@outparam	baseRefuses		maximum refuses every round
@outparam	oneBaseRefuses	temporary maximum refuses for the next round
@noreturn
*/
native ac_getClientRefuseVars(_:client, &_:refuses = 0, &_:maxRefuses = 0, &_:baseRefuses = 0, &_:oneBaseRefuses = 0);

/*
Set the capitulate variables for a client
@param client					the client to set the variables to
@param capitulates			capitulate count for the current round (-1 for no change)
@param maxCapitulates			maximum capitulating count for the current round (-1 for no change)
@param baseCapitulates		maximum capitulating every round (-1 for no change)
@param oneBaseCapitulates	temporary maximum capitulates for the next round (-1 for no change)
@noreturn
*/
native ac_setClientCapitulateVars(_:client, _:capitulates = -1, _:maxCapitulates = -1, _:baseCapitulates = -1, _:oneBaseCapitulates = -1);

/*
Get the capitulate variables for a client
@param client						the client to get the variables from
@outparam capitulates				capitulate count for the current round
@outparam maxCapitulates			maximum capitulating count for the current round
@outparam baseCapitulates		maximum capitulating every round
@outparam oneBaseCapitulates		temporary maximum capitulates for the next round
@noreturn
*/
native ac_getClientCapitulateVars(_:client, &_:capitulates = 0, &_:maxCapitulates = 0, &_:baseCapitulates = 0, &_:oneBaseCapitulates = 0);

/*
Set the dice variables for a client
@param client			the client to set the variables to
@param dices			dice count for the current round (-1 for no change)
@param baseDices		maximum dicing count for every round (-1 for no change)
@param oneBaseDices	temporary maximum dicing for the next round (-1 for no change)
@noreturn
*/
native ac_setClientDiceVars(_:client, _:dices = -1, _:baseDices = -1, _:oneBaseDices = -1);

/*
Get the dice variables for a client
@param		client			the client to get the variables from
@outparam	dices			dice count for the current round (-1 for no change)
@outparam	baseDices		maximum dicing count for the every round (-1 for no change)
@outparam	oneBaseDices	temporary maximum dicing for the next round (-1 for no change)
@noreturn
*/
native ac_getClientDiceVars(_:client, &_:dices = 0, &_:baseDices = 0, &_:oneBaseDices = 0);

/*
Check if a player is rebell
@param client		the client to get the state from
@return true if rebell, otherwise false
*/
native bool:ac_isClientRebell(_:client);

/*
Register the name of your Aero-extension here to display it in the extension-menu
@param pluginExtensionName		The name to show in the menu (max. 64 characters)
@param pluginExtensionAuthor		The author to show in the menu (max. 64 characters)
@param pluginExtensionVersion	The version to show in the menu (max. 64 characters)
@return The index of the first line
*/
native _:ac_registerPluginExtension(const String:pluginExtensionName[], const String:pluginExtensionAuthor[], const String:pluginExtensionVersion[]);

/*
Unregister the name of your Aero-Extension
@param index		The index of your first entry
@noreturn
*/
native ac_unregisterPluginExtension(index);

/*
Performes a dice on a client.
@param client				Client to perform on
@param forcedValue		A value which identifies the specific dice action. Use -1.0 to have a random value.
@param lowerDiceCount		Should the diceable count be descreased? (Attention, negative values can happen and are NOT checked!)
@noreturn
*/
native ac_performDiceOnClient(client, Float:forcedValue = -1.0, bool:lowerDiceCount = false);

/*
Adds a function to the private build-cmd-menu-forward
@param function			The function which should be called when the forward raises
@noparam
*/
native ac_registerCMDMenuBuildForward(buildCMDMenuForward:function);

/*
Got called when the tag gets changed.
@param tag				The changed Tag.
@noreturn
*/
forward ac_OnCoreComTagChanged(const String:tag[]);

/*
Got called, when a client is spawned and all core settings are applied to him.
@param client			The client id spawned.
@noreturn
*/
forward ac_OnClientIsReadyForRound(client);

/*
Got called, when the corestate is changed.
@param indicator		True for core online, false for core offline.
@noreturn
*/
forward ac_OnCoreStateChanged(bool:indicator);
