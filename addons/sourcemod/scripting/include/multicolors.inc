#if defined _multicolors_included
	#endinput
#endif
#define _multicolors_included

#define MuCo_VERSION "2.2.0"

#pragma newdecls required

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

#include <regex>

#define MAX_MESSAGE_LENGTH   256
#define MAX_BUFFER_LENGTH       (MAX_MESSAGE_LENGTH * 4)

#define GAME_DODS           0

#define PREFIX_MAX_LENGTH 64
#define PREFIX_SEPARATOR "{default} "

static EngineVersion g_evEngineVersion = Engine_Unknown;
static bool g_bSkipList[MAXPLAYERS+1];
static StringMap g_smTrie;
static ConVar g_cShowActivity;
static char g_sPrefix[PREFIX_MAX_LENGTH];
static int g_iTeamColors[][] = {
	{
		0xCCCCCC,
		0x4D7942,
		0xFF4040
	}
}; // Multi-dimensional array for games that don't support SayText2. First index is the game index (as defined by the GAME_ defines), second index is team. 0 = spectator, 1 = team1, 2 = team2

/**
 * Prints a message to a specific client in the chat area.
 * Supports color tags.
 * 
 * @param client		Client index.
 * @param message		Message (formatting rules).
 * 
 * On error/Errors:		If the client is not connected an error will be thrown.
 */
stock void CPrintToChat(int client, const char[] message, any ...) {
	CCheckTrie();

	if (client <= 0 || client > MaxClients) {
		ThrowError("Invalid client index %i", client);
	}

	if (!IsClientInGame(client)) {
		ThrowError("Client %i is not in game", client);
	}

	char buffer[MAX_BUFFER_LENGTH];
	char buffer2[MAX_BUFFER_LENGTH];

	SetGlobalTransTarget(client);
	Format(buffer, sizeof(buffer), "\x01%s", message);
	VFormat(buffer2, sizeof(buffer2), buffer, 3);

	CReplaceColorCodes(buffer2);
	CSendMessage(client, buffer2);
}

/**
 * Prints a message to all clients in the chat area.
 * Supports color tags.
 * 
 * @param client		Client index.
 * @param message		Message (formatting rules).
 */
stock void CPrintToChatAll(const char[] message, any ...) {
	CCheckTrie();

	char buffer[MAX_BUFFER_LENGTH], buffer2[MAX_BUFFER_LENGTH];

	for (int i = 1; i <= MaxClients; ++i) {
		if (!IsClientInGame(i) || g_bSkipList[i]) {
			g_bSkipList[i] = false;
			continue;
		}

		SetGlobalTransTarget(i);
		Format(buffer, sizeof(buffer), "\x01%s", message);
		VFormat(buffer2, sizeof(buffer2), buffer, 2);

		CReplaceColorCodes(buffer2);
		CSendMessage(i, buffer2);
	}
}

/**
 * Prints a message to a specific client in the chat area.
 * Supports color tags and teamcolor tag.
 * 
 * @param client		Client index.
 * @param author		Author index whose color will be used for teamcolor tag.
 * @param message		Message (formatting rules).
 * 
 * On error/Errors:		If the client or author are not connected an error will be thrown
 */
stock void CPrintToChatEx(int client, int author, const char[] message, any ...) {
	CCheckTrie();

	if (client <= 0 || client > MaxClients) {
		ThrowError("Invalid client index %i", client);
	}

	if (!IsClientInGame(client)) {
		ThrowError("Client %i is not in game", client);
	}

	if (author < 0 || author > MaxClients) {
		ThrowError("Invalid client index %i", author);
	}

	if ((author != 0) && !IsClientInGame(author)) {
		ThrowError("Client %i is not in game", author);
	}

	char buffer[MAX_BUFFER_LENGTH], buffer2[MAX_BUFFER_LENGTH];
	SetGlobalTransTarget(client);
	Format(buffer, sizeof(buffer), "\x01%s", message);
	VFormat(buffer2, sizeof(buffer2), buffer, 4);
	CReplaceColorCodes(buffer2, author);
	CSendMessage(client, buffer2, author);
}

/**
 * Prints a message to all clients in the chat area.
 * Supports color tags and teamcolor tag.
 *
 * @param author	  Author index whose color will be used for teamcolor tag.
 * @param message   Message (formatting rules).
 * 
 * On error/Errors:   If the author is not connected an error will be thrown.
 */
stock void CPrintToChatAllEx(int author, const char[] message, any ...) {
	CCheckTrie();

	if (author < 0 || author > MaxClients) {
		ThrowError("Invalid client index %i", author);
	}

	if ((author != 0) && !IsClientInGame(author)) {
		ThrowError("Client %i is not in game", author);
	}

	char buffer[MAX_BUFFER_LENGTH];
	char buffer2[MAX_BUFFER_LENGTH];

	for (int i = 1; i <= MaxClients; ++i) {
		if (!IsClientInGame(i) || g_bSkipList[i]) {
			g_bSkipList[i] = false;
			continue;
		}

		SetGlobalTransTarget(i);
		Format(buffer, sizeof(buffer), "\x01%s", message);
		VFormat(buffer2, sizeof(buffer2), buffer, 3);

		CReplaceColorCodes(buffer2, author);
		CSendMessage(i, buffer2, author);
	}
}

/**
 *
 * Adds a whitespace for games other than Source2009
 *
 * @param buffer	Buffer that contains the chat message
 * @param size		Size of the buffer
 */
stock void CAddWhiteSpace(char[] buffer, int size)
{
	if (!IsSource2009() && !(g_evEngineVersion == Engine_Left4Dead) && !(g_evEngineVersion == Engine_Left4Dead2)) {
		Format(buffer, size, " %s", buffer);
	}
}

