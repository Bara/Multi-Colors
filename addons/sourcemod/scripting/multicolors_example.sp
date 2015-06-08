#pragma semicolon 1

#include <sourcemod>
#include <multicolors>

#pragma newdecls required

public Plugin myinfo =
{
	name = "Example plugin for multi colors",
	author = "Bara",
	description = "Example plugin for multi colors",
	version = MuCo_VERSION,
	url = "www.bara.in"
}

public void OnPluginStart()
{
	RegConsoleCmd("sm_multicolors", Command_MultiColors);
}

public Action Command_MultiColors(int client, int args)
{
	char sName[MAX_NAME_LENGTH];
	GetClientName(client, sName, sizeof(sName));

	CPrintToChat(client, "CPrintToChat - {darkblue}%s - {valve}%s", "Test", "Test");
	CPrintToChatAll("CPrintToChatAll - {darkblue}%s - {valve}%s", "Test", "Test");
	CPrintToChatEx(client, client, "CPrintToChatEx - {darkblue}%s - {valve}%s", "Test", "Test");
	CPrintToChatAllEx(client, "CPrintToChatAllEx - {darkblue}%s - {valve}%s", "Test", "Test");

	CReplyToCommand(client, "CReplyToCommand - {darkblue}%s - {valve}%s", "Test", "Test");
	CReplyToCommandEx(client, client, "CReplyToCommandEx - {darkblue}%s - {valve}%s", "Test", "Test");

	CShowActivity(client, "CShowActivity - {darkblue}%s - {valve}%s", "Test", "Test");
	CShowActivityEx(client, "[SM]", "CShowActivityEx - {darkblue}%s - {valve}%s", "Test", "Test");
	CShowActivity2(client, "[SM]", "CShowActivity2 - {darkblue}%s - {valve}%s", "Test", "Test");

	CFormatColor(sName, MAX_NAME_LENGTH, client);
}