#if defined _mutlicolors_included
	#endinput
#endif
#define _mutlicolors_included

#define MuCo_VERSION "2.0.1"
#define MuCo_LoopClients(%1) for(int %1 = 1; %1 <= MaxClients; %1++)

#include <multicolors/morecolors>
#include <multicolors/colors>

/*
*
*   Credits:
*       - Popoklopsi
*       - Powerlord
*       - exvel
*       - Dr. McKay
*
*   Based on stamm-colors
*       - https://github.com/popoklopsi/Stamm/blob/master/include/stamm/stamm-colors.inc
*
*/



/* Global var to check whether colors are fixed or not */
bool g_bCFixColors = false;



/**
 * Writes a message to a client with the correct stock for the game.
 *
 * @param client        Client index.
 * @param message       Message (formatting rules).
 *
 * @noreturn
 * @error               If the client is not connected an error will be thrown.
 */
stock void CPrintToChat(int client, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 3);

	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
		C_PrintToChat(client, "%s", buffer);
	else
		MC_PrintToChat(client, "%s", buffer);
}



/**
 * Prints a message to all clients in the chat area.
 * Supports color tags.
 *
 * @param client	  Client index.
 * @param message     Message (formatting rules)
 * @return			  No return
 */
stock void CPrintToChatAll(const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 2);

	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
		C_PrintToChatAll("%s", buffer);
	else
		MC_PrintToChatAll("%s", buffer);
}

/**
 * Writes a message to all of a client's observers.
 *
 * @param target 	Client index.
 * @param message	Message (formatting rules).
 *
 * @noreturn
 */
stock void CPrintToChatObservers(int target, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 3);

	if (!g_bCFixColors)
		CFixColors();
	
 	for(int client = 1; client <= MaxClients; client++)
	{
 		if(IsClientInGame(client) && !IsPlayerAlive(client) && !IsFakeClient(client))
		{
 			int observee 		= GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
 			int ObserverMode 	= GetEntProp(client, Prop_Send, "m_iObserverMode");
 
 			if(observee == target && (ObserverMode == 4 || ObserverMode == 5))
			{
 				CPrintToChat(client, buffer);
 			}
 		}
 	}
}


/**
 * Writes a message to a client with the correct stock for the game.
 *
 * @param client        Client index.
 * @param author        Author index.
 * @param message       Message (formatting rules).
 *
 * @noreturn
 * @error               If the client is not connected an error will be thrown.
 */
stock void CPrintToChatEx(int client, int author, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 4);

	if (!g_bCFixColors)
		CFixColors();
	
	if (!IsSource2009())
		C_PrintToChatEx(client, author, "%s", buffer);
	else
		MC_PrintToChatEx(client, author, "%s", buffer);
}

/**
 * Writes a message to all clients with the correct stock for the game.
 *
 * @param author        Author index.
 * @param message       Message (formatting rules).
 *
 * @noreturn
 */
stock void CPrintToChatAllEx(int author, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 3);

	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
		C_PrintToChatAllEx(author, "%s", buffer);
	else
		MC_PrintToChatAllEx(author, "%s", buffer);
}

/**
 * Writes a message to all of a client's observers with the correct
 * game stock.
 *
 * @param target 	Client index.
 * @param message	Message (formatting rules).
 *
 * @noreturn
 */
stock void CPrintToChatObserversEx(int target, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 3);

	if (!g_bCFixColors)
		CFixColors();
	
 	for(int client = 1; client <= MaxClients; client++)
	{
 		if(IsClientInGame(client) && !IsPlayerAlive(client) && !IsFakeClient(client))
		{
 			int observee 		= GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
 			int ObserverMode 	= GetEntProp(client, Prop_Send, "m_iObserverMode");
 
 			if(observee == target && (ObserverMode == 4 || ObserverMode == 5))
			{
 				CPrintToChatEx(client, target, buffer);
 			}
 		}
 	}
}


/**
 * Replies to a command with colors
 * 
 * @param client		Client to reply to
 * @param message		Message (formatting rules)
 * @noreturn
 */
stock void CReplyToCommand(int author, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 3);

	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
		C_ReplyToCommand(author, "%s", buffer);
	else
		MC_ReplyToCommand(author, "%s", buffer);
}



