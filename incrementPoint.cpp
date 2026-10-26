/*
 * Copyright (C) 2026 Loymdayddaud
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "bzfsAPI.h"

class incrementPoint : public bz_Plugin, public bz_CustomSlashCommandHandler
{
	virtual const char* Name ();
	virtual void Init (const char*);
	virtual void Cleanup ();
	virtual bool SlashCommand (int playerID, bz_ApiString command, bz_ApiString /*message*/, bz_APIStringList *params);
};

BZ_PLUGIN(incrementPoint)

const char* incrementPoint::Name ()
{
	return "Increment Point Plugin";
}

void incrementPoint::Init (const char*)
{
	bz_registerCustomSlashCommand("incrementPoint", this);
}

void incrementPoint::Cleanup ()
{
	Flush();

	bz_removeCustomSlashCommand("incrementPoint");
}

bool incrementPoint::SlashCommand (int playerID, bz_ApiString command, bz_ApiString /*message*/, bz_APIStringList *params)
{
	if (command == "incrementPoint")
	{
		bz_BasePlayerRecord *fromPlayer = bz_getPlayerByIndex(playerID);

		if (!fromPlayer->admin)
		{
			bz_sendTextMessage(BZ_SERVER, playerID, "You do not have permission to run /incrementPoint.");
			bz_freePlayerRecord(fromPlayer);
			return true;
		}

		if (params->size() != 1)
		{
			bz_sendTextMessage(BZ_SERVER, playerID, "/incrementPoint <player ID|callsign>");
			bz_freePlayerRecord(fromPlayer);
			return true;
		}

		bz_BasePlayerRecord* receiverID;
		receiverID = bz_getPlayerBySlotOrCallsign(params->get(0).c_str());

		if (!receiverID)
		{
			bz_sendTextMessagef(BZ_SERVER, playerID, "player \"%s\" not found", receiverID);
		}
		else
		{
			bz_incrementPlayerLosses(receiverID->playerID, -1);
			bz_sendTextMessagef(BZ_SERVER, fromPlayer->playerID, "Decreased %s's losses by one.", bz_getPlayerCallsign(receiverID->playerID));
		}
		bz_freePlayerRecord(receiverID);
		bz_freePlayerRecord(fromPlayer);

		return true;
	}
	if (command == "decrementPoint")
	{
		bz_BasePlayerRecord *fromPlayer = bz_getPlayerByIndex(playerID);

		if (!fromPlayer->admin)
		{
			bz_sendTextMessage(BZ_SERVER, playerID, "You do not have permission to run /decrementPoint.");
			bz_freePlayerRecord(fromPlayer);
			return true;
		}

		if (params->size() != 1)
		{
			bz_sendTextMessage(BZ_SERVER, playerID, "/decrementPoint <player ID|callsign>");
			bz_freePlayerRecord(fromPlayer);
			return true;
		}

		bz_BasePlayerRecord* receiverID;
		receiverID = bz_getPlayerBySlotOrCallsign(params->get(0).c_str());

		if (!receiverID)
		{
			bz_sendTextMessagef(BZ_SERVER, playerID, "player \"%s\" not found", receiverID);
		}
		else
		{
			bz_incrementPlayerWins(receiverID->playerID, -1);
			bz_sendTextMessagef(BZ_SERVER, fromPlayer->playerID, "Decreased %s's wins by one.", bz_getPlayerCallsign(receiverID->playerID));
		}
		bz_freePlayerRecord(receiverID);
		bz_freePlayerRecord(fromPlayer);

		return true;
	}
	return false;
}
