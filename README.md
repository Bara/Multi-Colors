Multi Colors
============


| Action Status | Discord |
|:-------------:|:-------:|
| [![Action Status](https://github.com/Bara/Multi-Colors/workflows/Compile%20with%20SourceMod/badge.svg)](https://github.com/Bara/Multi-Colors/actions) | [![Discord](https://img.shields.io/discord/388685157286019072.svg)](https://discord.gg/NUMQfgs) |


(based on Stamm-Colors) is a summary of Colors and More Colors. Depending on the game More Colors will be used as default. Should the game be CS:GO Colors will be used. The include providing a perfect base for plugins supporting multiple games.

### Functions:
```sp
CPrintToChat(int client, const char[] message, any ...)
CPrintToChatAll(const char[] message, an ...)
CPrintToChatEx(int client, int author, const char[] message, any ...)
CPrintToChatAllEx(int author, const char[] message, any ...)
CReplyToCommand(int author, const char[] message, any ...)
CReplyToCommandEx(int client, int author, const char[] message, any ...)
CShowActivity(int author, const char[] message, any ...)
CShowActivityEx(int author, const char[] tag, const char[] message, any ...)
CShowActivity2(int author, const char[] tag, cconst char[] message, any ...)
CPrintToServer(const char[] message, any ...)
CFormatColor(char[] message, int maxlength, int author = -1)
CRemoveTags(char[] message, int maxlen)
CSetPrefix(const char[] sPrefix, any ...)
CClearPrefix()
```
By using C* Functions, offers these include a good compatibility to Colors and More Colors.

### Links:
 + Stamm-Colors: https://github.com/popoklopsi/Stamm/blob/master/include/stamm/stamm-colors.inc
 + Colors: https://forums.alliedmods.net/showthread.php?t=96831
 + More Colors: https://forums.alliedmods.net/showthread.php?t=185016

### Download:
 + https://github.com/Bara/Multi-Colors

### Example plugin:
```sp
#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <multicolors>

public void OnPluginStart()
{
	RegConsoleCmd("sm_multicolors", Command_MultiColors);
	RegConsoleCmd("sm_addprefix", Command_AddPrefix);
	RegConsoleCmd("sm_clearprefix", Command_ClearPrefix);
}

public Action Command_AddPrefix(int client, int args)
{
	if (IsSource2009())
	{
		CSetPrefix("{#FF0000AA}[{#FFFF00}MultiColor{#FF0000AA}]");
		CReplyToCommand(client, "You can use {#FFFFFF}sm_multicolors {default}now !");
	}
	else
	{
		CSetPrefix("{orange}[MultiColor]{default}");
		CReplyToCommand(client, "You can use {yellow}sm_multicolors {default}now !");
	}

	return Plugin_Continue;
}

public Action Command_ClearPrefix(int client, int args)
{
	CClearPrefix();

	if (IsSource2009())
	{
		CReplyToCommand(client, "You can use {#FFFFFF}sm_multicolors {default}now !");
	}
	else
	{
		CReplyToCommand(client, "You can use {yellow}sm_multicolors {default}now !");
	}

	return Plugin_Continue;
}

public Action Command_MultiColors(int client, int args)
{
	CPrintToChat(client, "CPrintToChat - {darkblue}%s - {darkred}%s", "Test", "Test");
	CPrintToChatAll("CPrintToChatAll - {darkblue}%s - {darkred}%s", "Test", "Test");
	CPrintToChatEx(client, client, "CPrintToChatEx - {darkblue}%s - {darkred}%s", "Test", "Test");
	CPrintToChatAllEx(client, "CPrintToChatAllEx - {darkblue}%s - {darkred}%s", "Test", "Test");

	CReplyToCommand(client, "CReplyToCommand - {darkblue}%s - {darkred}%s", "Test", "Test");
	CReplyToCommandEx(client, client, "CReplyToCommandEx - {darkblue}%s - {darkred}%s", "Test", "Test");

	CShowActivity(client, "CShowActivity - {darkblue}%s - {darkred}%s", "Test", "Test");
	CShowActivityEx(client, "[SM]", "CShowActivityEx - {darkblue}%s - {darkred}%s", "Test", "Test");
	CShowActivity2(client, "[SM]", "CShowActivity2 - {darkblue}%s - {darkred}%s", "Test", "Test");

	char sName[MAX_NAME_LENGTH];
	Format(sName, sizeof(sName), "{green}%N", client);

	CPrintToChat(client, "CPrintToChat - Name (Before CFormatColor): %s", sName);
	PrintToChat(client, "PrintToChat - Name (Before CFormatColor): %s", sName);
	CFormatColor(sName, MAX_NAME_LENGTH, client);
	CPrintToChat(client, "CPrintToChat - Name (After CFormatColor): %s", sName);
	PrintToChat(client, "PrintToChat - Name (After CFormatColor): %s", sName);

	Format(sName, sizeof(sName), "{green}%N", client);

	CPrintToChat(client, "Name (Before CFormatColor): %s", sName);
	CRemoveTags(sName, MAX_NAME_LENGTH);
	CPrintToChat(client, "Name (After CFormatColor): %s", sName);

	CPrintToChatObservers(client, "CPrintToChatObservers - {darkblue}%s - {darkred}%s", "Test", "Test");
	CPrintToChatObserversEx(client, "CPrintToChatObserversEx - {darkblue}%s - {darkred}%s", "Test", "Test");

	CPrintToServer("CPrintToServer - {darkblue}%s - {darkred}%s", "Test", "Test");

	return Plugin_Continue;
}
```
