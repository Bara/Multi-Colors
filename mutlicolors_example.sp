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

	CPrintToChat(client, "{darkred}Test - {valve}Test");
	CPrintToChatAll("{darkred}Test - {valve}Test");
	CPrintToChatEx(client, client, "{darkred}Test - {valve}Test");
	CPrintToChatAllEx(client, "{darkred}Test - {valve}Test");

	CReplyToCommand(client, "{darkred}Test - {valve}Test");
	CReplyToCommandEx(client, client, "{darkred}Test - {valve}Test");

	CShowActivity(client, "{darkred}Test - {valve}Test");
	CShowActivityEx(client, "[SM]", "{darkred}Test - {valve}Test");
	CShowActivity2(client, "[SM]", "{darkred}Test - {valve}Test");

	CFormatColor(sName, MAX_NAME_LENGTH, client);
}