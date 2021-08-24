#if defined _multicolors_included
	#endinput
#endif
#define _multicolors_included

#define MuCo_VERSION "2.1.2"

#include <multicolors/morecolors>

/*
*
*   Credits:
*       - Popoklopsi
*       - Powerlord
*       - exvel
*       - Dr. McKay
*       - maxime1907
*
*   Based on stamm-colors
*       - https://github.com/popoklopsi/Stamm/blob/master/include/stamm/stamm-colors.inc
*
*/



#define PREFIX_MAX_LENGTH 64
#define PREFIX_SEPARATOR "{default} "

static char g_sCPrefix[PREFIX_MAX_LENGTH];

/**
 * Add a chat prefix before all chat msg
 *
 * @param sPrefix		Prefix
 */
stock void CSetPrefix(const char[] sPrefix, any ...) {
	if (!sPrefix[0]) {
		return;
	}

	SetGlobalTransTarget(LANG_SERVER);
	VFormat(g_sCPrefix, sizeof(g_sCPrefix) - strlen(PREFIX_SEPARATOR), sPrefix, 2);

	// Add ending space
	Format(g_sCPrefix, sizeof(g_sCPrefix), "%s%s", g_sCPrefix, PREFIX_SEPARATOR);
}

/**
 * Add a chat prefix before all chat msg
 *
 * @param sPrefix		Prefix
 */
stock void CClearPrefix() {
	g_sCPrefix[0] = '\0';
}

/**
 * Writes a message to a client with the correct stock for the game.
 *
 * @param client        Client index.
 * @param message       Message (formatting rules).
 *
 * @error               If the client is not connected an error will be thrown.
 */
stock void CPrintToChat(int client, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(client);
	VFormat(buffer, sizeof(buffer), message, 3);

	MC_PrintToChat(client, "%s%s", g_sCPrefix, buffer);
}

/**
 * Prints a message to all clients in the chat area.
 * Supports color tags.
 *
 * @param client	  Client index.
 * @param message     Message (formatting rules)
 */
stock void CPrintToChatAll(const char[] message, any ...) {
	char buffer[MAX_BUFFER_LENGTH];
	MC_CheckTrie();

	char buffer2[MAX_BUFFER_LENGTH];

	for (int i = 1; i <= MaxClients; ++i) {
		if (!IsClientInGame(i) || MC_SkipList[i]) {
			MC_SkipList[i] = false;
			continue;
		}

		SetGlobalTransTarget(i);
		Format(buffer, sizeof(buffer), "\x01%s", message);
		VFormat(buffer2, sizeof(buffer2), buffer, 2);

		MC_ReplaceColorCodes(buffer2);
		MC_SendMessage(i, buffer2);
	}
}

/**
 * Writes a message to all of a client's observers.
 *
 * @param target 	Client index.
 * @param message	Message (formatting rules).
 */
stock void CPrintToChatObservers(int target, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(LANG_SERVER);
	VFormat(buffer, sizeof(buffer), message, 3);

 	for (int client = 1; client <= MaxClients; client++) {
 		if (IsClientInGame(client) && !IsPlayerAlive(client) && !IsFakeClient(client)) {
 			int observee = GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
 			int ObserverMode = GetEntProp(client, Prop_Send, "m_iObserverMode");
 
 			if (observee == target && (ObserverMode == 4 || ObserverMode == 5)) {
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
 * @error               If the client is not connected an error will be thrown.
 */
stock void CPrintToChatEx(int client, int author, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(client);
	VFormat(buffer, sizeof(buffer), message, 4);

	MC_PrintToChatEx(client, author, "%s%s", g_sCPrefix, buffer);
}

/**
 * Writes a message to all clients with the correct stock for the game.
 *
 * @param author        Author index.
 * @param message       Message (formatting rules).
 */
stock void CPrintToChatAllEx(int author, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(LANG_SERVER);
	VFormat(buffer, sizeof(buffer), message, 3);

	MC_PrintToChatAllEx(author, "%s%s", g_sCPrefix, buffer);
}

/**
 * Writes a message to all of a client's observers with the correct
 * game stock.
 *
 * @param target 	Client index.
 * @param message	Message (formatting rules).
 */
stock void CPrintToChatObserversEx(int target, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(LANG_SERVER);
	VFormat(buffer, sizeof(buffer), message, 3);

 	for (int client = 1; client <= MaxClients; client++) {
 		if (IsClientInGame(client) && !IsPlayerAlive(client) && !IsFakeClient(client)) {
 			int observee = GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
 			int ObserverMode = GetEntProp(client, Prop_Send, "m_iObserverMode");
 
 			if (observee == target && (ObserverMode == 4 || ObserverMode == 5)) {
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
 */
stock void CReplyToCommand(int client, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(client);
	VFormat(buffer, sizeof(buffer), message, 3);

	MC_ReplyToCommand(client, "%s%s", g_sCPrefix, buffer);
}

/**
 * Replies to a command with colors
 * 
 * @param client		Client to reply to
 * @param author		Client to use for {teamcolor}
 * @param message		Message (formatting rules)
 */
stock void CReplyToCommandEx(int client, int author, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(client);
	VFormat(buffer, sizeof(buffer), message, 4);

	MC_ReplyToCommandEx(client, author, "%s%s", g_sCPrefix, buffer);
}

/**
 * Remove all tags and print to server
 * 
 * @param message		Message (formatting rules)
 */
stock void CPrintToServer(const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	char prefixBuffer[PREFIX_MAX_LENGTH];
	SetGlobalTransTarget(LANG_SERVER);
	VFormat(buffer, sizeof(buffer), message, 2);
	strcopy(prefixBuffer, sizeof(prefixBuffer), g_sCPrefix);

	CRemoveTags(buffer, sizeof(buffer));
	CRemoveTags(prefixBuffer, sizeof(prefixBuffer));

	PrintToServer("%s%s", prefixBuffer, buffer);
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
 * 
 * @error
 */
stock void CShowActivity(int author, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(LANG_SERVER);
	VFormat(buffer, sizeof(buffer), message, 3);

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
 * 
 * @error
 */
stock void CShowActivityEx(int author, const char[] tag, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(LANG_SERVER);
	VFormat(buffer, sizeof(buffer), message, 4);

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
 * 
 * @error
 */
 stock void CShowActivity2(int author, const char[] tag, const char[] message, any ...) {
	char buffer[MC_MAX_MESSAGE_LENGTH];
	SetGlobalTransTarget(LANG_SERVER);
	VFormat(buffer, sizeof(buffer), message, 4);

	MC_ShowActivity2(author, tag, "%s", buffer);
}

/**
 * Replaces color tags in a string with color codes
 *
 * @param message       String.
 * @param maxlength     Maximum length of the string buffer.
 */
stock void CFormatColor(char[] message, int maxlength, int author = -1) {
	if (author == -1) {
		author = 0;
	}

	MC_ReplaceColorCodes(message, author, false, maxlength);
}

/**
 * Removes color tags from a message
 * 
 * @param message		Message to remove tags from
 * @param maxlen		Maximum buffer length
 */
stock void CRemoveTags(char[] message, int maxlen) {
	MC_RemoveTags(message, maxlen);
}

stock bool IsSource2009() {
	return (GetEngineVersion() == Engine_CSS
	|| GetEngineVersion() == Engine_HL2DM
	|| GetEngineVersion() == Engine_DODS
	|| GetEngineVersion() == Engine_TF2
	|| GetEngineVersion() == Engine_SDK2013);
}
