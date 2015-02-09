#if defined _AC_WAR_SYS_Included_
	#endinput
#endif
#define _AC_WAR_SYS_Included_

public SharedPlugin:__pl_ac_war_sys =
{
	name = "ac_war_sys",
	file = "AeroControler_WARSytem.smx",
	#if defined REQUIRE_PLUGIN
	required = 1,
	#else
	required = 0,
	#endif
};

public __pl_ac_war_sys_SetNTVOptional()
{
	MarkNativeAsOptional("ac_war_RegWar");
	MarkNativeAsOptional("ac_war_UnRegWar");
	MarkNativeAsOptional("ac_war_EndWar");
}

/*
** Called when a war is started.
** [@param index				Index of the started war.]
** [@param maxrounds			How many rounds are played maximum. (-1 on endless rounds)]
** [@param maxtime			How much is the war allowed to run maximum. (-1 on endless time)]
** @noreturn
*/
funcenum WarStart
{
	public(), //you perhaps dont need any informations. If you declared a single function for every war and you wont worry about your war gets forced to be stopped.
	public(_:index),
	public(_:index, _:maxrounds, _:maxtime)
}

/*
** Called when a war is forced to the end.
** [@param index				Index of the ended war.]
** @noreturn
*/
funcenum WarEnd
{
	public(), //again, if you delcared a single function for every war, this is the end function for each war
	public(_:index)
}

/*
** Called when a war freeze time is started.
** @param time 				Time the freezetime will run.
** @noreturn
**
*/
functag public WarFreezeTimeStart(_:time);

/*
** Called when a war freeze time is ended.
** @noreturn
**
*/
functag public WarFreezeTimeEnd();

/*
** Register a war into the war controler.
** @param startFunction			Function to call when the war should be started
** @param endFunction				Function to call when the war is forced to be over by the controler
** @param name					Name of the war
** @param HasFreezeTime			Should a freeze time be applied on the war.
** @param freezeStartFunction	Function to call when a freezetime starts.
** @param freezeEndFunction		Function to call when a freezetime ends.
** @return							Index of the war (-1 if no free place was available)
*/
native _:ac_war_RegWar(WarStart:startFunction
						, WarEnd:endFunction
						, const String:name[]
						, bool:HasFreezeTime = false
						, WarFreezeTimeStart:freezeStartFunction = WarFreezeTimeStart:0
						, WarFreezeTimeEnd:freezeEndFunction = WarFreezeTimeEnd:0);

/*
** Unregister a war from the controler.
** @param index				Index of the war to unregister
** @noreturn
*/
native ac_war_UnRegWar(_:index);

/*
** Ends a war softly. This should only be called when the war is over before the controler will force it to be over (Example: A gungame war which only needs one round).
** The controler will restart the round!
** @param ForceRoundRestart	Forces the controler to restart the round.
** @noreturn
*/
native ac_war_EndWar(bool:ForceRoundRestart = true);