/**
 * Sends a SayText2 usermessage
 * 
 * @param client	Client to send usermessage to
 * @param message	Message to send
 */
stock void CSendMessage(int client, const char[] message, int author = 0) {
	if (author == 0) {
		author = client;
	}

	char buffer[MAX_MESSAGE_LENGTH];
	strcopy(buffer, sizeof(buffer), message);

	CAddPrefix(buffer, sizeof(buffer));
	CAddWhiteSpace(buffer, sizeof(buffer));

	UserMsg index = GetUserMessageId("SayText2");
	if (index == INVALID_MESSAGE_ID) {
		if (IsSource2009() && g_evEngineVersion == Engine_DODS) {
			int team = GetClientTeam(author);
			if (team == 0) {
				ReplaceString(buffer, sizeof(buffer), "\x03", "\x04", false); // Unassigned gets green
			}
			else {
				char temp[16];
				Format(temp, sizeof(temp), "\x07%06X", g_iTeamColors[GAME_DODS][team - 1]);
				ReplaceString(buffer, sizeof(buffer), "\x03", temp, false);
			}
		}

		PrintToChat(client, "%s", buffer);
		return;
	}

	Handle buf = StartMessageOne("SayText2", client, USERMSG_RELIABLE|USERMSG_BLOCKHOOKS);
	if (GetFeatureStatus(FeatureType_Native, "GetUserMessageType") == FeatureStatus_Available && GetUserMessageType() == UM_Protobuf) {
		Protobuf pb = UserMessageToProtobuf(buf);
		pb.SetInt("ent_idx", author);
		pb.SetBool("chat", true);
		pb.SetString("msg_name", buffer);
		pb.AddString("params", "");
		pb.AddString("params", "");
		pb.AddString("params", "");
		pb.AddString("params", "");
	}
	else {
		BfWrite bf = UserMessageToBfWrite(buf);
		bf.WriteByte(author); // Message author
		bf.WriteByte(true); // Chat message
		bf.WriteString(buffer); // Message text
	}

	EndMessage();
}

/**
 * This function should only be used right in front of
 * CPrintToChatAll or CPrintToChatAllEx. It causes those functions
 * to skip the specified client when printing the message.
 * After printing the message, the client will no longer be skipped.
 * 
 * @param client   Client index
 */
stock void CSkipNextClient(int client) {
	if (client <= 0 || client > MaxClients) {
		ThrowError("Invalid client index %i", client);
	}
	
	g_bSkipList[client] = true;
}

/**
 * Checks if the colors trie is initialized and initializes it if it's not (used internally)
 * 
 * @return			No return
 */
stock void CCheckTrie() {
	if (g_smTrie == null) {
		g_smTrie = CInitColorTrie();
	}
}

stock StringMap CGetTrie() {
	CCheckTrie();
	return g_smTrie;
}

/**
 * Replaces color tags in a string with color codes (used internally by CPrintToChat, CPrintToChatAll, CPrintToChatEx, and CPrintToChatAllEx
 *
 * @param buffer		String.
 * @param author		Optional client index to use for {teamcolor} tags, or 0 for none
 * @param removeTags	Optional boolean value to determine whether we're replacing tags with colors, or just removing tags, used by CRemoveTags
 * @param maxlen		Optional value for max buffer length, used by CRemoveTags
 * 
 * On error/Errors:		If the client index passed for author is invalid or not in game.
 */
stock void CReplaceColorCodes(char[] buffer, int author = 0, bool removeTags = false, int maxlen = MAX_BUFFER_LENGTH) {
	CCheckTrie();
	if (removeTags) {
		ReplaceString(buffer, maxlen, "{default}", "", false);
		ReplaceString(buffer, maxlen, "{teamcolor}", "", false);
	}

	if (author != 0 && !removeTags) {
		if (author < 0 || author > MaxClients) {
			ThrowError("Invalid client index %i", author);
		}

		if (!IsClientInGame(author)) {
			ThrowError("Client %i is not in game", author);
		}

		ReplaceString(buffer, maxlen, "{teamcolor}", "\x03", false);
	}

	int cursor = 0;
	char value[32];
	char tag[32], buff[32]; 
	char[] output = new char[maxlen];

	strcopy(output, maxlen, buffer);
	// Since the string's size is going to be changing, output will hold the replaced string and we'll search buffer

	Regex regex = new Regex("{[#a-zA-Z0-9]+}");
	for (int i = 0; i < 1000; i++) { // The RegEx extension is quite flaky, so we have to loop here :/. This loop is supposed to be infinite and broken by return, but conditions have been added to be safe.
		if (regex.Match(buffer[cursor]) < 1) {
			delete regex;
			strcopy(buffer, maxlen, output);
			return;
		}

		regex.GetSubString(0, tag, sizeof(tag));
		CStrToLower(tag);
		cursor = StrContains(buffer[cursor], tag, false) + cursor + 1;
		strcopy(buff, sizeof(buff), tag);
		ReplaceString(buff, sizeof(buff), "{", "");
		ReplaceString(buff, sizeof(buff), "}", "");

		if (buff[0] == '#') {
			if (strlen(buff) == 7) {
				Format(buff, sizeof(buff), "\x07%s", buff[1]);
			}
			else if (strlen(buff) == 9) {
				Format(buff, sizeof(buff), "\x08%s", buff[1]);
			}
			else {
				continue;
			}

			if (removeTags) {
				ReplaceString(output, maxlen, tag, "", false);
			}
			else {
				ReplaceString(output, maxlen, tag, buff, false);
			}
		}
		else if (!g_smTrie.GetString(buff, value, sizeof(value))) {
			continue;
		}

		if (removeTags) {
			ReplaceString(output, maxlen, tag, "", false);
		}
		else {
			ReplaceString(output, maxlen, tag, value, false);
		}
	}
	LogError("[MULTICOLORS] Infinite loop broken.");
}

