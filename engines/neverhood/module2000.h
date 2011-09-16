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

#ifndef NEVERHOOD_MODULE2000_H
#define NEVERHOOD_MODULE2000_H

#include "neverhood/neverhood.h"
#include "neverhood/module.h"
#include "neverhood/scene.h"
#include "neverhood/module1200.h"

namespace Neverhood {

class Module2000 : public Module {
public:
	Module2000(NeverhoodEngine *vm, Module *parentModule, int which);
	virtual ~Module2000();
protected:
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
	void createScene2001(int which);
	void createScene2002(int which);
	void createScene2003(int which);
	void updateScene2001();			
	void updateScene2002();			
	void updateScene2003();			
};

// Scene2001

class Scene2001 : public Scene {
public:
	Scene2001(NeverhoodEngine *vm, Module *parentModule, int which);
protected:
	Sprite *_class401;
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
};

} // End of namespace Neverhood

#endif /* NEVERHOOD_MODULE2000_H */