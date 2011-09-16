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

#ifndef NEVERHOOD_GAMEVARS_H
#define NEVERHOOD_GAMEVARS_H

#include "common/array.h"
#include "neverhood/neverhood.h"

namespace Neverhood {

struct GameVar {
	uint32 nameHash;
	uint32 value;
	int16 firstIndex, nextIndex;
};

class GameVars {
public:
	GameVars();
	~GameVars();
	// TODO void load(???);
	// TODO void save(???);
	uint32 getGlobalVar(uint32 nameHash);
	void setGlobalVar(uint32 nameHash, uint32 value);
	uint32 getSubVar(uint32 nameHash, uint32 subNameHash);
	void setSubVar(uint32 nameHash, uint32 subNameHash, uint32 value);
protected:
	Common::Array<GameVar> _vars;
	int16 addVar(uint32 nameHash, uint32 value);
	int16 findSubVarIndex(int16 varIndex, uint32 subNameHash);
	int16 addSubVar(int16 varIndex, uint32 subNameHash, uint32 value);
	int16 getSubVarIndex(int16 varIndex, uint32 subNameHash);
};

} // End of namespace Neverhood

#endif /* NEVERHOOD_GAMEVARS_H */