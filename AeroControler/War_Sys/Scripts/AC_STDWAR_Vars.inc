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

new ExtensionEntryIndex = -1;

new bool:Ready = false;
new bool:WarOnline = false;

new bool:NormalWar_Enabled = true;
new bool:NormalWar_Online = false;

new bool:HCWar_Enabled = true;
new bool:HCWar_Online = false;
new Float:HCWar_DmgFactor = 5.0;

new bool:FunWar_Enabled = true;
new bool:FunWar_Online = false;
new FunWar_MinHp = 1;
new FunWar_MaxHp = 500;
new Float:FunWar_MaxSpeed = 3.0;
new Float:FunWar_MinGrav = 0.1;