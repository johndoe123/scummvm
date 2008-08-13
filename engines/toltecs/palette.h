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
 *
 */

#ifndef TOLTECS_PALETTE_H
#define TOLTECS_PALETTE_H

#include "common/scummsys.h"
#include "common/endian.h"
#include "common/util.h"
#include "common/file.h"
#include "common/savefile.h"
#include "common/stream.h"
#include "common/system.h"
#include "common/hash-str.h"
#include "common/events.h"
#include "common/keyboard.h"
#include "common/array.h"

#include "sound/audiostream.h"
#include "sound/mixer.h"
#include "sound/voc.h"
#include "sound/audiocd.h"

#include "engines/engine.h"

namespace Toltecs {

//#define ROT(index) (((index << 4) & 0xF0) | ((index >> 4) & 0x0F))
//#define ROT(index) (index)

class Palette {
public:
	Palette(ToltecsEngine *vm);
	~Palette();

	void setFullPalette(byte *palette);
	void getFullPalette(byte *palette);
	void setDeltaPalette(byte *palette, byte mask, char deltaValue, int16 count, int16 startIndex);

	void loadAddPalette(uint resIndex, byte startIndex);
	void loadAddPaletteFrom(byte *source, byte startIndex, byte count);

	void addFragment(uint resIndex, int16 id);
	uint16 findFragment(int16 id);
	void clearFragments();

	void buildColorTransTable(byte limit, char deltaValue, byte mask);
	byte getColorTransPixel(byte pixel) const { return _colorTransTable[pixel]; }

	byte *getMainPalette() { return _mainPalette; }
	byte *getAnimPalette() { return _animPalette; }

	void saveState(Common::WriteStream *out);
	void loadState(Common::ReadStream *in);

protected:

	struct PaletteFragment {
		int16 id;
		byte index, count;
	};
	
	typedef Common::Array<PaletteFragment> PaletteFragmentArray;

	ToltecsEngine *_vm;

	byte _mainPalette[768];
	byte _animPalette[768];
	byte _colorTransTable[256];

	PaletteFragmentArray _fragments;
	byte _fragmentIndex;

};

} // End of namespace Toltecs

#endif /* TOLTECS_PALETTE_H */
