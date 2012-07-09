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

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#ifndef WINTERMUTE_BFILE_H
#define WINTERMUTE_BFILE_H


#include "engines/wintermute/Base/BBase.h"
#include "common/str.h"
#include "common/stream.h"

namespace Common {
class SeekableReadStream;
}

namespace WinterMute {

class CBFile : public CBBase {
protected:
	uint32 _pos;
	uint32 _size;
public:
	virtual uint32 getSize() {
		return _size;
	};
	virtual uint32 getPos() {
		return _pos;
	};
	virtual ERRORCODE seek(uint32 pos, TSeek origin = SEEK_TO_BEGIN) = 0;
	virtual ERRORCODE read(void *buffer, uint32 size) = 0;
	virtual ERRORCODE close() = 0;
	virtual ERRORCODE open(const Common::String &filename) = 0;
	virtual bool isEOF();
	CBFile(CBGame *inGame);
	virtual ~CBFile();
	// Temporary solution to allow usage in ScummVM-code:
	virtual Common::SeekableReadStream *getMemStream();
};

} // end of namespace WinterMute

#endif
