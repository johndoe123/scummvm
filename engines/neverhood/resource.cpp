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

#include "common/memstream.h"
#include "neverhood/resource.h"
#include "neverhood/resourceman.h"

namespace Neverhood {

// TODO: Since the load() methods are similar in most cases some of the code therein
// can probably be copied into another method (e.g. inside the resource manager)
// to reduce code. 

// SpriteResource

SpriteResource::SpriteResource(NeverhoodEngine *vm)
	 : _vm(vm), _resourceHandle(-1), _pixels(NULL) {
}

SpriteResource::~SpriteResource() {
	unload();
}

void SpriteResource::draw(byte *dest, int destPitch, bool flipX, bool flipY) {
	if (_pixels) {
		if (_rle) {
			unpackSpriteRle(_pixels, _dimensions.width, _dimensions.height, dest, destPitch, flipX, flipY);
		} else {
			unpackSpriteNormal(_pixels, _dimensions.width, _dimensions.height, dest, destPitch, flipX, flipY);
		}
	}
}

bool SpriteResource::load(uint32 fileHash) {
	debug(2, "SpriteResource::load(%08X)", fileHash);
	// TODO: Later merge with load2 and make the mode a parameter
	unload();
	_resourceHandle = _vm->_res->useResource(fileHash);
	debug(2, "SpriteResource::load(0x%08X) _resourceHandle = %d", fileHash, _resourceHandle);
	if (_resourceHandle != -1) {
		if (_vm->_res->getResourceType(_resourceHandle) == 2) {
			byte *spriteData = _vm->_res->loadResource(_resourceHandle, true);
			parseBitmapResource(spriteData, &_rle, &_dimensions, NULL, NULL, &_pixels);
		} else {
			_vm->_res->unuseResource(_resourceHandle);
			_resourceHandle = -1;
		}
	} 
	return _pixels != NULL;
}

bool SpriteResource::load2(uint32 fileHash) {
	unload();
	_resourceHandle = _vm->_res->useResource(fileHash);
	if (_resourceHandle != -1) {
		if (_vm->_res->getResourceType(_resourceHandle) == 2) {
			byte *spriteData = _vm->_res->loadResource(_resourceHandle, true);
			parseBitmapResource(spriteData, &_rle, &_dimensions, &_position, NULL, &_pixels);
		} else {
			_vm->_res->unuseResource(_resourceHandle);
			_resourceHandle = -1;
		}
	} 
	return _pixels != NULL;
}

void SpriteResource::unload() {
	if (_resourceHandle != -1) {
		_vm->_res->unloadResource(_resourceHandle);
		_vm->_res->unuseResource(_resourceHandle);
		_resourceHandle = -1;
	} else {
		delete[] _pixels;
	}
	_pixels = NULL;
	_rle = false;
}

// PaletteResource

PaletteResource::PaletteResource(NeverhoodEngine *vm)
	: _vm(vm), _resourceHandle(-1), _palette(NULL) {
}

PaletteResource::~PaletteResource() {
	unload();
}

bool PaletteResource::load(uint32 fileHash) {
	debug(2, "PaletteResource::load(%08X)", fileHash);
	unload();
	_resourceHandle = _vm->_res->useResource(fileHash);
	if (_resourceHandle != -1) {
		_palette = _vm->_res->loadResource(_resourceHandle, true);
		switch (_vm->_res->getResourceType(_resourceHandle)) {
		case 2:
			// Palette is stored in a bitmap
			parseBitmapResource(_palette, NULL, NULL, NULL, &_palette, NULL);
			break;
		case 3:
			// _palette already points to the correct data
			break;
		default:						
			_vm->_res->unuseResource(_resourceHandle);
			_resourceHandle = -1;
			break;
		}
	} 
	return _palette != NULL;
}

void PaletteResource::unload() {
	if (_resourceHandle != -1) {
		_vm->_res->unloadResource(_resourceHandle);
		_vm->_res->unuseResource(_resourceHandle);
		_resourceHandle = -1;
		_palette = NULL;
	}
}

void PaletteResource::copyPalette(byte *destPalette) {
	if (_palette) {
		memcpy(destPalette, _palette, 1024);
	} else {
		// TODO?: buildDefaultPalette(palette);
	}
}

// AnimResource

AnimResource::AnimResource(NeverhoodEngine *vm)
	: _vm(vm), _width(0), _height(0), _currSpriteData(NULL) {
	
	clear();
	clear2();
}

AnimResource::~AnimResource() {
	unloadInternal();
}

void AnimResource::draw(uint frameIndex, byte *dest, int destPitch, bool flipX, bool flipY) {
	const AnimFrameInfo frameInfo = _frames[frameIndex];
	_currSpriteData = _spriteData + frameInfo.spriteDataOffs;
	_width = frameInfo.rect.width;
	_height = frameInfo.rect.height;
	if (_replEnabled && _replOldColor != _replNewColor)
		unpackSpriteRleRepl(_currSpriteData, _width, _height, dest, destPitch, _replOldColor, _replNewColor, flipX, flipY);
	else
		unpackSpriteRle(_currSpriteData, _width, _height, dest, destPitch, flipX, flipY);
}

bool AnimResource::load(uint32 fileHash) {
	debug(2, "AnimResource::load(%08X)", fileHash);

	if (fileHash == _fileHash)
		return true;

	unload();
	_resourceHandle = _vm->_res->useResource(fileHash);
	if (_resourceHandle == -1)
		return false;
	
	byte *resourceData, *animList, *frameList;
	uint16 animInfoStartOfs, animListIndex, animListCount;
	uint16 frameListStartOfs, frameCount;
	uint32 spriteDataOfs, paletteDataOfs;

	if (_vm->_res->getResourceType(_resourceHandle) != 4) {
		_vm->_res->unuseResource(_resourceHandle);
		_resourceHandle = -1;
		return false;
	}
	
	resourceData = _vm->_res->loadResource(_resourceHandle);
	if (!resourceData) {
		_vm->_res->unuseResource(_resourceHandle);
		_resourceHandle = -1;
		return false;
	}

	animListCount = READ_LE_UINT16(resourceData);
	animInfoStartOfs = READ_LE_UINT16(resourceData + 2);
	spriteDataOfs = READ_LE_UINT32(resourceData + 4);
	paletteDataOfs = READ_LE_UINT32(resourceData + 8);
	
	animList = resourceData + 12;
	for (animListIndex = 0; animListIndex < animListCount; animListIndex++) {
		debug(8, "hash: %08X", READ_LE_UINT32(animList));
		if (READ_LE_UINT32(animList) == fileHash)
			break;
		animList += 8;				
	}
	
	if (animListIndex >= animListCount) {
		_vm->_res->unloadResource(_resourceHandle);
		_vm->_res->unuseResource(_resourceHandle);
		_resourceHandle = -1;
		return false;
	}

	_spriteData = resourceData + spriteDataOfs;
	if (paletteDataOfs > 0)
		_paletteData = resourceData + paletteDataOfs;
	
	frameCount = READ_LE_UINT16(animList + 4);
	frameListStartOfs = READ_LE_UINT16(animList + 6);
	
	debug(8, "frameCount = %d; frameListStartOfs = %04X; animInfoStartOfs = %04X", frameCount, frameListStartOfs, animInfoStartOfs);
	
	frameList = resourceData + animInfoStartOfs + frameListStartOfs;

	_frames.clear();
	_frames.reserve(frameCount);

	for (uint16 frameIndex = 0; frameIndex < frameCount; frameIndex++) {
		AnimFrameInfo frameInfo;
		frameInfo.frameHash = READ_LE_UINT32(frameList);
		frameInfo.counter = READ_LE_UINT16(frameList + 4);
		frameInfo.rect.x = READ_LE_UINT16(frameList + 6);
		frameInfo.rect.y = READ_LE_UINT16(frameList + 8);
		frameInfo.rect.width = READ_LE_UINT16(frameList + 10);
		frameInfo.rect.height = READ_LE_UINT16(frameList + 12);
		frameInfo.deltaX = READ_LE_UINT16(frameList + 14);
		frameInfo.deltaY = READ_LE_UINT16(frameList + 16);
		frameInfo.deltaRect.x = READ_LE_UINT16(frameList + 18); 
		frameInfo.deltaRect.y = READ_LE_UINT16(frameList + 20); 
		frameInfo.deltaRect.width = READ_LE_UINT16(frameList + 22); 
		frameInfo.deltaRect.height = READ_LE_UINT16(frameList + 24);
		frameInfo.field_1A = READ_LE_UINT16(frameList + 26); 
		frameInfo.spriteDataOffs = READ_LE_UINT32(frameList + 28);
		debug(8, "frameHash = %08X; counter = %d; rect = (%d,%d,%d,%d); deltaX = %d; deltaY = %d; deltaRect = (%d,%d,%d,%d); field_1A = %04X; spriteDataOffs = %08X", 
			frameInfo.frameHash, frameInfo.counter, 
			frameInfo.rect.x, frameInfo.rect.y, frameInfo.rect.width, frameInfo.rect.height,
			frameInfo.deltaX, frameInfo.deltaY,
			frameInfo.deltaRect.x, frameInfo.deltaRect.y, frameInfo.deltaRect.width, frameInfo.deltaRect.height,
			frameInfo.field_1A, frameInfo.spriteDataOffs);
		frameList += 32;
		_frames.push_back(frameInfo);
	}
	
	_fileHash = fileHash;
	
	return true;
	
}

void AnimResource::unload() {
	if (_resourceHandle != -1) {
		_vm->_res->unloadResource(_resourceHandle);
		_vm->_res->unuseResource(_resourceHandle);
		clear();
	}
}

void AnimResource::clear() {
	_resourceHandle = -1;
	_currSpriteData = NULL;
	_fileHash = 0;
	_paletteData = NULL;
	_spriteData = NULL;
}

void AnimResource::clear2() {
	clear();
	_replEnabled = true;
	_replOldColor = 0;
	_replNewColor = 0;
}

bool AnimResource::loadInternal(uint32 fileHash) {
	unloadInternal();
	return load(fileHash);
}

void AnimResource::unloadInternal() {
	unload();
	clear2();
}

int16 AnimResource::getFrameIndex(uint32 frameHash) {
	int16 frameIndex = -1;
	for (uint i = 0; i < _frames.size(); i++)
		if (_frames[i].frameHash == frameHash) {
			frameIndex = (int16)i;
			break;
		}
	debug(2, "AnimResource::getFrameIndex(%08X) -> %d", frameHash, frameIndex);
	return frameIndex;			
}

void AnimResource::setRepl(byte oldColor, byte newColor) {
	_replOldColor = oldColor;
	_replNewColor = newColor;
}

NDimensions AnimResource::loadSpriteDimensions(uint32 fileHash) {
	NDimensions dimensions;
	byte *resDimensions = _vm->_res->getResourceExtDataByHash(fileHash);
	if (resDimensions) {
		dimensions.width = READ_LE_UINT16(resDimensions + 0);
		dimensions.height = READ_LE_UINT16(resDimensions + 2);
	} else {
		dimensions.width = 0;
		dimensions.height = 0;
	}
	return dimensions;
}

// MouseCursorResource

MouseCursorResource::MouseCursorResource(NeverhoodEngine *vm) 
	: _cursorSprite(vm), _cursorNum(4), _currFileHash(0) {

	_rect.width = 32;
	_rect.height = 32;
}

void MouseCursorResource::load(uint32 fileHash) {
	if (_currFileHash != fileHash) {
		if (_cursorSprite.load(fileHash) && !_cursorSprite.isRle() &&
			_cursorSprite.getDimensions().width == 96 && _cursorSprite.getDimensions().height == 224) {
			_currFileHash = fileHash; 
		} else {
			unload();
		}
	}
}

void MouseCursorResource::unload() {
	_cursorSprite.unload();
	_currFileHash = 0;
	_cursorNum = 4;
}

NDrawRect& MouseCursorResource::getRect() {
	static const NPoint kCursorHotSpots[] = {
		{-15, -5},
		{-17, -25},
		{-17, -30},
		{-14, -1},
		{-3, -7},
		{-30, -18},
		{-1, -18}
	};
	_rect.x = kCursorHotSpots[_cursorNum].x;
	_rect.y = kCursorHotSpots[_cursorNum].y;
	return _rect;
}

void MouseCursorResource::draw(int frameNum, byte *dest, int destPitch) {
	if (_cursorSprite.getPixels()) {
		int sourcePitch = (_cursorSprite.getDimensions().width + 3) & 0xFFFC; // 4 byte alignment
		byte *source = _cursorSprite.getPixels() + _cursorNum * (sourcePitch * 32) + frameNum * 32;
		for (int16 yc = 0; yc < 32; yc++) {
			memcpy(dest, source, 32);
			source += sourcePitch;
			dest += destPitch;
		}	
	}
}

// TextResource

TextResource::TextResource(NeverhoodEngine *vm)
	: _vm(vm), _resourceHandle(-1), _textData(NULL), _count(0) {
	
}

TextResource::~TextResource() {
	unload();
}

void TextResource::load(uint32 fileHash) {
	unload();
	_resourceHandle = _vm->_res->useResource(fileHash);
	if (_resourceHandle != -1) {
		if (_vm->_res->getResourceType(_resourceHandle) == 6) {
			_textData = _vm->_res->loadResource(_resourceHandle, true);
			_count = READ_LE_UINT32(_textData);
		} else {
			_vm->_res->unuseResource(_resourceHandle);
			_resourceHandle = -1;
		}
	}
}

void TextResource::unload() {
	if (_resourceHandle != -1) {
		_vm->_res->unloadResource(_resourceHandle);
		_vm->_res->unuseResource(_resourceHandle);
		_resourceHandle = -1;
		_textData = NULL;
		_count = 0;
	}
}

const char *TextResource::getString(uint index, const char *&textEnd) {
	const char *textStart = (const char*)(_textData + 4 + _count * 4 + READ_LE_UINT32(_textData + (index + 1) * 4));
	textEnd = (const char*)(_textData + 4 + _count * 4 + READ_LE_UINT32(_textData + (index + 2) * 4));
	return textStart;
}

// DataResource

DataResource::DataResource(NeverhoodEngine *vm)
	: _vm(vm), _resourceHandle(-1) {
}

DataResource::~DataResource() {
	unload();
}

void DataResource::load(uint32 fileHash) {
	debug(2, "DataResource::load(%08X)", fileHash);
	byte *data = NULL;
	uint32 dataSize = 0;
	unload();
	_resourceHandle = _vm->_res->useResource(fileHash);
	if (_resourceHandle != -1) {
		if (_vm->_res->getResourceType(_resourceHandle) == 5) {
			data = _vm->_res->loadResource(_resourceHandle, true);
			dataSize = _vm->_res->getResourceSize(_resourceHandle);
		} else {
			_vm->_res->unuseResource(_resourceHandle);
			_resourceHandle = -1;
		}
	}
	if (data && dataSize) {
		Common::MemoryReadStream dataS(data, dataSize);
		uint itemCount = dataS.readUint16LE();
		uint32 itemStartOffs = 2 + itemCount * 8;
		debug(2, "itemCount = %d", itemCount);
		for (uint i = 0; i < itemCount; i++) {
			dataS.seek(2 + i * 8);
			DRDirectoryItem drDirectoryItem;
			drDirectoryItem.nameHash = dataS.readUint32LE();
			drDirectoryItem.offset = dataS.readUint16LE(); 
			drDirectoryItem.type = dataS.readUint16LE();
			debug(2, "%03d nameHash = %08X; offset = %04X; type = %d", i, drDirectoryItem.nameHash, drDirectoryItem.offset, drDirectoryItem.type);
			dataS.seek(itemStartOffs + drDirectoryItem.offset);
			switch (drDirectoryItem.type) {
			case 1:
				{
					debug(3, "NPoint");
					NPoint point;
					point.x = dataS.readUint16LE();
					point.y = dataS.readUint16LE();
					debug(3, "(%d, %d)", point.x, point.y);
					drDirectoryItem.offset = _points.size();
					_points.push_back(point);
					break;
				}
			case 2:
				{
					uint count = dataS.readUint16LE();
					NPointArray *pointArray = new NPointArray();
					debug(3, "NPointArray; count = %d", count);
					for (uint j = 0; j < count; j++) {
						NPoint point;
						point.x = dataS.readUint16LE();
						point.y = dataS.readUint16LE();
						debug(3, "(%d, %d)", point.x, point.y);
						pointArray->push_back(point);
					}
					drDirectoryItem.offset = _pointArrays.size();
					_pointArrays.push_back(pointArray);
					break;
				}
			case 3:
				{
					uint count = dataS.readUint16LE();
					HitRectList *hitRectList = new HitRectList();
					debug(3, "HitRectList; count = %d", count);
					for (uint j = 0; j < count; j++) {
						HitRect hitRect;
						hitRect.rect.x1 = dataS.readUint16LE();
						hitRect.rect.y1 = dataS.readUint16LE();
						hitRect.rect.x2 = dataS.readUint16LE();
						hitRect.rect.y2 = dataS.readUint16LE();
						hitRect.type = dataS.readUint16LE() + 0x5001; 
						debug(3, "(%d, %d, %d, %d) -> %04d", hitRect.rect.x1, hitRect.rect.y1, hitRect.rect.x2, hitRect.rect.y2, hitRect.type);
						hitRectList->push_back(hitRect);
					}
					drDirectoryItem.offset = _hitRectLists.size();
					_hitRectLists.push_back(hitRectList);
					break;
				}
			case 4:
				{
					uint count = dataS.readUint16LE();
					MessageList *messageList = new MessageList();
					debug(3, "MessageList; count = %d", count);
					for (uint j = 0; j < count; j++) {
						MessageItem messageItem;
						messageItem.messageNum = dataS.readUint32LE();
						messageItem.messageValue = dataS.readUint32LE();
						debug(3, "(%08X, %08X)", messageItem.messageNum, messageItem.messageValue);
						messageList->push_back(messageItem);
					}
					drDirectoryItem.offset = _messageLists.size();
					_messageLists.push_back(messageList);
					break;
				}
			case 5:
				{
					uint count = dataS.readUint16LE();
					DRSubRectList *drSubRectList = new DRSubRectList();
					debug(3, "SubRectList; count = %d", count);
					for (uint j = 0; j < count; j++) {
						DRSubRect drSubRect;
						drSubRect.rect.x1 = dataS.readUint16LE();
						drSubRect.rect.y1 = dataS.readUint16LE();
						drSubRect.rect.x2 = dataS.readUint16LE();
						drSubRect.rect.y2 = dataS.readUint16LE();
						drSubRect.messageListHash = dataS.readUint32LE();
						drSubRect.messageListItemIndex = dataS.readUint16LE();
						debug(3, "(%d, %d, %d, %d) -> %08X (%d)", drSubRect.rect.x1, drSubRect.rect.y1, drSubRect.rect.x2, drSubRect.rect.y2, drSubRect.messageListHash, drSubRect.messageListItemIndex);
						drSubRectList->push_back(drSubRect);
					}
					drDirectoryItem.offset = _drSubRectLists.size();
					_drSubRectLists.push_back(drSubRectList);
					break;
				}
			case 6:
				{
					DRRect drRect;
					drRect.rect.x1 = dataS.readUint16LE();
					drRect.rect.y1 = dataS.readUint16LE();
					drRect.rect.x2 = dataS.readUint16LE();
					drRect.rect.y2 = dataS.readUint16LE();
					drRect.subRectIndex = dataS.readUint16LE();
					debug(3, "(%d, %d, %d, %d) -> %d", drRect.rect.x1, drRect.rect.y1, drRect.rect.x2, drRect.rect.y2, drRect.subRectIndex);
					drDirectoryItem.offset = _drRects.size();
					_drRects.push_back(drRect);
					break;
				}
			case 7:
				{
					uint count = dataS.readUint16LE();
					NRectArray *rectArray = new NRectArray();
					debug(3, "NRectArray; count = %d", count);
					for (uint j = 0; j < count; j++) {
						NRect rect;
						rect.x1 = dataS.readUint16LE();
						rect.y1 = dataS.readUint16LE();
						rect.x2 = dataS.readUint16LE();
						rect.y2 = dataS.readUint16LE();
						debug(3, "(%d, %d, %d, %d)", rect.x1, rect.y1, rect.x2, rect.y2);
						rectArray->push_back(rect);
					}
					drDirectoryItem.offset = _rectArrays.size();
					_rectArrays.push_back(rectArray);
					break;
				}
			}
			_directory.push_back(drDirectoryItem); 
		}
	}
}

void DataResource::unload() {
	if (_resourceHandle != -1) {
		_vm->_res->unloadResource(_resourceHandle);
		_vm->_res->unuseResource(_resourceHandle);
		_resourceHandle = -1;
		// TODO: Clear arrays
	}
}

NPoint DataResource::getPoint(uint32 nameHash) {
	DataResource::DRDirectoryItem *drDirectoryItem = findDRDirectoryItem(nameHash, 1);
	if (drDirectoryItem)
		return _points[drDirectoryItem->offset];
	return NPoint();
}

NPointArray *DataResource::getPointArray(uint32 nameHash) {
	DataResource::DRDirectoryItem *drDirectoryItem = findDRDirectoryItem(nameHash, 2);
	if (drDirectoryItem)
		return _pointArrays[drDirectoryItem->offset];
	return NULL;
}

HitRectList *DataResource::getHitRectList() {
	DataResource::DRDirectoryItem *drDirectoryItem = findDRDirectoryItem(calcHash("HitArray"), 3);
	if (drDirectoryItem)
		return _hitRectLists[drDirectoryItem->offset];
	return NULL;
}

MessageList *DataResource::getMessageListAtPos(int16 klaymanX, int16 klaymanY, int16 mouseX, int16 mouseY) {
	for (uint i = 0; i < _drRects.size(); i++) {
		if (klaymanX >= _drRects[i].rect.x1 && klaymanX <= _drRects[i].rect.x2 && 
			klaymanY >= _drRects[i].rect.y1 && klaymanY <= _drRects[i].rect.y2) {
			DRSubRectList *drSubRectList = _drSubRectLists[_drRects[i].subRectIndex]; 
			for (uint j = 0; j < drSubRectList->size(); j++) {
				DRSubRect &subRect = (*drSubRectList)[j];
				if (mouseX >= subRect.rect.x1 && mouseX <= subRect.rect.x2 && 
					mouseY >= subRect.rect.y1 && mouseY <= subRect.rect.y2) {
					return _messageLists[subRect.messageListItemIndex];
				}
			}
		}
	}
	return NULL;
}

DataResource::DRDirectoryItem *DataResource::findDRDirectoryItem(uint32 nameHash, uint16 type) {
	for (Common::Array<DRDirectoryItem>::iterator it = _directory.begin(); it != _directory.end(); it++) {
		if ((*it).nameHash == nameHash && (*it).type == type)
			return &(*it);
	}
	return NULL;
}

// SoundResource
// ALL TODO

SoundResource::SoundResource(NeverhoodEngine *vm)
	: _vm(vm) {
}

bool SoundResource::isPlaying() { 
	return false; 
}

void SoundResource::load(uint32 fileHash) {
}

void SoundResource::play(uint32 fileHash, bool looping) {
}

void SoundResource::play() {
}

uint32 calcHash(const char *value) {
	uint32 hash = 0, shiftValue = 0;
	while (*value != 0) {
		char ch = *value++;
		if (ch >= 'a' && ch <= 'z')
			ch -= 32;
		else if (ch >= '0' && ch <= '9')
			ch += 22;
		shiftValue += ch - 64;
		if (shiftValue >= 32)
			shiftValue -= 32;
		hash ^= 1 << shiftValue;
	}
	return hash;
}

} // End of namespace Neverhood