/**
 * Gets a part of a string
 * 
 * @param input			String to get the part from
 * @param output		Buffer to write to
 * @param maxlen		Max length of output buffer
 * @param start			Position to start at
 * @param numChars		Number of characters to return, or 0 for the end of the string
 */
stock void CSubString(const char[] input, char[] output, int maxlen, int start, int numChars = 0) {
	int i = 0;
	for (;;) {
		if (i == maxlen - 1 || i >= numChars || input[start + i] == '\0') {
			output[i] = '\0';
			return;
		}

		output[i] = input[start + i];
		i++;
	}
}

/**
 * Converts a string to lowercase
 * 
 * @param buffer		String to convert
 */
stock void CStrToLower(char[] buffer) {
	int len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		buffer[i] = CharToLower(buffer[i]);
	}
}

/**
 * Adds a color to the colors trie
 *
 * @param name			Color name, without braces
 * @param color			Hexadecimal representation of the color (0xRRGGBB)
 * @return				True if color was added successfully, false if a color already exists with that name
 */
stock bool CAddColor(const char[] name, int color) {
	CCheckTrie();

	char value[32];

	if (g_smTrie.GetString(name, value, sizeof(value))) {
		return false;
	}

	char newName[64];
	strcopy(newName, sizeof(newName), name);

	CStrToLower(newName);
	g_smTrie.SetValue(newName, color);
	return true;
}

/**
 * Removes color tags from a message
 * 
 * @param message		Message to remove tags from
 * @param maxlen		Maximum buffer length
 */
stock void CRemoveTags(char[] message, int maxlen) {
	CReplaceColorCodes(message, 0, true, maxlen);
}

/**
 * Replies to a command with colors
 * 
 * @param client		Client to reply to
 * @param message		Message (formatting rules)
 */
stock void CReplyToCommand(int client, const char[] message, any ...) {
	char buffer[MAX_BUFFER_LENGTH];
	SetGlobalTransTarget(client);
	VFormat(buffer, sizeof(buffer), message, 3);

	if (client == 0) {
		CRemoveTags(buffer, sizeof(buffer));
		PrintToServer("%s", buffer);
	}
	else if (GetCmdReplySource() == SM_REPLY_TO_CONSOLE) {
		CRemoveTags(buffer, sizeof(buffer));
		PrintToConsole(client, "%s", buffer);
	}
	else {
		CPrintToChat(client, "%s", buffer);
	}
}

/**
 * Replies to a command with colors
 * 
 * @param client		Client to reply to
 * @param author		Client to use for {teamcolor}
 * @param message		Message (formatting rules)
 */
