#pragma semicolon 1

/* γ = dev : α = canditae for control testing : β = proving ground/release candidate : λ = Final stable/RTM */
#define PLUGIN_VERSION "0.00γ"
#define PLUGIN_AUTHOR ""

/* SM INCLUDES */
#include <sourcemod>
#include <sdktools>
/* SM INCLUDES */

/* AERO INCLUDES */
#include "AeroControler\\SharedPluginBase\\AC_ErrorSys.inc"
#include "AeroControler\\SharedPluginBase\\AC_UISys.inc"
#include "AeroControler\\SharedPluginBase\\AC_ClientSys.inc"

#include "AeroControler\\aerocontroler_core_interface.inc" //interface to the core
#include "AeroControler\\aerocontroler_war_interface.inc" //interface to the war-base
/* AERO INCLUDES */

/* CUSTOM INCLUDES */
/* CUSTOM INCLUDES */

new ExtensionEntryIndex = -1; //These variable will hold the menu-extension-entry-index.
new bool:Ready = false; //If this is true, our WAR's are registered.

public Plugin:myinfo = 
{
	name = "WAR Template",
	author = PLUGIN_AUTHOR,
	description = "",
	version = PLUGIN_VERSION,
	url = ""
};

public OnPluginStart()
{
	ac_getCoreComTag(Tag, sizeof(Tag)); //get our chat tag
	ExtensionEntryIndex = ac_registerPluginExtension("WAR Template", PLUGIN_AUTHOR, PLUGIN_VERSION); //register our plugin in the about menu
	RegisterWars();
}

public ac_OnCoreComTagChanged(const String:tag[]) { Format(Tag, sizeof(Tag), "%s", tag); } //when the tag change, we want to have the new one

public OnPluginEnd()
{
	if (LibraryExists("ac_core")) //alibi check
	{ ac_unregisterPluginExtension(ExtensionEntryIndex); } //unregister our plugin from the about menu
}

public OnLibraryAdded(const String:name[])
{
	if (StrEqual(name, "ac_war_sys"))
	{ RegisterWars(); }
}

public OnLibraryRemoved(const String:name[])
{
	if (StrEqual(name, "ac_war_sys"))
	{ Ready = false; }
}

stock RegisterWars()
{
	if (!Ready) //skip when we already registered
	{
		Ready = true;
		ac_war_RegWar(warcallback_War1Start, warcallback_War1End, "Template WAR 1", true);
		ac_war_RegWar(warcallback_War2Start, warcallback_War2End, "Template WAR 2", true);
	}
}

public warcallback_War1Start()
{
	//WAR 1 started
}
public warcallback_War1End()
{
	//WAR 1 ended
}

public warcallback_War2Start()
{
	//WAR 2 started
}
public warcallback_War2End()
{
	//WAR 2 ended
}