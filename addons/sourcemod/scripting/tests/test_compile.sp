#pragma semicolon 1

#include <sourcemod>
#include <multicolors>

public Plugin myinfo = {
	name = "Multi Colors Testing Suite",
	author = "Bara",
	description = "Multi Colors Testing Suite",
	version = "1.0",
	url = ""
}

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
