/*
**
** Aero Jail Controler Plugin
** Store System - partial configuration file
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
	BuildPath(Path_SM, file, sizeof(file), "configs/AC_STORE_StaticConfig.ini");
	Handle kv = CreateKeyValues("StaticSettings");
	if (FileToKeyValues(kv, file))
	{
		KvGetString(kv, "Store_CurrentName", currentName, sizeof(currentName), "Points");
		Cmd_Store = IntToBool(KvGetNum(kv, "Cmd_Store", 1));
		KvGetString(kv, "Cmd_Str_Store", Cmd_Str_Store, sizeof(Cmd_Str_Store), "sm_store;sm_shop");
		KvGetString(kv, "Cmd_PStr_Store", Cmd_PStr_Store, sizeof(Cmd_PStr_Store), "!store");
		Cmd_StoreTop = IntToBool(KvGetNum(kv, "Cmd_StoreTop", 1));
		KvGetString(kv, "Cmd_Str_StoreTop", Cmd_Str_StoreTop, sizeof(Cmd_Str_StoreTop), "sm_storetop;sm_st");
		KvGetString(kv, "Cmd_PStr_StoreTop", Cmd_PStr_StoreTop, sizeof(Cmd_PStr_StoreTop), "!storetop");
		Cmd_StoreRank = IntToBool(KvGetNum(kv, "Cmd_StoreRank", 1));
		KvGetString(kv, "Cmd_Str_StoreRank", Cmd_Str_StoreRank, sizeof(Cmd_Str_StoreRank), "sm_storerank;sm_sr");
		KvGetString(kv, "Cmd_PStr_StoreRank", Cmd_PStr_StoreRank, sizeof(Cmd_PStr_StoreRank), "!storerank");
		Cmd_SF = IntToBool(KvGetNum(kv, "Cmd_SkillForce", 1));
		KvGetString(kv, "Cmd_Str_SkillForce", Cmd_Str_SF, sizeof(Cmd_Str_SF), "sm_sf;sm_skillforce");
		KvGetString(kv, "Cmd_PStr_SkillForce", Cmd_PStr_SF, sizeof(Cmd_PStr_SF), "!sf");
	}
	CloseHandle(kv);
}

stock void LoadTranslationFiles()
{
	LoadTranslations("common.phrases.txt");
	char transFile[PLATFORM_MAX_PATH];
	char transDestFile[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, transFile, sizeof(transFile), "/translations/_aerocontroler_store.phrases.txt");
	BuildPath(Path_SM, transDestFile, sizeof(transDestFile), "/translations/aerocontroler_store.phrases.txt");
	int transResult = ac_parseTransFile(transFile, transDestFile);
	if (transResult)
	{ LoadTranslations("aerocontroler_store.phrases.txt"); }
	else
	{ Log(EL_FatalEx, "STORESys", "The template file %s could not be readed or the destination file %s be written.", transFile, transDestFile); }
}

stock void LoadDatabaseSettings()
{
	char file[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, file, sizeof(file), "configs/AC_STORE_DatabaseConfig.ini");
	Handle kv = CreateKeyValues("Store_DBConfig");
	if (FileToKeyValues(kv, file))
	{
		KvGetString(kv, "StoreTableName", tableName, sizeof(tableName), "ac_store");
		KvGetString(kv, "StoreSkillForceTableName", sfTableName, sizeof(sfTableName), "ac_store_skillforces");
		db_useDecimals = IntToBool(KvGetNum(kv, "UseDecimalType", 1));
		db_decimalAbsolutPrecision = KvGetNum(kv, "Decimal_AbsolutePrecision", 17);
		db_decimalPrecision = KvGetNum(kv, "Decimal_DecimalPrecision", 7);
	}
	CloseHandle(kv);
}

stock void ConnectToDatabase()
{
	LoadDatabaseSettings();
	char error[512];
	db = SQL_Connect("sql_ac_store", false, error, sizeof(error));
	if (db == INVALID_HANDLE)
	{
		db = SQL_DefConnect(error, sizeof(error), false);
		if (db == INVALID_HANDLE)
		{
			Log(EL_Abort, "STORESys", "Could not connect to the database. (Message: %s)", error); //stop this plugin here...
		}
		else
		{
			Log(EL_Info, "STORESys", "There is no 'sql_ac_store' entry in the databases.cfg. - Connecting using standart configuration...");
		}
	}
	char query[512];
	bool queryResult;
	if (db_useDecimals)
	{
		Format(query, sizeof(query), "CREATE TABLE IF NOT EXISTS `%s`(id INT NOT NULL AUTO_INCREMENT, steamid VARCHAR(32) UNIQUE, name VARCHAR(32), points DECIMAL(%i,%i), gained DECIMAL(%i,%i), spend DECIMAL(%i,%i), lastjoin DATE, skillforce INT, sfgained DECIMAL(%i,%i), sflevel TINYINT, sfskill TINYINT, sfskilllevel TINYINT, PRIMARY KEY (id))", tableName, db_decimalAbsolutPrecision, db_decimalPrecision, db_decimalAbsolutPrecision, db_decimalPrecision, db_decimalAbsolutPrecision, db_decimalPrecision, db_decimalAbsolutPrecision, db_decimalPrecision);
		SQL_LockDatabase(db);
		queryResult = SQL_FastQuery(db, query);
		if (queryResult)
		{
			Format(query, sizeof(query), "CREATE TABLE IF NOT EXISTS `%s`(id INT NOT NULL AUTO_INCREMENT, name VARCHAR(32) UNIQUE, membercount INT, account DECIMAL(%i,%i), gained DECIMAL(%i,%i), tax DECIMAL(%i,%i), PRIMARY KEY (id))", sfTableName, db_decimalAbsolutPrecision, db_decimalPrecision, db_decimalAbsolutPrecision, db_decimalPrecision, db_decimalAbsolutPrecision, db_decimalPrecision);
			queryResult = queryResult && SQL_FastQuery(db, query);
		}
	}
	else
	{
		Format(query, sizeof(query), "CREATE TABLE IF NOT EXISTS `%s`(id INT NOT NULL AUTO_INCREMENT, steamid VARCHAR(32) UNIQUE, name VARCHAR(32), points DOUBLE, gained DOUBLE, spend DOUBLE, lastjoin DATE, skillforce INT, sfgained DOUBLE, sflevel TINYINT, sfskill TINYINT, sfskilllevel TINYINT, PRIMARY KEY (id))", tableName);
		SQL_LockDatabase(db);
		queryResult = SQL_FastQuery(db, query);
		if (queryResult)
		{
			Format(query, sizeof(query), "CREATE TABLE IF NOT EXISTS `%s`(id INT NOT NULL AUTO_INCREMENT, name VARCHAR(32) UNIQUE, membercount INT, account DOUBLE, gained DOUBLE, tax DOUBLE, PRIMARY KEY (id))", sfTableName);
			queryResult = queryResult && SQL_FastQuery(db, query);
		}
	}
	SQL_UnlockDatabase(db);
	if (!queryResult)
	{
		SQL_GetError(db, error, sizeof(error));
		Log(EL_Abort, "STORESys", "Could not create the nessecary sql-tables. (Message: %s)", error); //stop this plugin here...
	}
}

stock bool IntToBool(int integer)
{
	if (integer == 0)
	{
		return false;
	}
	return true;
}