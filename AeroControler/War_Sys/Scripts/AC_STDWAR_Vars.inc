/*
**
** Aero Jail Controler Plugin
** Standart WARs - Partial variables file
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

int ExtensionEntryIndex = -1;

bool Ready = false;
bool WarOnline = false;

bool NormalWar_Enabled = true;
bool NormalWar_Online = false;

bool HCWar_Enabled = true;
bool HCWar_Online = false;
float HCWar_DmgFactor = 5.0;

bool FunWar_Enabled = true;
bool FunWar_Online = false;
int FunWar_MinHp = 1;
int FunWar_MaxHp = 500;
float FunWar_MaxSpeed = 3.0;
float FunWar_MinGrav = 0.1;