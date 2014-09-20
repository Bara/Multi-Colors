#if defined _mutlicolors_included
	#endinput
#endif
#define _mutlicolors_included

#include <multicolors/colors>
#include <multicolors/morecolors>

#define MuCo_VERSION "1.0.1"

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
new bool:g_bCFixColors = false;



/**
 * Writes a message to a client with the correct stock for the game.
 *
 * @param client        Client index.
 * @param message       Message (formatting rules).
 *
 * @noreturn
 * @error               If the client is not connected an error will be thrown.
 */
stock CPrintToChat(client, const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 3);


	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		C_PrintToChat(client, buffer);
	}
	else
	{
		MC_PrintToChat(client, buffer);
	}
}



/**
 * Prints a message to all clients in the chat area.
 * Supports color tags.
 *
 * @param client	  Client index.
 * @param message     Message (formatting rules)
 * @return			  No return
 */
stock CPrintToChatAll(const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 2);


	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		C_PrintToChatAll(buffer);
	}
	else
	{
		MC_PrintToChatAll(buffer);
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
stock CPrintToChatEx(client, author, const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 4);


	if (!g_bCFixColors)
	{
		CFixColors();
	}
	

	if (!IsSource2009())
	{
		C_PrintToChatEx(client, author, buffer);
	}
	else
	{
		MC_PrintToChatEx(client, author, buffer);
	}
}



/**
 * Writes a message to all clients with the correct stock for the game.
 *
 * @param author        Author index.
 * @param message       Message (formatting rules).
 *
 * @noreturn
 */
stock CPrintToChatAllEx(author, const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 3);


	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		C_PrintToChatAllEx(author, buffer);
	}
	else
	{
		MC_PrintToChatAllEx(author, buffer);
	}
}




/**
 * Replies to a command with colors
 * 
 * @param client		Client to reply to
 * @param message		Message (formatting rules)
 * @noreturn
 */
stock CReplyToCommand(author, const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 3);


	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		C_ReplyToCommand(author, buffer);
	}
	else
	{
		MC_ReplyToCommand(author, buffer);
	}
}



/**
 * Replies to a command with colors
 * 
 * @param client		Client to reply to
 * @param author		Client to use for {teamcolor}
 * @param message		Message (formatting rules)
 * @noreturn
 */
 stock CReplyToCommandEx(client, author, const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 4);


	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		C_ReplyToCommandEx(client, author, buffer);
	}
	else
	{
		MC_ReplyToCommandEx(client, author, buffer);
	}
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
stock CShowActivity(author, const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 3);


	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		C_ShowActivity(author, buffer);
	}
	else
	{
		MC_ShowActivity(author, buffer);
	}
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
stock CShowActivityEx(author, const String:tag[], const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 4);


	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		C_ShowActivityEx(author, tag, buffer);
	}
	else
	{
		MC_ShowActivityEx(author, tag, buffer);
	}
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
 stock CShowActivity2(author, const String:tag[], const String:message[], any:...)
{
	decl String:buffer[MAX_MESSAGE_LENGTH];

	VFormat(buffer, sizeof(buffer), message, 4);


	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		C_ShowActivity2(author, tag, buffer);
	}
	else
	{
		MC_ShowActivity2(author, tag, buffer);
	}
}



/**
 * Replaces color tags in a string with color codes
 *
 * @param message       String.
 * @param maxlength     Maximum length of the string buffer.
 *
 * @noreturn
 */
stock CFormatColor(String:message[], maxlength, author=-1)
{
	if (!g_bCFixColors)
	{
		CFixColors();
	}


	if (!IsSource2009())
	{
		if (author == 0)
		{
			author = -1;
		}

		C_Format(message, maxlength, author);
	}
	else
	{
		if (author == -1)
		{
			author = 0;
		}

		MC_ReplaceColorCodes(message, author, false, maxlength);
	}
}



/**
 * Fixes missing Lightgreen color.
 *
 * @noreturn
 */
stock CFixColors()
{
	g_bCFixColors = true;

	// Replace lightgreen if not exists
	if (!C_ColorAllowed(Color_Lightgreen))
	{
		if (C_ColorAllowed(Color_Lime))
		{
			C_ReplaceColor(Color_Lightgreen, Color_Lime);
		}
		else if (C_ColorAllowed(Color_Olive))
		{
			C_ReplaceColor(Color_Lightgreen, Color_Olive);
		}
	}
}

stock IsSource2009()
{
	if(GetEngineVersion() == Engine_CSS || GetEngineVersion() == Engine_HL2DM || GetEngineVersion() == Engine_DODS || GetEngineVersion() == Engine_TF2)
	{
		return true;
	}
	else
	{
		return false;
	}
}