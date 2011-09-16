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

#ifndef NEVERHOOD_SMACKERSCENE_H
#define NEVERHOOD_SMACKERSCENE_H

#include "neverhood/neverhood.h"
#include "neverhood/resourceman.h"
#include "neverhood/scene.h"

namespace Neverhood {

class SmackerScene : public Scene {
public:
	SmackerScene(NeverhoodEngine *vm, Module *parentModule, bool doubleSurface, bool flag1, bool canAbort);
	virtual ~SmackerScene();
	void setFileHash(uint32 fileHash);
	void setFileHashList(uint32 *fileHashList);
	void nextVideo();
protected:
	bool _doubleSurface;
	bool _flag1;
	bool _canAbort;
	bool _fieldDF;
	bool _playNextVideoFlag;
	int _fileHashListIndex;
	uint32 *_fileHashList;
	uint32 _fileHash[2];
	void update();
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
};

} // End of namespace Neverhood

#endif /* NEVERHOOD_SMACKERSCENE_H */