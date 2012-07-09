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

#ifndef WINTERMUTE_BSOUNDBUFFER_H
#define WINTERMUTE_BSOUNDBUFFER_H


#include "engines/wintermute/Base/BBase.h"
#include "common/stream.h"

namespace Audio {
class SeekableAudioStream;
class SoundHandle;
}

namespace WinterMute {

class CBFile;
class CBSoundBuffer : public CBBase {
public:

	CBSoundBuffer(CBGame *inGame);
	virtual ~CBSoundBuffer();


	ERRORCODE pause();
	ERRORCODE play(bool looping = false, uint32 startSample = 0);
	ERRORCODE resume();
	ERRORCODE stop();
	bool isPlaying();

	void setLooping(bool looping);

	uint32 getPosition();
	ERRORCODE setPosition(uint32 pos);
	uint32 getLength();

	ERRORCODE setLoopStart(uint32 pos);
	uint32 getLoopStart() const {
		return _loopStart;
	}

	ERRORCODE setPan(float pan);
	ERRORCODE setPrivateVolume(int colume);
	ERRORCODE setVolume(int colume);

	void setType(TSoundType Type);

	ERRORCODE loadFromFile(const char *filename, bool forceReload = false);
	void setStreaming(bool streamed, uint32 numBlocks = 0, uint32 blockSize = 0);
	ERRORCODE applyFX(TSFXType type, float param1, float param2, float param3, float param4);

	//HSTREAM _stream;
	//HSYNC _sync;
	Audio::SeekableAudioStream *_stream;
	Audio::SoundHandle *_handle;

	bool _freezePaused;
	uint32 _loopStart;
	TSoundType _type;
	bool _looping;
	Common::SeekableReadStream *_file;
	char *_filename;
	bool _streamed;
	int _privateVolume;

	/*static void CALLBACK LoopSyncProc(HSYNC handle, uint32 channel, uint32 data, void *user);

	static void CALLBACK FileCloseProc(void *user);
	static QWORD CALLBACK FileLenProc(void *user);
	static uint32 CALLBACK FileReadProc(void *buffer, uint32 length, void *user);
	static BOOL CALLBACK FileSeekProc(QWORD offset, void *user);*/
};

} // end of namespace WinterMute

#endif