/**
 * Replies to a command with colors
 * 
 * @param client		Client to reply to
 * @param author		Client to use for {teamcolor}
 * @param message		Message (formatting rules)
 * @noreturn
 */
 stock void CReplyToCommandEx(int client, int author, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 4);

	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
		C_ReplyToCommandEx(client, author, "%s", buffer);
	else
		MC_ReplyToCommandEx(client, author, "%s", buffer);
}



/**
 * Displays usage of an admin command to users depending on the 
 * setting of the sm_show_activity cvar.  
 *
 * This version does not display a message to the originating client 
 * if used from chat triggers or menus.  If manual replies are used 
 * for these cases, then this function will suffice.  Otherwise, 
 * CShowActivity2() is slightly more useful.
 * Supports color tags.
 *
 * @param client		Client index doing the action, or 0 for server.
 * @param format		Formatting rules.
 * @param ...			Variable number of format parameters.
 * @noreturn
 * @error
 */
stock void CShowActivity(int author, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 3);

	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
		C_ShowActivity(author, "%s", buffer);
	else
		MC_ShowActivity(author, "%s", buffer);
}


/**
 * Same as C_ShowActivity(), except the tag parameter is used instead of "[SM] " (note that you must supply any spacing).
 * Supports color tags.
 *
 * @param client		Client index doing the action, or 0 for server.
 * @param tags			Tag to display with.
 * @param format		Formatting rules.
 * @param ...			Variable number of format parameters.
 * @noreturn
 * @error
 */
stock void CShowActivityEx(int author, const char[] tag, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 4);

	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
		C_ShowActivityEx(author, tag, "%s", buffer);
	else
		MC_ShowActivityEx(author, tag, "%s", buffer);
}



/**
 * Displays usage of an admin command to users depending on the setting of the sm_show_activity cvar.
 * All users receive a message in their chat text, except for the originating client, 
 * who receives the message based on the current ReplySource.
 * Supports color tags.
 *
 * @param client		Client index doing the action, or 0 for server.
 * @param tags			Tag to prepend to the message.
 * @param format		Formatting rules.
 * @param ...			Variable number of format parameters.
 * @noreturn
 * @error
 */
 stock void CShowActivity2(int author, const char[] tag, const char[] message, any ...)
{
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 4);

	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
		C_ShowActivity2(author, tag, "%s", buffer);
	else
		MC_ShowActivity2(author, tag, "%s", buffer);
}



/**
 * Replaces color tags in a string with color codes
 *
 * @param message       String.
 * @param maxlength     Maximum length of the string buffer.
 *
 * @noreturn
 */
stock void CFormatColor(char[] message, int maxlength, int author = -1)
{
	if (!g_bCFixColors)
		CFixColors();

	if (!IsSource2009())
	{
		if (author == 0)
			author = -1;
	
		C_Format(message, maxlength, author);
	}
	else
	{
		if (author == -1)
			author = 0;

		MC_ReplaceColorCodes(message, author, false, maxlength);
	}
}



/**
 * Removes color tags from a message
 * 
 * @param message		Message to remove tags from
 * @param maxlen		Maximum buffer length
 * @noreturn
 */
stock void CRemoveTags(char[] message, int maxlen)
{
	if (!IsSource2009())
	{
		C_RemoveTags(message, maxlen);
	}
	else
	{
		MC_RemoveTags(message, maxlen);
	}
}



/**
 * Fixes missing Lightgreen color.
 *
 * @noreturn
 */
stock void CFixColors()
{
	g_bCFixColors = true;

	// Replace lightgreen if not exists
	if (!C_ColorAllowed(Color_Lightgreen))
	{
		if (C_ColorAllowed(Color_Lime))
			C_ReplaceColor(Color_Lightgreen, Color_Lime);
		else if (C_ColorAllowed(Color_Olive))
			C_ReplaceColor(Color_Lightgreen, Color_Olive);
	}
}

stock bool IsSource2009()
{
	if(GetEngineVersion() == Engine_CSS || GetEngineVersion() == Engine_HL2DM || GetEngineVersion() == Engine_DODS || GetEngineVersion() == Engine_TF2)
	{
		return true;
	}
	return false;
}