stock void CReplyToCommandEx(int client, int author, const char[] message, any ...) {
	char buffer[MAX_BUFFER_LENGTH];
	SetGlobalTransTarget(client);
	VFormat(buffer, sizeof(buffer), message, 4);

	if (client == 0) {
		CRemoveTags(buffer, sizeof(buffer));
		PrintToServer("%s", buffer);
	}
	else if (GetCmdReplySource() == SM_REPLY_TO_CONSOLE) {
		CRemoveTags(buffer, sizeof(buffer));
		PrintToConsole(client, "%s", buffer);
	}
	else {
		CPrintToChatEx(client, author, "%s", buffer);
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
 * @error
 */
stock int CShowActivity(int client, const char[] format, any ...) {
	if (g_cShowActivity == null) {
		g_cShowActivity = FindConVar("sm_show_activity");
	}

	char tag[] = "[SM] ";

	char szBuffer[MAX_MESSAGE_LENGTH];
	//char szCMessage[MAX_MESSAGE_LENGTH];
	int value = g_cShowActivity.IntValue;
	ReplySource replyto = GetCmdReplySource();

	char name[MAX_NAME_LENGTH] = "Console";
	char sign[MAX_NAME_LENGTH] = "ADMIN";
	bool display_in_chat = false;
	if (client != 0) {
		if (client < 0 || client > MaxClients || !IsClientConnected(client))
			ThrowError("Client index %d is invalid", client);

		GetClientName(client, name, sizeof(name));
		AdminId id = GetUserAdmin(client);
		if (id == INVALID_ADMIN_ID || !id.HasFlag(Admin_Generic, Access_Effective)) {
			sign = "PLAYER";
		}

		/* Display the message to the client? */
		if (replyto == SM_REPLY_TO_CONSOLE) {
			SetGlobalTransTarget(client);
			VFormat(szBuffer, sizeof(szBuffer), format, 3);

			CRemoveTags(szBuffer, sizeof(szBuffer));
			PrintToConsole(client, "%s%s", tag, szBuffer);
			display_in_chat = true;
		}
	}
	else {
		SetGlobalTransTarget(LANG_SERVER);
		VFormat(szBuffer, sizeof(szBuffer), format, 3);

		CRemoveTags(szBuffer, sizeof(szBuffer));
		PrintToServer("%s%s", tag, szBuffer);
	}

	if (!value) {
		return 1;
	}

	for (int i = 1; i <= MaxClients; ++i) {
		if (!IsClientInGame(i) || IsFakeClient(i) || (display_in_chat && i == client)) {
			continue;
		}

		AdminId id = GetUserAdmin(i);
		SetGlobalTransTarget(i);
		if (id == INVALID_ADMIN_ID || !id.HasFlag(Admin_Generic, Access_Effective)) {
			/* Treat this as a normal user. */
			if ((value & 1) | (value & 2)) {
				char newsign[MAX_NAME_LENGTH];

				if ((value & 2) || (i == client)) {
					newsign = name;
				}
				else {
					newsign = sign;
				}

				VFormat(szBuffer, sizeof(szBuffer), format, 3);

				CPrintToChatEx(i, client, "%s%s: %s", tag, newsign, szBuffer);
			}
		}
		else {
			/* Treat this as an admin user */
			bool is_root = id.HasFlag(Admin_Root, Access_Effective);
			if ((value & 4) || (value & 8) || ((value & 16) && is_root)) {
				char newsign[MAX_NAME_LENGTH];

				if ((value & 8) || ((value & 16) && is_root) || (i == client)) {
					newsign = name;
				}
				else {
					newsign = sign;
				}

				VFormat(szBuffer, sizeof(szBuffer), format, 3);

				CPrintToChatEx(i, client, "%s%s: %s", tag, newsign, szBuffer);
			}
		}
	}

	return 1;
}

/**
 * Same as CShowActivity(), except the tag parameter is used instead of "[SM] " (note that you must supply any spacing).
 * Supports color tags.
 *
 * @param client		Client index doing the action, or 0 for server.
 * @param tags			Tag to display with.
 * @param format		Formatting rules.
 * @param ...			Variable number of format parameters.
 * @error
 */
stock int CShowActivityEx(int client, const char[] tag, const char[] format, any ...) {
	if (g_cShowActivity == null) {
		g_cShowActivity = FindConVar("sm_show_activity");
	}

	char szTag[MAX_MESSAGE_LENGTH];
	strcopy(szTag, sizeof(szTag), tag);
	CRemoveTags(szTag, sizeof(szTag));

	char szBuffer[MAX_MESSAGE_LENGTH];
	//char szCMessage[MAX_MESSAGE_LENGTH];
	int value = g_cShowActivity.IntValue;
	ReplySource replyto = GetCmdReplySource();

	char name[MAX_NAME_LENGTH] = "Console";
	char sign[MAX_NAME_LENGTH] = "ADMIN";
	bool display_in_chat = false;
	if (client != 0) {
		if (client < 0 || client > MaxClients || !IsClientConnected(client)) {
			ThrowError("Client index %d is invalid", client);
		}

		GetClientName(client, name, sizeof(name));
		AdminId id = GetUserAdmin(client);
		if (id == INVALID_ADMIN_ID || !id.HasFlag(Admin_Generic, Access_Effective)) {
			sign = "PLAYER";
		}

		/* Display the message to the client? */
		if (replyto == SM_REPLY_TO_CONSOLE) {
			SetGlobalTransTarget(client);
			VFormat(szBuffer, sizeof(szBuffer), format, 4);

			CRemoveTags(szBuffer, sizeof(szBuffer));
			PrintToConsole(client, "%s%s", szTag, szBuffer);
			display_in_chat = true;
		}
	}
	else {
		SetGlobalTransTarget(LANG_SERVER);
		VFormat(szBuffer, sizeof(szBuffer), format, 4);

		CRemoveTags(szBuffer, sizeof(szBuffer));
		PrintToServer("%s%s", szTag, szBuffer);
	}

	if (!value) {
		return 1;
	}

	for (int i = 1; i <= MaxClients; ++i) {
		if (!IsClientInGame(i) || IsFakeClient(i) || (display_in_chat && i == client)) {
			continue;
		}

		AdminId id = GetUserAdmin(i);
		SetGlobalTransTarget(i);
		if (id == INVALID_ADMIN_ID || !id.HasFlag(Admin_Generic, Access_Effective)) {
			/* Treat this as a normal user. */
			if ((value & 1) | (value & 2)) {
				char newsign[MAX_NAME_LENGTH];

				if ((value & 2) || (i == client)) {
					newsign = name;
				}
				else {
					newsign = sign;
				}

				VFormat(szBuffer, sizeof(szBuffer), format, 4);

				CPrintToChatEx(i, client, "%s%s: %s", tag, newsign, szBuffer);
			}
		}
		else {
			/* Treat this as an admin user */
			bool is_root = id.HasFlag(Admin_Root, Access_Effective);
			if ((value & 4) || (value & 8) || ((value & 16) && is_root)) {
				char newsign[MAX_NAME_LENGTH];

				if ((value & 8) || ((value & 16) && is_root) || (i == client)) {
					newsign = name;
				}
				else {
					newsign = sign;
				}

				VFormat(szBuffer, sizeof(szBuffer), format, 4);

				CPrintToChatEx(i, client, "%s%s: %s", tag, newsign, szBuffer);
			}
		}
	}

	return 1;
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
 * @error
 */
stock int CShowActivity2(int client, const char[] tag, const char[] format, any ...) {
	if (g_cShowActivity == null) {
		g_cShowActivity = FindConVar("sm_show_activity");
	}

	char szTag[MAX_MESSAGE_LENGTH];
	strcopy(szTag, sizeof(szTag), tag);
	CRemoveTags(szTag, sizeof(szTag));

	char szBuffer[MAX_MESSAGE_LENGTH];
	//char szCMessage[MAX_MESSAGE_LENGTH];
	int value = g_cShowActivity.IntValue;
	// ReplySource replyto = GetCmdReplySource();

	char name[MAX_NAME_LENGTH] = "Console";
	char sign[MAX_NAME_LENGTH] = "ADMIN";

	if (client != 0) {
		if (client < 0 || client > MaxClients || !IsClientConnected(client)) {
			ThrowError("Client index %d is invalid", client);
		}

		GetClientName(client, name, sizeof(name));

		AdminId id = GetUserAdmin(client);
		if (id == INVALID_ADMIN_ID || !id.HasFlag(Admin_Generic, Access_Effective)) {
			sign = "PLAYER";
		}

		SetGlobalTransTarget(client);
		VFormat(szBuffer, sizeof(szBuffer), format, 4);

		/* We don't display directly to the console because the chat text 
		 * simply gets added to the console, so we don't want it to print 
		 * twice.
		 */
		CPrintToChatEx(client, client, "%s%s", szTag, szBuffer);
	}
	else {
		SetGlobalTransTarget(LANG_SERVER);
		VFormat(szBuffer, sizeof(szBuffer), format, 4);

		CRemoveTags(szBuffer, sizeof(szBuffer));
		PrintToServer("%s%s", szTag, szBuffer);
	}

	if (!value) {
		return 1;
	}

	for (int i = 1; i <= MaxClients; ++i) {
		if (!IsClientInGame(i) || IsFakeClient(i) || i == client) {
			continue;
		}

		AdminId id = GetUserAdmin(i);
		SetGlobalTransTarget(i);
		if (id == INVALID_ADMIN_ID || !id.HasFlag(Admin_Generic, Access_Effective)) {
			/* Treat this as a normal user. */
			if ((value & 1) | (value & 2)) {
				char newsign[MAX_NAME_LENGTH];

				if ((value & 2)) {
					newsign = name;
				}
				else {
					newsign = sign;
				}

				VFormat(szBuffer, sizeof(szBuffer), format, 4);

				CPrintToChatEx(i, client, "%s%s: %s", tag, newsign, szBuffer);
			}
		}
		else {
			/* Treat this as an admin user */
			bool is_root = id.HasFlag(Admin_Root, Access_Effective);
			if ((value & 4) || (value & 8) || ((value & 16) && is_root)) {
				char newsign[MAX_NAME_LENGTH];
				

				if ((value & 8) || ((value & 16) && is_root)) {
					newsign = name;
				}
				else {
					newsign = sign;
				}

				VFormat(szBuffer, sizeof(szBuffer), format, 4);

				CPrintToChatEx(i, client, "%s%s: %s", tag, newsign, szBuffer);
			}
		}
	}

	return 1;
}

/**
 * Determines whether a color name exists
 * 
 * @param color			The color name to check
 * @return				True if the color exists, false otherwise
 */
stock bool CColorExists(const char[] color) {
	CCheckTrie();
	char temp[32];
	return g_smTrie.GetString(color, temp, sizeof(temp));
}

/**
 * Returns the hexadecimal representation of a client's team color (will NOT initialize the trie)
 *
 * @param client		Client to get the team color for
 * @param value			Client's team color, or green if unknown
 * @param size			Size of the value parameter
 * @return				Client's team color has been found
 * On error/Errors:		If the client index passed is invalid or not in game.
 */
stock bool CGetTeamColor(int client, char[] value, int size) {
	if (client <= 0 || client > MaxClients) {
		ThrowError("Invalid client index %i", client);
		return false;
	}

	if (!IsClientInGame(client)) {
		ThrowError("Client %i is not in game", client);
		return false;
	}

	StringMap smTrie = CGetTrie();
	if (smTrie == null)
	{
		ThrowError("No color in StringMap tree", client);
		return false;
	}

	switch(GetClientTeam(client)) {
		case 1: {
			smTrie.GetString("gray", value, size);
		}
		case 2: {
			smTrie.GetString("red", value, size);
		}
		case 3: {
			smTrie.GetString("blue", value, size);
		}
		default: {
			smTrie.GetString("green", value, size);
		}
	}

	return true;
}

stock StringMap CInitColorTrie() {
	StringMap hTrie = new StringMap();

	if (IsSource2009()) {
		SetTrieString(hTrie, "default", "\x01");
		SetTrieString(hTrie, "teamcolor", "\x03");

		SetTrieString(hTrie, "aliceblue", "\x07F0F8FF");
		SetTrieString(hTrie, "allies", "\x074D7942"); // same as Allies team in DoD:S
		SetTrieString(hTrie, "ancient", "\x07EB4B4B"); // same as Ancient item rarity in Dota 2
		SetTrieString(hTrie, "antiquewhite", "\x07FAEBD7");
		SetTrieString(hTrie, "aqua", "\x0700FFFF");
		SetTrieString(hTrie, "aquamarine", "\x077FFFD4");
		SetTrieString(hTrie, "arcana", "\x07ADE55C"); // same as Arcana item rarity in Dota 2
		SetTrieString(hTrie, "axis", "\x07FF4040"); // same as Axis team in DoD:S
		SetTrieString(hTrie, "azure", "\x07007FFF");
		SetTrieString(hTrie, "beige", "\x07F5F5DC");
		SetTrieString(hTrie, "bisque", "\x07FFE4C4");
		SetTrieString(hTrie, "black", "\x07000000");
		SetTrieString(hTrie, "blanchedalmond", "\x07FFEBCD");
		SetTrieString(hTrie, "blue", "\x0799CCFF"); // same as BLU/Counter-Terrorist team color
		SetTrieString(hTrie, "blueviolet", "\x078A2BE2");
		SetTrieString(hTrie, "brown", "\x07A52A2A");
		SetTrieString(hTrie, "burlywood", "\x07DEB887");
		SetTrieString(hTrie, "cadetblue", "\x075F9EA0");
		SetTrieString(hTrie, "chartreuse", "\x077FFF00");
		SetTrieString(hTrie, "chocolate", "\x07D2691E");
		SetTrieString(hTrie, "collectors", "\x07AA0000"); // same as Collector's item quality in TF2
		SetTrieString(hTrie, "common", "\x07B0C3D9"); // same as Common item rarity in Dota 2
		SetTrieString(hTrie, "community", "\x0770B04A"); // same as Community item quality in TF2
		SetTrieString(hTrie, "coral", "\x07FF7F50");
		SetTrieString(hTrie, "cornflowerblue", "\x076495ED");
		SetTrieString(hTrie, "cornsilk", "\x07FFF8DC");
		SetTrieString(hTrie, "corrupted", "\x07A32C2E"); // same as Corrupted item quality in Dota 2
		SetTrieString(hTrie, "crimson", "\x07DC143C");
		SetTrieString(hTrie, "cyan", "\x0700FFFF");
		SetTrieString(hTrie, "darkblue", "\x0700008B");
		SetTrieString(hTrie, "darkcyan", "\x07008B8B");
		SetTrieString(hTrie, "darkgoldenrod", "\x07B8860B");
		SetTrieString(hTrie, "darkgray", "\x07A9A9A9");
		SetTrieString(hTrie, "darkgrey", "\x07A9A9A9");
		SetTrieString(hTrie, "darkgreen", "\x07006400");
		SetTrieString(hTrie, "darkkhaki", "\x07BDB76B");
		SetTrieString(hTrie, "darkmagenta", "\x078B008B");
		SetTrieString(hTrie, "darkolivegreen", "\x07556B2F");
		SetTrieString(hTrie, "darkorange", "\x07FF8C00");
		SetTrieString(hTrie, "darkorchid", "\x079932CC");
		SetTrieString(hTrie, "darkred", "\x078B0000");
		SetTrieString(hTrie, "darksalmon", "\x07E9967A");
		SetTrieString(hTrie, "darkseagreen", "\x078FBC8F");
		SetTrieString(hTrie, "darkslateblue", "\x07483D8B");
		SetTrieString(hTrie, "darkslategray", "\x072F4F4F");
		SetTrieString(hTrie, "darkslategrey", "\x072F4F4F");
		SetTrieString(hTrie, "darkturquoise", "\x0700CED1");
		SetTrieString(hTrie, "darkviolet", "\x079400D3");
		SetTrieString(hTrie, "deeppink", "\x07FF1493");
		SetTrieString(hTrie, "deepskyblue", "\x0700BFFF");
		SetTrieString(hTrie, "dimgray", "\x07696969");
		SetTrieString(hTrie, "dimgrey", "\x07696969");
		SetTrieString(hTrie, "dodgerblue", "\x071E90FF");
		SetTrieString(hTrie, "exalted", "\x07CCCCCD"); // same as Exalted item quality in Dota 2
		SetTrieString(hTrie, "firebrick", "\x07B22222");
		SetTrieString(hTrie, "floralwhite", "\x07FFFAF0");
		SetTrieString(hTrie, "forestgreen", "\x07228B22");
		SetTrieString(hTrie, "frozen", "\x074983B3"); // same as Frozen item quality in Dota 2
		SetTrieString(hTrie, "fuchsia", "\x07FF00FF");
		SetTrieString(hTrie, "fullblue", "\x070000FF");
		SetTrieString(hTrie, "fullred", "\x07FF0000");
		SetTrieString(hTrie, "gainsboro", "\x07DCDCDC");
		SetTrieString(hTrie, "genuine", "\x074D7455"); // same as Genuine item quality in TF2
		SetTrieString(hTrie, "ghostwhite", "\x07F8F8FF");
		SetTrieString(hTrie, "gold", "\x07FFD700");
		SetTrieString(hTrie, "goldenrod", "\x07DAA520");
		SetTrieString(hTrie, "gray", "\x07CCCCCC"); // same as spectator team color
		SetTrieString(hTrie, "grey", "\x07CCCCCC");
		SetTrieString(hTrie, "green", "\x073EFF3E");
		SetTrieString(hTrie, "greenyellow", "\x07ADFF2F");
		SetTrieString(hTrie, "haunted", "\x0738F3AB"); // same as Haunted item quality in TF2
		SetTrieString(hTrie, "honeydew", "\x07F0FFF0");
		SetTrieString(hTrie, "hotpink", "\x07FF69B4");
		SetTrieString(hTrie, "immortal", "\x07E4AE33"); // same as Immortal item rarity in Dota 2
		SetTrieString(hTrie, "indianred", "\x07CD5C5C");
		SetTrieString(hTrie, "indigo", "\x074B0082");
		SetTrieString(hTrie, "ivory", "\x07FFFFF0");
		SetTrieString(hTrie, "khaki", "\x07F0E68C");
		SetTrieString(hTrie, "lavender", "\x07E6E6FA");
		SetTrieString(hTrie, "lavenderblush", "\x07FFF0F5");
		SetTrieString(hTrie, "lawngreen", "\x077CFC00");
		SetTrieString(hTrie, "legendary", "\x07D32CE6"); // same as Legendary item rarity in Dota 2
		SetTrieString(hTrie, "lemonchiffon", "\x07FFFACD");
		SetTrieString(hTrie, "lightblue", "\x07ADD8E6");
		SetTrieString(hTrie, "lightcoral", "\x07F08080");
		SetTrieString(hTrie, "lightcyan", "\x07E0FFFF");
		SetTrieString(hTrie, "lightgoldenrodyellow", "\x07FAFAD2");
		SetTrieString(hTrie, "lightgray", "\x07D3D3D3");
		SetTrieString(hTrie, "lightgrey", "\x07D3D3D3");
		SetTrieString(hTrie, "lightgreen", "\x0799FF99");
		SetTrieString(hTrie, "lightpink", "\x07FFB6C1");
		SetTrieString(hTrie, "lightsalmon", "\x07FFA07A");
		SetTrieString(hTrie, "lightseagreen", "\x0720B2AA");
		SetTrieString(hTrie, "lightskyblue", "\x0787CEFA");
		SetTrieString(hTrie, "lightslategray", "\x07778899");
		SetTrieString(hTrie, "lightslategrey", "\x07778899");
		SetTrieString(hTrie, "lightsteelblue", "\x07B0C4DE");
		SetTrieString(hTrie, "lightyellow", "\x07FFFFE0");
		SetTrieString(hTrie, "lime", "\x0700FF00");
		SetTrieString(hTrie, "limegreen", "\x0732CD32");
		SetTrieString(hTrie, "linen", "\x07FAF0E6");
		SetTrieString(hTrie, "magenta", "\x07FF00FF");
		SetTrieString(hTrie, "maroon", "\x07800000");
		SetTrieString(hTrie, "mediumaquamarine", "\x0766CDAA");
		SetTrieString(hTrie, "mediumblue", "\x070000CD");
		SetTrieString(hTrie, "mediumorchid", "\x07BA55D3");
		SetTrieString(hTrie, "mediumpurple", "\x079370D8");
		SetTrieString(hTrie, "mediumseagreen", "\x073CB371");
		SetTrieString(hTrie, "mediumslateblue", "\x077B68EE");
		SetTrieString(hTrie, "mediumspringgreen", "\x0700FA9A");
		SetTrieString(hTrie, "mediumturquoise", "\x0748D1CC");
		SetTrieString(hTrie, "mediumvioletred", "\x07C71585");
		SetTrieString(hTrie, "midnightblue", "\x07191970");
		SetTrieString(hTrie, "mintcream", "\x07F5FFFA");
		SetTrieString(hTrie, "mistyrose", "\x07FFE4E1");
		SetTrieString(hTrie, "moccasin", "\x07FFE4B5");
		SetTrieString(hTrie, "mythical", "\x078847FF"); // same as Mythical item rarity in Dota 2
		SetTrieString(hTrie, "navajowhite", "\x07FFDEAD");
		SetTrieString(hTrie, "navy", "\x07000080");
		SetTrieString(hTrie, "normal", "\x07B2B2B2"); // same as Normal item quality in TF2
		SetTrieString(hTrie, "oldlace", "\x07FDF5E6");
		SetTrieString(hTrie, "olive", "\x079EC34F");
		SetTrieString(hTrie, "olivedrab", "\x076B8E23");
		SetTrieString(hTrie, "orange", "\x07FFA500");
		SetTrieString(hTrie, "orangered", "\x07FF4500");
		SetTrieString(hTrie, "orchid", "\x07DA70D6");
		SetTrieString(hTrie, "palegoldenrod", "\x07EEE8AA");
		SetTrieString(hTrie, "palegreen", "\x0798FB98");
		SetTrieString(hTrie, "paleturquoise", "\x07AFEEEE");
		SetTrieString(hTrie, "palevioletred", "\x07D87093");
		SetTrieString(hTrie, "papayawhip", "\x07FFEFD5");
		SetTrieString(hTrie, "peachpuff", "\x07FFDAB9");
		SetTrieString(hTrie, "peru", "\x07CD853F");
		SetTrieString(hTrie, "pink", "\x07FFC0CB");
		SetTrieString(hTrie, "plum", "\x07DDA0DD");
		SetTrieString(hTrie, "powderblue", "\x07B0E0E6");
		SetTrieString(hTrie, "purple", "\x07800080");
		SetTrieString(hTrie, "rare", "\x074B69FF"); // same as Rare item rarity in Dota 2
		SetTrieString(hTrie, "red", "\x07FF4040"); // same as RED/Terrorist team color
		SetTrieString(hTrie, "rosybrown", "\x07BC8F8F");
		SetTrieString(hTrie, "royalblue", "\x074169E1");
		SetTrieString(hTrie, "saddlebrown", "\x078B4513");
		SetTrieString(hTrie, "salmon", "\x07FA8072");
		SetTrieString(hTrie, "sandybrown", "\x07F4A460");
		SetTrieString(hTrie, "seagreen", "\x072E8B57");
		SetTrieString(hTrie, "seashell", "\x07FFF5EE");
		SetTrieString(hTrie, "selfmade", "\x0770B04A"); // same as Self-Made item quality in TF2
		SetTrieString(hTrie, "sienna", "\x07A0522D");
		SetTrieString(hTrie, "silver", "\x07C0C0C0");
		SetTrieString(hTrie, "skyblue", "\x0787CEEB");
		SetTrieString(hTrie, "slateblue", "\x076A5ACD");
		SetTrieString(hTrie, "slategray", "\x07708090");
		SetTrieString(hTrie, "slategrey", "\x07708090");
		SetTrieString(hTrie, "snow", "\x07FFFAFA");
		SetTrieString(hTrie, "springgreen", "\x0700FF7F");
		SetTrieString(hTrie, "steelblue", "\x074682B4");
		SetTrieString(hTrie, "strange", "\x07CF6A32"); // same as Strange item quality in TF2
		SetTrieString(hTrie, "tan", "\x07D2B48C");
		SetTrieString(hTrie, "teal", "\x07008080");
		SetTrieString(hTrie, "thistle", "\x07D8BFD8");
		SetTrieString(hTrie, "tomato", "\x07FF6347");
		SetTrieString(hTrie, "turquoise", "\x0740E0D0");
		SetTrieString(hTrie, "uncommon", "\x07B0C3D9"); // same as Uncommon item rarity in Dota 2
		SetTrieString(hTrie, "unique", "\x07FFD700"); // same as Unique item quality in TF2
		SetTrieString(hTrie, "unusual", "\x078650AC"); // same as Unusual item quality in TF2
		SetTrieString(hTrie, "valve", "\x07A50F79"); // same as Valve item quality in TF2
		SetTrieString(hTrie, "vintage", "\x07476291"); // same as Vintage item quality in TF2
		SetTrieString(hTrie, "violet", "\x07EE82EE");
		SetTrieString(hTrie, "wheat", "\x07F5DEB3");
		SetTrieString(hTrie, "white", "\x07FFFFFF");
		SetTrieString(hTrie, "whitesmoke", "\x07F5F5F5");
		SetTrieString(hTrie, "yellow", "\x07FFFF00");
		SetTrieString(hTrie, "yellowgreen", "\x079ACD32");
	} else {
		SetTrieString(hTrie, "default", "\x01");       // "\x01" "{default}"
		SetTrieString(hTrie, "teamcolor", "\x03");     // "\x03" "{lightgreen}" "\x03" "{orange}" "\x03" "{blue}" "\x03" "{purple}"

		SetTrieString(hTrie, "red", "\x07");           // "\x07" "{red}"
		SetTrieString(hTrie, "lightred", "\x0F");      // "\x0F" "{lightred}"
		SetTrieString(hTrie, "darkred", "\x02");       // "\x02" "{darkred}"
		SetTrieString(hTrie, "bluegrey", "\x0A");      // "\x0A" "{lightblue}"
		SetTrieString(hTrie, "blue", "\x0B");          // "\x0B" "{steelblue}"
		SetTrieString(hTrie, "darkblue", "\x0C");      // "\x0C" "{darkblue}"
		SetTrieString(hTrie, "purple", "\x03");
		SetTrieString(hTrie, "orchid", "\x0E");        // "\x0E" "{pink}"
		SetTrieString(hTrie, "yellow", "\x09");        // "\x09" "{yellow}"
		SetTrieString(hTrie, "gold", "\x10");
		SetTrieString(hTrie, "lightgreen", "\x05");    // "\x05" "{olive}"
		SetTrieString(hTrie, "green", "\x04");         // "\x04" "{green}"
		SetTrieString(hTrie, "lime", "\x06");          // "\x06" "{lime}"
		SetTrieString(hTrie, "grey", "\x08");          // "\x08" "{grey}"
		SetTrieString(hTrie, "grey2", "\x0D");
	}

	// SetTrieString(hTrie, "engine 1", "\x01");
	// SetTrieString(hTrie, "engine 2", "\x02");
	// SetTrieString(hTrie, "engine 3", "\x03");
	// SetTrieString(hTrie, "engine 4", "\x04");
	// SetTrieString(hTrie, "engine 5", "\x05");
	// SetTrieString(hTrie, "engine 6", "\x06");
	// SetTrieString(hTrie, "engine 7", "\x07");
	// SetTrieString(hTrie, "engine 8", "\x08");
	// SetTrieString(hTrie, "engine 9", "\x09");
	// SetTrieString(hTrie, "engine 10", "\x0A");
	// SetTrieString(hTrie, "engine 11", "\x0B");
	// SetTrieString(hTrie, "engine 12", "\x0C");
	// SetTrieString(hTrie, "engine 13", "\x0D");
	// SetTrieString(hTrie, "engine 14", "\x0E");
	// SetTrieString(hTrie, "engine 15", "\x0F");
	// SetTrieString(hTrie, "engine 16", "\x10");

	return hTrie;
}

/*
*
*   Below are the original sourcemod function names with "C" prefixed for colors
*
*/

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
	VFormat(g_sPrefix, sizeof(g_sPrefix) - strlen(PREFIX_SEPARATOR), sPrefix, 2);

	// Add ending space
	Format(g_sPrefix, sizeof(g_sPrefix), "%s%s", g_sPrefix, PREFIX_SEPARATOR);

	// Set colors
	CReplaceColorCodes(g_sPrefix);
}

/**
 * Add a chat prefix before all chat msg
 *
 * @param sPrefix		Prefix
 */
stock void CClearPrefix() {
	g_sPrefix[0] = '\0';
}

/**
 * Writes a message to all of a client's observers.
 *
 * @param target 	Client index.
 * @param message	Message (formatting rules).
 */
stock void CPrintToChatObservers(int target, const char[] message, any ...) {
	char buffer[MAX_MESSAGE_LENGTH];
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
 * Writes a message to all of a client's observers with the correct
 * game stock.
 *
 * @param target 	Client index.
 * @param message	Message (formatting rules).
 */
stock void CPrintToChatObserversEx(int target, const char[] message, any ...) {
	char buffer[MAX_MESSAGE_LENGTH];
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
 * Remove all tags and print to server
 * 
 * @param message		Message (formatting rules)
 */
stock void CPrintToServer(const char[] message, any ...) {
	char buffer[MAX_MESSAGE_LENGTH];
	char prefixBuffer[PREFIX_MAX_LENGTH];
	SetGlobalTransTarget(LANG_SERVER);
	VFormat(buffer, sizeof(buffer), message, 2);
	strcopy(prefixBuffer, sizeof(prefixBuffer), g_sPrefix);

	CRemoveTags(buffer, sizeof(buffer));
	CRemoveTags(prefixBuffer, sizeof(prefixBuffer));

	PrintToServer("%s%s", prefixBuffer, buffer);
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

	CReplaceColorCodes(message, author, false, maxlength);
}

stock void CAddPrefix(char[] buffer, int size)
{
	Format(buffer, size, "%s%s", g_sPrefix, buffer);
}

stock bool IsSource2009() {
	if (g_evEngineVersion == Engine_Unknown)
	{
		g_evEngineVersion = GetEngineVersion();
	}

	return (g_evEngineVersion == Engine_CSS
	|| g_evEngineVersion == Engine_HL2DM
	|| g_evEngineVersion == Engine_DODS
	|| g_evEngineVersion == Engine_TF2
	|| g_evEngineVersion == Engine_Insurgency
	|| g_evEngineVersion == Engine_SDK2013);
}
