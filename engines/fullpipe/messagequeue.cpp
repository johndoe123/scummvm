/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "fullpipe/fullpipe.h"

#include "fullpipe/objects.h"
#include "fullpipe/messagequeue.h"

namespace Fullpipe {

MessageQueue::MessageQueue() {
	_field_14 = 0;
	_parId = 0;
	_dataId = 0;
	_id = 0;
	_isFinished = 0;
	_flags = 0;
}

bool MessageQueue::load(MfcArchive &file) {
	debug(5, "MessageQueue::load()");

	_dataId = file.readUint16LE();

	int count = file.readUint16LE();

	assert(g_fullpipe->_gameProjectVersion >= 12);

	_queueName = file.readPascalString();

	for (int i = 0; i < count; i++) {
		CObject *tmp = file.readClass();

		_exCommands.push_back(tmp);
	}

	_id = -1;
	_field_14 = 0;
	_parId = 0;
	_isFinished = 0;

	return true;
}

MessageQueue *GlobalMessageQueueList::getMessageQueueById(int id) {
	for (CPtrList::iterator s = begin(); s != end(); ++s) {
		if (((MessageQueue *)s)->_id == id)
			return (MessageQueue *)s;
	}

	return 0;
}

void GlobalMessageQueueList::deleteQueueById(int id) {
	for (uint i = 0; i < size(); i++)
		if (((MessageQueue *)((*this).operator[](i)))->_id == id) {
			delete (MessageQueue *)remove_at(i);

			disableQueueById(id);
			return;
		}
}

void GlobalMessageQueueList::disableQueueById(int id) {
	for (CPtrList::iterator s = begin(); s != end(); ++s) {
		if (((MessageQueue *)s)->_parId == id)
			((MessageQueue *)s)->_parId = 0;
	}
}

bool removeMessageHandler(int16 id, int pos) {
	if (g_fullpipe->_messageHandlers) {
		MessageHandler *curItem = g_fullpipe->_messageHandlers;
		MessageHandler *prevItem = 0;
		int curPos = 0;

		while (id != curItem->id) {
			prevItem = curItem;
			curItem = curItem->nextItem;
			curPos++;

			if (!curItem)
				return false;
		}

		if (pos == -1 || curPos == pos) {
			prevItem->nextItem = curItem->nextItem;
			delete curItem;
			updateMessageHandlerIndex(prevItem->nextItem, -1);

			return true;
		}
	}

	return false;
}

void updateMessageHandlerIndex(MessageHandler *msg, int offset) {
	for (; msg; msg = msg->nextItem)
		msg->index += offset;
}

} // End of namespace Fullpipe
