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
 * $URL$
 * $Id$
 *
 */

#include "common/endian.h"
#include "common/util.h"
#include "common/events.h"

#include "graphics/cursorman.h"

#include "made/made.h"
#include "made/resource.h"
#include "made/database.h"
#include "made/screen.h"
#include "made/script.h"
#include "made/pmvplayer.h"
#include "made/scriptfuncs.h"
#include "made/music.h"

namespace Made {

typedef Common::Functor2Mem<int16, int16*, int16, ScriptFunctions> ExternalScriptFunc;
#define External(x) \
	_externalFuncs.push_back(new ExternalScriptFunc(this, &ScriptFunctions::x));  \
	_externalFuncNames.push_back(#x);
void ScriptFunctions::setupExternalsTable() {

	External(sfSystemCall);
	External(sfInitGraf);
	External(sfRestoreGraf);
	External(sfDrawPicture);
	External(sfClearScreen);
	External(sfShowPage);
	External(sfPollEvent);
	External(sfGetMouseX);
	External(sfGetMouseY);
	External(sfGetKey);
	External(sfSetVisualEffect);
	External(sfPlaySound);
	External(sfPlayMusic);
	External(sfStopMusic);
	External(sfIsMusicPlaying);
	External(sfSetTextPos);
	External(sfFlashScreen);
	External(sfPlayNote);
	External(sfStopNote);
	External(sfPlayTele);
	External(sfStopTele);
	External(sfHideMouseCursor);
	External(sfShowMouseCursor);
	External(sfGetMusicBeat);
	External(sfSetScreenLock);
	External(sfAddSprite);
	External(sfFreeAnim);
	External(sfDrawSprite);
	External(sfEraseSprites);
	External(sfUpdateSprites);
	External(sfGetTimer);
	External(sfSetTimer);
	External(sfResetTimer);
	External(sfAllocTimer);
	External(sfFreeTimer);
	External(sfSetPaletteLock);
	External(sfSetFont);
	External(sfDrawText);
	External(sfHomeText);
	External(sfSetTextRect);
	External(sfSetTextXY);
	External(sfSetFontDropShadow);
	External(sfSetFontColor);
	External(sfSetFontOutline);
	External(sfLoadMouseCursor);
	External(sfSetSpriteGround);
	External(sfLoadResText);
	
	if (_vm->getGameID() == GID_MANHOLE || _vm->getGameID() == GID_LGOP2) {
		External(sfAddScreenMask);
		External(sfSetSpriteMask);
	} else if (_vm->getGameID() == GID_RTZ) {
		External(sfSetClipArea);
		External(sfSetSpriteClip);
	}

	External(sfSoundPlaying);
	External(sfStopSound);
	External(sfPlayVoice);

	if (_vm->getGameID() == GID_MANHOLE || _vm->getGameID() == GID_RTZ) {
		External(sfPlayCd);
		External(sfStopCd);
		External(sfGetCdStatus);
		External(sfGetCdTime);
		External(sfPlayCdSegment);
	}
	
	if (_vm->getGameID() == GID_RTZ) {
		External(sfPrintf);
		External(sfClearMono);
		External(sfGetSoundEnergy);
		External(sfClearText);
		External(sfAnimText);
		External(sfGetTextWidth);
		External(sfPlayMovie);
		External(sfLoadSound);
		External(sfLoadMusic);
		External(sfLoadPicture);
		External(sfSetMusicVolume);
		External(sfRestartEvents);
		External(sfPlaceSprite);
		External(sfPlaceText);
		External(sfDeleteChannel);
		External(sfGetChannelType);
		External(sfSetChannelState);
		External(sfSetChannelLocation);
		External(sfSetChannelContent);
		External(sfSetExcludeArea);
		External(sfSetSpriteExclude);
		External(sfGetChannelState);
		External(sfPlaceAnim);
		External(sfSetAnimFrame);
		External(sfGetAnimFrame);
		External(sfGetAnimFrameCount);
		External(sfGetPictureWidth);
		External(sfGetPictureHeight);
		External(sfSetSoundRate);
		External(sfDrawAnimPic);
		External(sfLoadAnim);
		External(sfReadText);
		External(sfReadMenu);
		External(sfDrawMenu);
		External(sfGetMenuCount);
		External(sfSaveGame);
		External(sfLoadGame);
		External(sfGetGameDescription);
		External(sfShakeScreen);
		External(sfPlaceMenu);
		External(sfSetSoundVolume);
		External(sfGetSynthType);
		External(sfIsSlowSystem);
	}

}
#undef External

int16 ScriptFunctions::sfSystemCall(int16 argc, int16 *argv) {
	// This opcode is empty.
	return 0;
}

int16 ScriptFunctions::sfInitGraf(int16 argc, int16 *argv) {
	// This opcode is empty.
	return 0;
}

int16 ScriptFunctions::sfRestoreGraf(int16 argc, int16 *argv) {
	// This opcode is empty.
	return 0;
}

int16 ScriptFunctions::sfDrawPicture(int16 argc, int16 *argv) {
	return _vm->_screen->drawPic(argv[4], argv[3], argv[2], argv[1], argv[0]);
}

int16 ScriptFunctions::sfClearScreen(int16 argc, int16 *argv) {
 	_vm->_screen->clearScreen();
	return 0;
}

int16 ScriptFunctions::sfShowPage(int16 argc, int16 *argv) {
 	if (_vm->getGameID() != GID_RTZ)
		_vm->_mixer->stopHandle(_audioStreamHandle);
	_vm->_screen->show();
	return 0;
}

int16 ScriptFunctions::sfPollEvent(int16 argc, int16 *argv) {
	int16 eventNum = _vm->_eventNum;
	_vm->_eventNum = 0;
	return eventNum;
}

int16 ScriptFunctions::sfGetMouseX(int16 argc, int16 *argv) {
	return _vm->_eventMouseX;
}

int16 ScriptFunctions::sfGetMouseY(int16 argc, int16 *argv) {
	return _vm->_eventMouseY;
}

int16 ScriptFunctions::sfGetKey(int16 argc, int16 *argv) {
	return _vm->_eventKey;
}

int16 ScriptFunctions::sfSetVisualEffect(int16 argc, int16 *argv) {
	_vm->_screen->setVisualEffectNum(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfPlaySound(int16 argc, int16 *argv) {
	int soundNum = argv[0];
	bool loop = false;

	if (argc > 1) {
		soundNum = argv[1];
		loop = (argv[0] == 1);
	}

	if (soundNum > 0) {
		if (!_vm->_mixer->isSoundHandleActive(_audioStreamHandle)) {
			_vm->_mixer->playInputStream(Audio::Mixer::kPlainSoundType, &_audioStreamHandle, 
										 _vm->_res->getSound(soundNum)->getAudioStream(_vm->_soundRate, loop));
		}
	}

	return 0;
}

int16 ScriptFunctions::sfPlayMusic(int16 argc, int16 *argv) {
	// TODO: Music in LGOP2 and Manhole isn't supported yet
	if (_vm->getGameID() == GID_RTZ) {
		int16 musicNum = argv[0];
		if (musicNum > 0) {
			_musicRes = _vm->_res->getXmidi(musicNum);
			if (_musicRes)
				_vm->_music->playXMIDI(_musicRes);
		}
	}
	return 0;
}

int16 ScriptFunctions::sfStopMusic(int16 argc, int16 *argv) {
	if (_vm->_music->isPlaying() && _musicRes) {
		_vm->_music->stop();
		_vm->_res->freeResource(_musicRes);
		_musicRes = NULL;
	}
	return 0;
}

int16 ScriptFunctions::sfIsMusicPlaying(int16 argc, int16 *argv) {
	if (_vm->_music->isPlaying())
		return 1;
	else
		return 0;
}

int16 ScriptFunctions::sfSetTextPos(int16 argc, int16 *argv) {
	// TODO: Used in Manhole:NE
	warning("Unimplemented opcode: sfSetTextPos");
	// This seems to be some kind of low-level opcode.
	// The original engine calls int 10h to set the VGA cursor position.
	return 0;
}

int16 ScriptFunctions::sfFlashScreen(int16 argc, int16 *argv) {
	_vm->_screen->flash(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfPlayNote(int16 argc, int16 *argv) {
	// TODO: Used in Manhole:NE
	warning("Unimplemented opcode: sfPlayNote");
	return 0;
}

int16 ScriptFunctions::sfStopNote(int16 argc, int16 *argv) {
	// TODO: Used in Manhole:NE
	warning("Unimplemented opcode: sfStopNote");
	return 0;
}

int16 ScriptFunctions::sfPlayTele(int16 argc, int16 *argv) {
	// TODO: Used in Manhole:NE
	warning("Unimplemented opcode: sfPlayTele");
	return 0;
}

int16 ScriptFunctions::sfStopTele(int16 argc, int16 *argv) {
	// TODO: Used in Manhole:NE
	warning("Unimplemented opcode: sfStopTele");
	return 0;
}

int16 ScriptFunctions::sfHideMouseCursor(int16 argc, int16 *argv) {
	_vm->_system->showMouse(false);
	return 0;
}

int16 ScriptFunctions::sfShowMouseCursor(int16 argc, int16 *argv) {
	_vm->_system->showMouse(true);
	return 0;
}

int16 ScriptFunctions::sfGetMusicBeat(int16 argc, int16 *argv) {
	// This is called loads of times in the intro of the floppy version
	// of RtZ. Not sure what it does. Commented out to reduce spam
	//warning("Unimplemented opcode: sfGetMusicBeat");
	return 0;
}

int16 ScriptFunctions::sfSetScreenLock(int16 argc, int16 *argv) {
	_vm->_screen->setScreenLock(argv[0] != 0);
	return 0;
}

int16 ScriptFunctions::sfAddSprite(int16 argc, int16 *argv) {
	if (_vm->getGameID() == GID_RTZ) {
		// Unused in RTZ
		return 0;
	} if (_vm->getGameID() == GID_LGOP2 || _vm->getGameID() == GID_MANHOLE) {
		return _vm->_screen->addToSpriteList(argv[2], argv[1], argv[0]);
	} else {
		return 0;
	}
}

int16 ScriptFunctions::sfFreeAnim(int16 argc, int16 *argv) {
	_vm->_screen->clearChannels();
	if (_vm->getGameID() == GID_LGOP2 || _vm->getGameID() == GID_MANHOLE) {
		_vm->_screen->clearSpriteList();
	}
	return 0;
}

int16 ScriptFunctions::sfDrawSprite(int16 argc, int16 *argv) {
	if (_vm->getGameID() == GID_RTZ) {
		return _vm->_screen->drawSprite(argv[2], argv[1], argv[0]);
	} if (_vm->getGameID() == GID_LGOP2 || _vm->getGameID() == GID_MANHOLE) {
		SpriteListItem item = _vm->_screen->getFromSpriteList(argv[2]);
		int16 channelIndex = _vm->_screen->drawSprite(item.index, argv[1] - item.xofs, argv[0] - item.yofs);
		_vm->_screen->setChannelUseMask(channelIndex);
		return 0;
	} else {
		return 0;
	}
}

int16 ScriptFunctions::sfEraseSprites(int16 argc, int16 *argv) {
	_vm->_screen->clearChannels();
	return 0;
}

int16 ScriptFunctions::sfUpdateSprites(int16 argc, int16 *argv) {
	_vm->_screen->updateSprites();
	return 0;
}

int16 ScriptFunctions::sfGetTimer(int16 argc, int16 *argv) {
	return _vm->getTimer(argv[0]);
}

int16 ScriptFunctions::sfSetTimer(int16 argc, int16 *argv) {
	_vm->setTimer(argv[1], argv[0]);
	return 0;
}

int16 ScriptFunctions::sfResetTimer(int16 argc, int16 *argv) {
	_vm->resetTimer(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfAllocTimer(int16 argc, int16 *argv) {
	return _vm->allocTimer();
}

int16 ScriptFunctions::sfFreeTimer(int16 argc, int16 *argv) {
	_vm->freeTimer(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfSetPaletteLock(int16 argc, int16 *argv) {
	_vm->_screen->setPaletteLock(argv[0] != 0);
	return 0;
}

int16 ScriptFunctions::sfSetFont(int16 argc, int16 *argv) {
	_vm->_screen->setFont(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfDrawText(int16 argc, int16 *argv) {

	const char *text = NULL;

	if (_vm->getGameID() == GID_RTZ) {
		Object *obj = _vm->_dat->getObject(argv[argc - 1]);
		text = obj->getString();
	} if (_vm->getGameID() == GID_LGOP2 || _vm->getGameID() == GID_MANHOLE) {
		text = _vm->_dat->getString(argv[argc - 1]);
	}

	if (text) {
		char finalText[1024];
		switch (argc) {
		case 1:
			snprintf(finalText, 1024, "%s", text);
			break;
		case 2:
			snprintf(finalText, 1024, text, argv[0]);
			break;
		case 3:
			snprintf(finalText, 1024, text, argv[1], argv[0]);
			break;
		case 4:
			snprintf(finalText, 1024, text, argv[2], argv[1], argv[0]);
			break;
		case 5:
			snprintf(finalText, 1024, text, argv[3], argv[2], argv[1], argv[0]);
			break;
		default:
			finalText[0] = '\0';
			break;
		}
		_vm->_screen->printText(finalText);
	}

	return 0;
}

int16 ScriptFunctions::sfHomeText(int16 argc, int16 *argv) {
	// TODO: Used in LGOP2
	warning("Unimplemented opcode: sfHomeText");
	return 0;
}

int16 ScriptFunctions::sfSetTextRect(int16 argc, int16 *argv) {
	// Used in the save/load screens of RtZ, and perhaps other places as well
	int16 x1 = CLIP<int16>(argv[4], 1, 318);
	int16 y1 = CLIP<int16>(argv[3], 1, 198);
	int16 x2 = CLIP<int16>(argv[2], 1, 318);
	int16 y2 = CLIP<int16>(argv[1], 1, 198);
	//int16 textValue = argv[0];
	// TODO: textValue
	_vm->_screen->setTextRect(Common::Rect(x1, y1, x2, y2));
	return 0;
}

int16 ScriptFunctions::sfSetTextXY(int16 argc, int16 *argv) {
	int16 x = CLIP<int16>(argv[1], 1, 318);
	int16 y = CLIP<int16>(argv[0], 1, 198);
	_vm->_screen->setTextXY(x, y);
	return 0;
}

int16 ScriptFunctions::sfSetFontDropShadow(int16 argc, int16 *argv) {
	// if the drop shadow color is -1, then text drop shadow is disabled
	// when font drop shadow is enabled, outline is disabled
	_vm->_screen->setDropShadowColor(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfSetFontColor(int16 argc, int16 *argv) {
	_vm->_screen->setTextColor(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfSetFontOutline(int16 argc, int16 *argv) {
	// if the outline color is -1, then text outline is disabled
	// when font outline is enabled, drop shadow is disabled
	_vm->_screen->setOutlineColor(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfLoadMouseCursor(int16 argc, int16 *argv) {
	PictureResource *flex = _vm->_res->getPicture(argv[2]);
	if (flex) {
		Graphics::Surface *surf = flex->getPicture();
		CursorMan.replaceCursor((const byte *)surf->pixels, surf->w, surf->h, argv[1], argv[0], 0);
		CursorMan.showMouse(true);
		_vm->_res->freeResource(flex);
	}
	return 0;
}

int16 ScriptFunctions::sfSetSpriteGround(int16 argc, int16 *argv) {
	_vm->_screen->setGround(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfLoadResText(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfLoadResText");
	return 0;
}

int16 ScriptFunctions::sfSetClipArea(int16 argc, int16 *argv) {
	_vm->_screen->setClipArea(argv[3], argv[2], argv[1], argv[0]);
	return 0;
}

int16 ScriptFunctions::sfSetSpriteClip(int16 argc, int16 *argv) {
	_vm->_screen->setClip(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfAddScreenMask(int16 argc, int16 *argv) {
	_vm->_screen->drawMask(argv[2], argv[1], argv[0]);
	return 0;
}

int16 ScriptFunctions::sfSetSpriteMask(int16 argc, int16 *argv) {
	_vm->_screen->setMask(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfSoundPlaying(int16 argc, int16 *argv) {
	if (_vm->_mixer->isSoundHandleActive(_audioStreamHandle))
		return 1;
	else
		return 0;
}

int16 ScriptFunctions::sfStopSound(int16 argc, int16 *argv) {
	_vm->_mixer->stopHandle(_audioStreamHandle);
	return 0;
}

int16 ScriptFunctions::sfPlayVoice(int16 argc, int16 *argv) {
	if (argv[0] > 0) {
		_vm->_mixer->stopHandle(_audioStreamHandle);
		_vm->_mixer->playInputStream(Audio::Mixer::kPlainSoundType, &_audioStreamHandle,
			_vm->_res->getSound(argv[0])->getAudioStream(_vm->_soundRate, false));
	}
	return 0;
}

int16 ScriptFunctions::sfPlayCd(int16 argc, int16 *argv) {
	// This one is called loads of times, so it has been commented out to reduce spam
	//warning("Unimplemented opcode: sfPlayCd");
	return 0;
}

int16 ScriptFunctions::sfStopCd(int16 argc, int16 *argv) {
	warning("Unimplemented opcode: sfStopCd");
	return 0;
}

int16 ScriptFunctions::sfGetCdStatus(int16 argc, int16 *argv) {
	// This one is called loads of times, so it has been commented out to reduce spam
	//warning("Unimplemented opcode: sfGetCdStatus");
	return 0;
}

int16 ScriptFunctions::sfGetCdTime(int16 argc, int16 *argv) {
	// This one is called loads of times, so it has been commented out to reduce spam
	//warning("Unimplemented opcode: sfGetCdTime");
	return 0;
}

int16 ScriptFunctions::sfPlayCdSegment(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfPlayCdSegment");
	return 0;
}

int16 ScriptFunctions::sfPrintf(int16 argc, int16 *argv) {
	Object *obj = _vm->_dat->getObject(argv[argc - 1]);
	const char *text = obj->getString();
	debug(4, "--> text = %s", text);
	return 0;
}

int16 ScriptFunctions::sfClearMono(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfClearMono");
	return 0;
}

int16 ScriptFunctions::sfGetSoundEnergy(int16 argc, int16 *argv) {
	// This is called while in-game voices are played
	// Not sure what it's used for
	// -> It's used to animate mouths when NPCs are talking
	// FIXME: This is a workaround for the "sound energy" problem
	// At least the characters move their lips when talking now
	return _vm->_rnd->getRandomNumber(5);
}

int16 ScriptFunctions::sfClearText(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfClearText");
	return 1;
}

int16 ScriptFunctions::sfAnimText(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfAnimText");
	return 0;
}

int16 ScriptFunctions::sfGetTextWidth(int16 argc, int16 *argv) {
	int16 width = 0;
	if (argv[1] > 0) {
		Object *obj = _vm->_dat->getObject(argv[1]);
		const char *text = obj->getString();
		width = _vm->_screen->getTextWidth(argv[0], text);
	}
	return width;
}

int16 ScriptFunctions::sfPlayMovie(int16 argc, int16 *argv) {
	const char *movieName = _vm->_dat->getObject(argv[1])->getString();
	_vm->_system->showMouse(false);
	_vm->_pmvPlayer->play(movieName);
	_vm->_system->showMouse(true);
	return 0;
}

int16 ScriptFunctions::sfLoadSound(int16 argc, int16 *argv) {
	SoundResource *sound = _vm->_res->getSound(argv[0]);
	if (sound) {
		_vm->_res->freeResource(sound);
		return 1;
	}
	return 0;
}

int16 ScriptFunctions::sfLoadMusic(int16 argc, int16 *argv) {
	GenericResource *xmidi = _vm->_res->getXmidi(argv[0]);
	if (xmidi) {
		_vm->_res->freeResource(xmidi);
		return 1;
	}
	return 0;
}

int16 ScriptFunctions::sfLoadPicture(int16 argc, int16 *argv) {
	PictureResource *flex = _vm->_res->getPicture(argv[0]);
	if (flex) {
		_vm->_res->freeResource(flex);
		return 1;
	}
	return 0;
}

int16 ScriptFunctions::sfSetMusicVolume(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfSetMusicVolume");
	return 0;
}

int16 ScriptFunctions::sfRestartEvents(int16 argc, int16 *argv) {
	// TODO: Used in RTZ
	warning("Unimplemented opcode: sfRestartEvents");
	return 0;
}

int16 ScriptFunctions::sfPlaceSprite(int16 argc, int16 *argv) {
	return _vm->_screen->placeSprite(argv[3], argv[2], argv[1], argv[0]);
}

int16 ScriptFunctions::sfPlaceText(int16 argc, int16 *argv) {
	return _vm->_screen->placeText(argv[6], argv[5], argv[4], argv[3], argv[2], argv[1], argv[0]);
}

int16 ScriptFunctions::sfDeleteChannel(int16 argc, int16 *argv) {
	_vm->_screen->deleteChannel(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfGetChannelType(int16 argc, int16 *argv) {
	return _vm->_screen->getChannelType(argv[0]);
}

int16 ScriptFunctions::sfSetChannelState(int16 argc, int16 *argv) {
	_vm->_screen->setChannelState(argv[1], argv[0]);
	return 0;
}

int16 ScriptFunctions::sfSetChannelLocation(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfSetChannelLocation");
	return 0;
}

int16 ScriptFunctions::sfSetChannelContent(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfSetChannelContent");
	return 0;
}

int16 ScriptFunctions::sfSetExcludeArea(int16 argc, int16 *argv) {
	_vm->_screen->setExcludeArea(argv[3], argv[2], argv[1], argv[0]);
	return 0;
}

int16 ScriptFunctions::sfSetSpriteExclude(int16 argc, int16 *argv) {
	_vm->_screen->setExclude(argv[0]);
	return 0;
}

int16 ScriptFunctions::sfGetChannelState(int16 argc, int16 *argv) {
	return _vm->_screen->getChannelState(argv[0]);
}

int16 ScriptFunctions::sfPlaceAnim(int16 argc, int16 *argv) {
	return _vm->_screen->placeAnim(argv[4], argv[3], argv[2], argv[1], argv[0]);
}

int16 ScriptFunctions::sfSetAnimFrame(int16 argc, int16 *argv) {
	_vm->_screen->setAnimFrame(argv[1], argv[0]);
	return 0;
}

int16 ScriptFunctions::sfGetAnimFrame(int16 argc, int16 *argv) {
	return _vm->_screen->getAnimFrame(argv[0]);
}

int16 ScriptFunctions::sfGetAnimFrameCount(int16 argc, int16 *argv) {
	int16 frameCount = 0;
	AnimationResource *anim = _vm->_res->getAnimation(argv[0]);
	if (anim) {
		frameCount = anim->getCount();
		_vm->_res->freeResource(anim);
	}
	return frameCount;
}

int16 ScriptFunctions::sfGetPictureWidth(int16 argc, int16 *argv) {
	int16 width = 0;
	PictureResource *flex = _vm->_res->getPicture(argv[0]);
	if (flex) {
		width = flex->getPicture()->w;
		_vm->_res->freeResource(flex);
	}
	return width;
}

int16 ScriptFunctions::sfGetPictureHeight(int16 argc, int16 *argv) {
	int16 height = 0;
	PictureResource *flex = _vm->_res->getPicture(argv[0]);
	if (flex) {
		height = flex->getPicture()->h;
		_vm->_res->freeResource(flex);
	}
	return height;
}

int16 ScriptFunctions::sfSetSoundRate(int16 argc, int16 *argv) {
	_vm->_soundRate = argv[0];
	return 1;
}

int16 ScriptFunctions::sfDrawAnimPic(int16 argc, int16 *argv) {
	// TODO: Used in RTZ
	warning("Unimplemented opcode: sfDrawAnimPic");
	return 0;
}

int16 ScriptFunctions::sfLoadAnim(int16 argc, int16 *argv) {
	AnimationResource *anim = _vm->_res->getAnimation(argv[0]);
	if (anim) {
		_vm->_res->freeResource(anim);
		return 1;
	}
	return 0;
}

int16 ScriptFunctions::sfReadText(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfReadText");
	return 0;
}

int16 ScriptFunctions::sfReadMenu(int16 argc, int16 *argv) {
	int16 objectIndex = argv[2];
	int16 menuIndex = argv[1];
	int16 textIndex = argv[0];
	int16 length = 0;
	MenuResource *menu = _vm->_res->getMenu(menuIndex);
	if (menu) {
		const char *text = menu->getString(textIndex);
		debug(4, "objectIndex = %04X; text = %s\n", objectIndex, text);
		Object *obj = _vm->_dat->getObject(objectIndex);
		obj->setString(text);
		_vm->_res->freeResource(menu);
		if (text)
			length = strlen(text);
	}
	return length;
}

int16 ScriptFunctions::sfDrawMenu(int16 argc, int16 *argv) {
	int16 menuIndex = argv[1];
	int16 textIndex = argv[0];
	MenuResource *menu = _vm->_res->getMenu(menuIndex);
	if (menu) {
		const char *text = menu->getString(textIndex);
		if (text)
			_vm->_screen->printText(text);
		_vm->_res->freeResource(menu);
	}
	return 0;
}

int16 ScriptFunctions::sfGetMenuCount(int16 argc, int16 *argv) {
	int16 menuIndex = argv[0];
	int16 count = 0;
	MenuResource *menu = _vm->_res->getMenu(menuIndex);
	if (menu) {
		count = menu->getCount();
		_vm->_res->freeResource(menu);
	}
	return count;
}

int16 ScriptFunctions::sfSaveGame(int16 argc, int16 *argv) {
	
	int16 saveNum = argv[2];
	int16 descObjectIndex = argv[1];
	int16 version = argv[0];
	
	if (saveNum > 999)
		return 6;

	Object *obj = _vm->_dat->getObject(descObjectIndex);
	const char *description = obj->getString();

	Common::String filename = _vm->getSavegameFilename(saveNum);

	return _vm->_dat->savegame(filename.c_str(), description, version);
	
}

int16 ScriptFunctions::sfLoadGame(int16 argc, int16 *argv) {

	int16 saveNum = argv[1];
	int16 version = argv[0];

	if (saveNum > 999)
		return 1;

	Common::String filename = _vm->getSavegameFilename(saveNum);

	return _vm->_dat->loadgame(filename.c_str(), version);
	
}

int16 ScriptFunctions::sfGetGameDescription(int16 argc, int16 *argv) {
	
	int16 descObjectIndex = argv[2];
	int16 saveNum = argv[1];
	/*int16 version = argv[0];*/
	Common::String description;

	if (saveNum > 999)
		return 1;

	Common::String filename = _vm->getSavegameFilename(saveNum);

	Object *obj = _vm->_dat->getObject(descObjectIndex);

	if (_vm->_dat->getSavegameDescription(filename.c_str(), description)) {
		obj->setString(description.c_str());
		return 0;
	} else {
		obj->setString("");
		return 1;
	}

}

int16 ScriptFunctions::sfShakeScreen(int16 argc, int16 *argv) {
	// TODO: Used in RTZ
	warning("Unimplemented opcode: sfShakeScreen");
	return 0;
}

int16 ScriptFunctions::sfPlaceMenu(int16 argc, int16 *argv) {
	// Never used in LGOP2, RTZ, Manhole:NE
	warning("Unimplemented opcode: sfPlaceMenu");
	return 0;
}

int16 ScriptFunctions::sfSetSoundVolume(int16 argc, int16 *argv) {
	_vm->_mixer->setVolumeForSoundType(Audio::Mixer::kSFXSoundType, argv[0] * 25);
	_vm->_mixer->setVolumeForSoundType(Audio::Mixer::kSpeechSoundType, argv[0] * 25);
	return 0;
}

int16 ScriptFunctions::sfGetSynthType(int16 argc, int16 *argv) {
	// 0 = Default
	// 1 = PCSPKR
	// 2 = SBFM/ADLIB
	// 3 = ADLIBG
	// 4 = MT32MPU
	warning("Unimplemented opcode: sfGetSynthType");
	return 0;
}

int16 ScriptFunctions::sfIsSlowSystem(int16 argc, int16 *argv) {
	//warning("Unimplemented opcode: sfIsSlowSystem");
	// NOTE: In the original engine this value is set via a command-line parameter
	// I don't think it's needed here
	// One could maybe think about returning 1 here on actually slower systems.
	return 0;
}

} // End of namespace Made
