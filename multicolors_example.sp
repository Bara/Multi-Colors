#include <sourcemod>
#include <multicolors>

public Plugin:myinfo =
{
	name = "Example plugin for multi colors",
	author = "Bara",
	description = "Example plugin for multi colors",
	version = "1.0",
	url = "www.bara.in"
}

public OnPluginStart()
{
	RegConsoleCmd("sm_multicolors", Command_MultiColors);
}

public Action:Command_MultiColors(client, args)
{
	decl String:sName[MAX_NAME_LENGTH];
	GetClientName(client, sName, sizeof(sName));

	CPrintToChat(client, "CPrintToChat - {darkred}%s - {valve}%s", "Test", "Test");
	CPrintToChatAll("CPrintToChatAll - {darkred}%s - {valve}%s", "Test", "Test");
	CPrintToChatEx(client, client, "CPrintToChatEx - {darkred}%s - {valve}%s", "Test", "Test");
	CPrintToChatAllEx(client, "CPrintToChatAllEx - {darkred}%s - {valve}%s", "Test", "Test");

	CReplyToCommand(client, "CReplyToCommand - {darkred}%s - {valve}%s", "Test", "Test");
	CReplyToCommandEx(client, client, "CReplyToCommandEx - {darkred}%s - {valve}%s", "Test", "Test");

	CShowActivity(client, "CShowActivity - {darkred}%s - {valve}%s", "Test", "Test");
	CShowActivityEx(client, "[SM]", "CShowActivityEx - {darkred}%s - {valve}%s", "Test", "Test");
	CShowActivity2(client, "[SM]", "CShowActivity2 - {darkred}%s - {valve}%s", "Test", "Test");

	CFormatColor(sName, MAX_NAME_LENGTH, client);
}