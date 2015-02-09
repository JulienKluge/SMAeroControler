/*
**
** Aero Jail Controler Plugin - Interface Control partial File
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

functag public buildCMDMenuForward(client, Handle:menu);

stock AC_InterfaceControlRegister()
{
	tagChangedForward = CreateGlobalForward("ac_OnCoreComTagChanged", ET_Ignore, Param_String);
	clientRoundReadyForward = CreateGlobalForward("ac_OnClientIsReadyForRound", ET_Ignore, Param_Cell);
	coreStateForward = CreateGlobalForward("ac_OnCoreStateChanged", ET_Ignore, Param_Cell);
	cmdMenuBuildForward = CreateForward(ET_Hook, Param_Cell, Param_Cell);
	
	CreateNative("ac_setCoreOffline", NativeCall:AC_Native_SetCoreOffline);
	CreateNative("ac_setCoreOnline", NativeCall:AC_Native_SetCoreOnline);
	CreateNative("ac_getCoreComTag", NativeCall:AC_Native_GetCoreComTag);
	CreateNative("ac_parseTransFile", NativeCall:AC_Native_ParseTransFile);
	CreateNative("ac_setClientRefuseVars", NativeCall:AC_Native_SetClientRefuseVars);
	CreateNative("ac_getClientRefuseVars", NativeCall:AC_Native_GetClientRefuseVars);
	CreateNative("ac_setClientCapitulateVars", NativeCall:AC_Native_SetClientCapitulateVars);
	CreateNative("ac_getClientCapitulateVars", NativeCall:AC_Native_GetClientCapitulateVars);
	CreateNative("ac_setClientDiceVars", NativeCall:AC_Native_SetClientDiceVars);
	CreateNative("ac_getClientDiceVars", NativeCall:AC_Native_GetClientDiceVars);
	CreateNative("ac_isClientRebell", NativeCall:AC_Native_IsClientRebell);
	CreateNative("ac_registerPluginExtension", NativeCall:AC_Native_RegisterPluginExtension);
	CreateNative("ac_unregisterPluginExtension", NativeCall:AC_Native_UnRegisterPluginExtension);
	CreateNative("ac_performDiceOnClient", NativeCall:AC_Native_PerformDiceOnClient);
	CreateNative("ac_registerCMDMenuBuildForward", NativeCall:AC_Native_RegisterCMDMenuBuildForward);
	
	RegPluginLibrary("ac_core");
}

public AC_Native_SetCoreOffline(Handle:plugin, numParams)
{
	OverrideWorkToFalse = true; SetCoreState(false); return 0;
}

public AC_Native_SetCoreOnline(Handle:plugin, numParams)
{
	OverrideWorkToFalse = false; SetCoreState(true); return 0;
}

public AC_Native_GetCoreComTag(Handle:plugin, numParams)
{
	new length = GetNativeCell(2);
	SetNativeString(1, Tag, length);
	return 0;
}

public bool:AC_Native_ParseTransFile(Handle:plugin, numParams)
{
	decl String:file_Dest[PLATFORM_MAX_PATH];
	decl String:file_Srce[PLATFORM_MAX_PATH];
	GetNativeString(1, file_Srce, sizeof(file_Srce));
	GetNativeString(2, file_Dest, sizeof(file_Dest));
	if (FileExists(file_Srce))
	{
		if (FileExists(file_Dest)) { DeleteFile(file_Dest); }
		return ParseTransFile(file_Srce, file_Dest);
	}
	return false;
}

public AC_Native_SetClientRefuseVars(Handle:plugin, numParams)
{
	new client = GetNativeCell(1);
	new refusings = GetNativeCell(2);
	new maxRefusings = GetNativeCell(3);
	new baseRefusings = GetNativeCell(4);
	new oneBaseRefusings = GetNativeCell(5);
	if (IsClientValid(client))
	{
		if (refusings != -1)
		{ c_r[client] = refusings; }
		if (maxRefusings != -1)
		{ c_rBaseMax[client] = maxRefusings; }
		if (baseRefusings != -1)
		{ c_rBase[client] = baseRefusings; }
		if (oneBaseRefusings != -1)
		{ c_rSubBase[client] = oneBaseRefusings; }
	}
	return 0;
}

public AC_Native_GetClientRefuseVars(Handle:plugin, numParams)
{
	new client = GetNativeCell(1);
	if (IsClientValid(client))
	{
		SetNativeCellRef(2, c_r[client]);
		SetNativeCellRef(3, c_rBaseMax[client]);
		SetNativeCellRef(4, c_rBase[client]);
		SetNativeCellRef(5, c_rSubBase[client]);
	}
	else
	{
		SetNativeCellRef(2, -1);
		SetNativeCellRef(3, -1);
		SetNativeCellRef(4, -1);
		SetNativeCellRef(5, -1);
	}
	return 0;
}

public AC_Native_SetClientCapitulateVars(Handle:plugin, numParams)
{
	new client = GetNativeCell(1);
	new capitulatings = GetNativeCell(2);
	new maxCapitulatings = GetNativeCell(3);
	new baseCapitulatings = GetNativeCell(4);
	new oneBaseCapitulatings = GetNativeCell(5);
	if (IsClientValid(client))
	{
		if (capitulatings != -1)
		{ c_c[client] = capitulatings; }
		if (maxCapitulatings != -1)
		{ c_cBaseMax[client] = maxCapitulatings; }
		if (baseCapitulatings != -1)
		{ c_cBase[client] = baseCapitulatings; }
		if (oneBaseCapitulatings != -1)
		{ c_cSubBase[client] = oneBaseCapitulatings; }
	}
	return 0;
}

public AC_Native_GetClientCapitulateVars(Handle:plugin, numParams)
{
	new client = GetNativeCell(1);
	if (IsClientValid(client))
	{
		SetNativeCellRef(2, c_c[client]);
		SetNativeCellRef(3, c_cBaseMax[client]);
		SetNativeCellRef(4, c_cBase[client]);
		SetNativeCellRef(5, c_cSubBase[client]);
	}
	else
	{
		SetNativeCellRef(2, -1);
		SetNativeCellRef(3, -1);
		SetNativeCellRef(4, -1);
		SetNativeCellRef(5, -1);
	}
	return 0;
}

public AC_Native_SetClientDiceVars(Handle:plugin, numParams)
{
	new client = GetNativeCell(1);
	new dicings = GetNativeCell(2);
	new baseDicings = GetNativeCell(3);
	new oneBaseDicings = GetNativeCell(4);
	if (IsClientValid(client))
	{
		if (dicings != -1)
		{ c_dices[client] = dicings; }
		if (baseDicings != -1)
		{ c_dicesBase[client] = baseDicings; }
		if (oneBaseDicings != -1)
		{ c_dicesSubBase[client] = oneBaseDicings; }
	}
	return 0;
}

public AC_Native_GetClientDiceVars(Handle:plugin, numParams)
{
	new client = GetNativeCell(1);
	if (IsClientValid(client))
	{
		SetNativeCellRef(2, c_dices[client]);
		SetNativeCellRef(3, c_dicesBase[client]);
		SetNativeCellRef(4, c_dicesSubBase[client]);
	}
	else
	{
		SetNativeCellRef(2, -1);
		SetNativeCellRef(3, -1);
		SetNativeCellRef(4, -1);
	}
	return 0;
}

public bool:AC_Native_IsClientRebell(Handle:plugin, numParams)
{
	new client = GetNativeCell(1);
	if (IsClientValid(client))
	{
		return c_rebell[client];
	}
	return false;
}

public _:AC_Native_RegisterPluginExtension(Handle:plugin, numParams)
{
	decl String:subString[64];
	GetNativeString(1, subString, sizeof(subString));
	new index = PushArrayString(extensionPlugins, subString);
	GetNativeString(2, subString, sizeof(subString));
	PushArrayString(extensionPlugins, subString);
	GetNativeString(3, subString, sizeof(subString));
	PushArrayString(extensionPlugins, subString);
	PushArrayString(extensionPlugins, "- - - -");
	return index;
}

public AC_Native_UnRegisterPluginExtension(Handle:plugin, numParams)
{
	new index = GetNativeCell(1);
	for (new i = 0; i < 4; i++)
	{ SetArrayString(extensionPlugins, index + i, ""); } //not the yellow from the egg ... i know...
	return 0;
}

public AC_Native_PerformDiceOnClient(Handle:plugin, numParams)
{
	new client = GetNativeCell(1);
	new Float:value = Float:GetNativeCell(2);
	new bool:lowerDiceCount = bool:GetNativeCell(3);
	if (lowerDiceCount)
	{ c_dices[client]--; }
	ThrowDice(client, value, false);
	return 0;
}

public AC_Native_RegisterCMDMenuBuildForward(Handle:plugin, numParams)
{
	new Function:func = GetNativeCell(1);
	AddToForward(cmdMenuBuildForward, plugin, func);
	return 0;
}