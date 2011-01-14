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

#include "mohawk/cursors.h"
#include "mohawk/graphics.h"
#include "mohawk/riven.h"
#include "mohawk/riven_external.h"
#include "mohawk/sound.h"
#include "mohawk/video.h"

#include "gui/message.h"
#include "common/events.h"

namespace Mohawk {

static const uint32 kDomeSliderDefaultState = 0x01F00000;
static const uint32 kDomeSliderSlotCount = 25;

RivenExternal::RivenExternal(MohawkEngine_Riven *vm) : _vm(vm) {
	setupCommands();
	_sliderState = kDomeSliderDefaultState;
}

RivenExternal::~RivenExternal() {
	for (uint32 i = 0; i < _externalCommands.size(); i++)
		delete _externalCommands[i];

	_externalCommands.clear();
}

void RivenExternal::setupCommands() {
	// aspit (Main Menu, Books, Setup) external commands
	COMMAND(xastartupbtnhide);
	COMMAND(xasetupcomplete);
	COMMAND(xaatrusopenbook);
	COMMAND(xaatrusbookback);
	COMMAND(xaatrusbookprevpage);
	COMMAND(xaatrusbooknextpage);
	COMMAND(xacathopenbook);
	COMMAND(xacathbookback);
	COMMAND(xacathbookprevpage);
	COMMAND(xacathbooknextpage);
	COMMAND(xtrapbookback);
	COMMAND(xatrapbookclose);
	COMMAND(xatrapbookopen);
	COMMAND(xarestoregame);
	COMMAND(xadisablemenureturn);
	COMMAND(xaenablemenureturn);
	COMMAND(xalaunchbrowser);
	COMMAND(xadisablemenuintro);
	COMMAND(xaenablemenuintro);
	COMMAND(xademoquit);
	COMMAND(xaexittomain);

	// bspit (Bookmaking Island) external commands
	COMMAND(xblabopenbook);
	COMMAND(xblabbookprevpage);
	COMMAND(xblabbooknextpage);
	COMMAND(xsoundplug);
	COMMAND(xbchangeboiler);
	COMMAND(xbupdateboiler);
	COMMAND(xbsettrap);
	COMMAND(xbcheckcatch);
	COMMAND(xbait);
	COMMAND(xbfreeytram);
	COMMAND(xbaitplate);
	COMMAND(xbisland190_opencard);
	COMMAND(xbisland190_resetsliders);
	COMMAND(xbisland190_slidermd);
	COMMAND(xbisland190_slidermw);
	COMMAND(xbscpbtn);
	COMMAND(xbisland_domecheck);
	COMMAND(xvalvecontrol);
	COMMAND(xbchipper);

	// gspit (Garden Island) external commands
	COMMAND(xgresetpins);
	COMMAND(xgrotatepins);
	COMMAND(xgpincontrols);
	COMMAND(xgisland25_opencard);
	COMMAND(xgisland25_resetsliders);
	COMMAND(xgisland25_slidermd);
	COMMAND(xgisland25_slidermw);
	COMMAND(xgscpbtn);
	COMMAND(xgisland1490_domecheck);
	COMMAND(xgplateau3160_dopools);
	COMMAND(xgwt200_scribetime);
	COMMAND(xgwt900_scribe);
	COMMAND(xgplaywhark);
	COMMAND(xgrviewer);
	COMMAND(xgwharksnd);
	COMMAND(xglview_prisonoff);
	COMMAND(xglview_villageoff);
	COMMAND(xglviewer);
	COMMAND(xglview_prisonon);
	COMMAND(xglview_villageon);

	// jspit (Jungle Island) external commands
	COMMAND(xreseticons);
	COMMAND(xicon);
	COMMAND(xcheckicons);
	COMMAND(xtoggleicon);
	COMMAND(xjtunnel103_pictfix);
	COMMAND(xjtunnel104_pictfix);
	COMMAND(xjtunnel105_pictfix);
	COMMAND(xjtunnel106_pictfix);
	COMMAND(xvga1300_carriage);
	COMMAND(xjdome25_resetsliders);
	COMMAND(xjdome25_slidermd);
	COMMAND(xjdome25_slidermw);
	COMMAND(xjscpbtn);
	COMMAND(xjisland3500_domecheck);
	COMMAND(xhandlecontroldown);
	COMMAND(xhandlecontrolmid);
	COMMAND(xhandlecontrolup);
	COMMAND(xjplaybeetle_550);
	COMMAND(xjplaybeetle_600);
	COMMAND(xjplaybeetle_950);
	COMMAND(xjplaybeetle_1050);
	COMMAND(xjplaybeetle_1450);
	COMMAND(xjlagoon700_alert);
	COMMAND(xjlagoon800_alert);
	COMMAND(xjlagoon1500_alert);
	COMMAND(xschool280_playwhark);
	COMMAND(xjschool280_resetleft);
	COMMAND(xjschool280_resetright);
	COMMAND(xjatboundary);

	// ospit (Gehn's Office) external commands
	COMMAND(xorollcredittime);
	COMMAND(xbookclick);
	COMMAND(xooffice30_closebook);
	COMMAND(xobedroom5_closedrawer);
	COMMAND(xogehnopenbook);
	COMMAND(xogehnbookprevpage);
	COMMAND(xogehnbooknextpage);
	COMMAND(xgwatch);

	// pspit (Prison Island) external commands
	COMMAND(xpisland990_elevcombo);
	COMMAND(xpscpbtn);
	COMMAND(xpisland290_domecheck);
	COMMAND(xpisland25_opencard);
	COMMAND(xpisland25_resetsliders);
	COMMAND(xpisland25_slidermd);
	COMMAND(xpisland25_slidermw);

	// rspit (Rebel Age) external commands
	COMMAND(xrshowinventory);
	COMMAND(xrhideinventory);
	COMMAND(xrcredittime);
	COMMAND(xrwindowsetup);

	// tspit (Temple Island) external commands
	COMMAND(xtexterior300_telescopedown);
	COMMAND(xtexterior300_telescopeup);
	COMMAND(xtisland390_covercombo);
	COMMAND(xtatrusgivesbooks);
	COMMAND(xtchotakesbook);
	COMMAND(xthideinventory);
	COMMAND(xt7500_checkmarbles);
	COMMAND(xt7600_setupmarbles);
	COMMAND(xt7800_setup);
	COMMAND(xdrawmarbles);
	COMMAND(xtakeit);
	COMMAND(xtscpbtn);
	COMMAND(xtisland4990_domecheck);
	COMMAND(xtisland5056_opencard);
	COMMAND(xtisland5056_resetsliders);
	COMMAND(xtisland5056_slidermd);
	COMMAND(xtisland5056_slidermw);
	COMMAND(xtatboundary);

	// Common external commands
	COMMAND(xflies);
}

void RivenExternal::runCommand(uint16 argc, uint16 *argv) {
	Common::String externalCommandName = _vm->getName(ExternalCommandNames, argv[0]);

	for (uint16 i = 0; i < _externalCommands.size(); i++)
		if (externalCommandName == _externalCommands[i]->desc) {
			debug(0, "Running Riven External Command \'%s\'", externalCommandName.c_str());
			(this->*(_externalCommands[i]->proc)) (argv[1], argv[1] ? argv + 2 : NULL);
			return;
		}

	error("Unknown external command \'%s\'", externalCommandName.c_str());
}

void RivenExternal::runDemoBoundaryDialog() {
	GUI::MessageDialog dialog("Exploration beyond this point available only within the full version of\n"
							  "the game.");
	dialog.runModal();
}

void RivenExternal::runEndGame(uint16 video) {
	_vm->_sound->stopAllSLST();
	_vm->_video->playMovie(video);
	runCredits(video);
}

void RivenExternal::runCredits(uint16 video) {
	// TODO: Play until the last frame and then run the credits

	VideoHandle videoHandle = _vm->_video->findVideoHandleRiven(video);

	while (!_vm->_video->endOfVideo(videoHandle) && !_vm->shouldQuit()) {
		if (_vm->_video->updateBackgroundMovies())
			_vm->_system->updateScreen();

		Common::Event event;
		while (_vm->_system->getEventManager()->pollEvent(event))
			;

		_vm->_system->delayMillis(10);
	}

	_vm->setGameOver();
}

void RivenExternal::runDomeButtonMovie() {
	// This command just plays the video of the button moving down and up.
	_vm->_video->playMovieBlocking(2);
}

void RivenExternal::runDomeCheck() {
	// Check if we clicked while the golden frame was showing

	VideoHandle video = _vm->_video->findVideoHandleRiven(1);
	assert(video != NULL_VID_HANDLE);

	int32 curFrame = _vm->_video->getCurFrame(video);
	int32 frameCount = _vm->_video->getFrameCount(video);

	// The final frame of the video is the 'golden' frame (double meaning: the
	// frame that is the magic one is the one with the golden symbol) but we
	// give a 3 frame leeway in either direction.
	if (frameCount - curFrame < 3 || curFrame < 3)
		*_vm->getVar("domecheck") = 1;
}

void RivenExternal::resetDomeSliders(uint16 soundId, uint16 startHotspot) {
	// The rightmost slider should move left until it finds the next slider,
	// then those two continue until they find the third slider. This continues
	// until all five sliders have returned their starting slots.
	byte slidersFound = 0;
	for (uint32 i = 0; i < kDomeSliderSlotCount; i++) {
		if (_sliderState & (1 << i)) {
			// A slider occupies this spot. Increase the number of sliders we
			// have found, but we're not doing any moving this iteration.
			slidersFound++;
		} else {
			// Move all the sliders we have found over one slot
			for (byte j = 0; j < slidersFound; j++) {
				_sliderState &= ~(1 << (i - j - 1));
				_sliderState |= 1 << (i - j);
			}

			// If we have at least one found slider, it has now moved
			// so we should redraw and play a tick sound
			if (slidersFound) {
				_vm->_sound->playSound(soundId);
				drawDomeSliders(startHotspot);
				_vm->_system->delayMillis(100);
			}
		}
	}

	// Sanity checks - the slider count should always be 5 and we should end up at
	// the default state after moving them all over.
	assert(slidersFound == 5);
	assert(_sliderState == kDomeSliderDefaultState);
}

void RivenExternal::checkDomeSliders(uint16 resetSlidersHotspot, uint16 openDomeHotspot) {
	// Let's see if we're all matched up...
	if (*_vm->getVar("adomecombo") == _sliderState) {
		// Set the button hotspot to the open dome hotspot
		_vm->_hotspots[resetSlidersHotspot].enabled = false;
		_vm->_hotspots[openDomeHotspot].enabled = true;
	} else {
		// Set the button hotspot to the reset sliders hotspot
		_vm->_hotspots[resetSlidersHotspot].enabled = true;
		_vm->_hotspots[openDomeHotspot].enabled = false;
	}
}

void RivenExternal::checkSliderCursorChange(uint16 startHotspot) {
	// Set the cursor based on _sliderState and what hotspot we're over
	for (uint16 i = 0; i < kDomeSliderSlotCount; i++) {
		if (_vm->_hotspots[i + startHotspot].rect.contains(_vm->_system->getEventManager()->getMousePos())) {
			if (_sliderState & (1 << (24 - i)))
				_vm->_cursor->setCursor(kRivenOpenHandCursor);
			else
				_vm->_cursor->setCursor(kRivenMainCursor);
			break;
		}
	}
}

void RivenExternal::dragDomeSlider(uint16 soundId, uint16 resetSlidersHotspot, uint16 openDomeHotspot, uint16 startHotspot) {
	int16 foundSlider = -1;

	for (uint16 i = 0; i < kDomeSliderSlotCount; i++) {
		if (_vm->_hotspots[i + startHotspot].rect.contains(_vm->_system->getEventManager()->getMousePos())) {
			// If the slider is not at this hotspot, we can't do anything else
			if (!(_sliderState & (1 << (24 - i))))
				return;

			foundSlider = i;
			break;
		}
	}

	// We're not over any slider
	if (foundSlider < 0)
		return;

	// We've clicked down, so show the closed hand cursor
	_vm->_cursor->setCursor(kRivenClosedHandCursor);

	bool done = false;
	while (!done) {
		Common::Event event;
		while (_vm->_system->getEventManager()->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_MOUSEMOVE:
				if (foundSlider < 24 && !(_sliderState & (1 << (23 - foundSlider))) && _vm->_hotspots[foundSlider + startHotspot + 1].rect.contains(event.mouse)) {
					// We've moved the slider right one space
					_sliderState &= ~(_sliderState & (1 << (24 - foundSlider)));
					foundSlider++;
					_sliderState |= 1 << (24 - foundSlider);

					// Now play a click sound and redraw
					_vm->_sound->playSound(soundId);
					drawDomeSliders(startHotspot);
				} else if (foundSlider > 0 && !(_sliderState & (1 << (25 - foundSlider))) && _vm->_hotspots[foundSlider + startHotspot - 1].rect.contains(event.mouse)) {
					// We've moved the slider left one space
					_sliderState &= ~(_sliderState & (1 << (24 - foundSlider)));
					foundSlider--;
					_sliderState |= 1 << (24 - foundSlider);

					// Now play a click sound and redraw
					_vm->_sound->playSound(soundId);
					drawDomeSliders(startHotspot);
				} else
					_vm->_system->updateScreen(); // A normal update for the cursor
				break;
			case Common::EVENT_LBUTTONUP:
				done = true;
				break;
			default:
				break;
			}
		}
		_vm->_system->delayMillis(10);
	}

	// Check to see if we have the right combination
	checkDomeSliders(resetSlidersHotspot, openDomeHotspot);
}

void RivenExternal::drawDomeSliders(uint16 startHotspot) {
	Common::Rect dstAreaRect = Common::Rect(200, 250, 420, 319);

	// On pspit, the rect is different by two pixels
	// (alternatively, we could just use hotspot 3 here, but only on pspit is there a hotspot for this)
	if (_vm->getCurStack() == pspit)
		dstAreaRect.translate(-2, 0);

	// Find out bitmap id
	uint16 bitmapId = _vm->findResourceID(ID_TBMP, "*sliders*");

	for (uint16 i = 0; i < kDomeSliderSlotCount; i++) {
		Common::Rect srcRect = _vm->_hotspots[startHotspot + i].rect;
		srcRect.translate(-dstAreaRect.left, -dstAreaRect.top); // Adjust the rect so it's in the destination area

		Common::Rect dstRect = _vm->_hotspots[startHotspot + i].rect;

		if (_sliderState & (1 << (24 - i)))
			_vm->_gfx->drawImageRect(bitmapId, srcRect, dstRect);
		else
			_vm->_gfx->drawImageRect(bitmapId + 1, srcRect, dstRect);
	}

	_vm->_gfx->updateScreen();
}

// ------------------------------------------------------------------------------------
// aspit (Main Menu, Books, Setup) external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xastartupbtnhide(uint16 argc, uint16 *argv) {
	// The original game hides the start/setup buttons depending on an ini entry.
	// It's safe to ignore this command.
}

void RivenExternal::xasetupcomplete(uint16 argc, uint16 *argv) {
	// The original game sets an ini entry to disable the setup button and use the
	// start button only. It's safe to ignore this part of the command.
	_vm->_sound->stopSound();
	_vm->changeToCard(1);
}

void RivenExternal::xaatrusopenbook(uint16 argc, uint16 *argv) {
	// Get the variable
	uint32 page = *_vm->getVar("aatruspage");

	// Set hotspots depending on the page
	if (page == 1) {
		_vm->_hotspots[1].enabled = false;
		_vm->_hotspots[2].enabled = false;
		_vm->_hotspots[3].enabled = true;
	} else {
		_vm->_hotspots[1].enabled = true;
		_vm->_hotspots[2].enabled = true;
		_vm->_hotspots[3].enabled = false;
	}

	// Draw the image of the page
	_vm->_gfx->drawPLST(page);
}

void RivenExternal::xaatrusbookback(uint16 argc, uint16 *argv) {
	// Return to where we were before entering the book
	_vm->changeToStack(*_vm->getVar("returnstackid"));
	_vm->changeToCard(*_vm->getVar("returncardid"));
}

void RivenExternal::xaatrusbookprevpage(uint16 argc, uint16 *argv) {
	// Get the page variable
	uint32 *page = _vm->getVar("aatruspage");

	// Decrement the page if it's not the first page
	if (*page == 1)
		return;
	(*page)--;

	// Play the page turning sound
	if (_vm->getFeatures() & GF_DEMO)
		_vm->_sound->playSound(4);
	else
		_vm->_sound->playSound(3);

	// Now update the screen :)
	_vm->_gfx->scheduleTransition(1);
	_vm->_gfx->updateScreen();
}

void RivenExternal::xaatrusbooknextpage(uint16 argc, uint16 *argv) {
	// Get the page variable
	uint32 *page = _vm->getVar("aatruspage");

	// Increment the page if it's not the last page
	if (((_vm->getFeatures() & GF_DEMO) && *page == 6) || *page == 10)
		return;
	(*page)++;

	// Play the page turning sound
	if (_vm->getFeatures() & GF_DEMO)
		_vm->_sound->playSound(5);
	else
		_vm->_sound->playSound(4);

	// Now update the screen :)
	_vm->_gfx->scheduleTransition(0);
	_vm->_gfx->updateScreen();
}

void RivenExternal::xacathopenbook(uint16 argc, uint16 *argv) {
	// Get the variable
	uint32 page = *_vm->getVar("acathpage");

	// Set hotspots depending on the page
	if (page == 1) {
		_vm->_hotspots[1].enabled = false;
		_vm->_hotspots[2].enabled = false;
		_vm->_hotspots[3].enabled = true;
	} else {
		_vm->_hotspots[1].enabled = true;
		_vm->_hotspots[2].enabled = true;
		_vm->_hotspots[3].enabled = false;
	}

	// Draw the image of the page
	_vm->_gfx->drawPLST(page);

	// Draw the white page edges
	if (page > 1 && page < 5)
		_vm->_gfx->drawPLST(50);
	else if (page > 5)
		_vm->_gfx->drawPLST(51);

	if (page == 28) {
		// Draw the telescope combination
		// The images for the numbers are tBMP's 13 through 17.
		// The start point is at (156, 247)
		uint32 teleCombo = *_vm->getVar("tcorrectorder");
		static const uint16 kNumberWidth = 32;
		static const uint16 kNumberHeight = 25;
		static const uint16 kDstX = 156;
		static const uint16 kDstY = 247;

		for (byte i = 0; i < 5; i++) {
			uint16 offset = (getComboDigit(teleCombo, i) - 1) * kNumberWidth;
			Common::Rect srcRect = Common::Rect(offset, 0, offset + kNumberWidth, kNumberHeight);
			Common::Rect dstRect = Common::Rect(i * kNumberWidth + kDstX, kDstY, (i + 1) * kNumberWidth + kDstX, kDstY + kNumberHeight);
			_vm->_gfx->drawImageRect(i + 13, srcRect, dstRect);
		}
	}
}

void RivenExternal::xacathbookback(uint16 argc, uint16 *argv) {
	// Return to where we were before entering the book
	_vm->changeToStack(*_vm->getVar("returnstackid"));
	_vm->changeToCard(*_vm->getVar("returncardid"));
}

void RivenExternal::xacathbookprevpage(uint16 argc, uint16 *argv) {
	// Get the variable
	uint32 *page = _vm->getVar("acathpage");

	// Increment the page if it's not the first page
	if (*page == 1)
		return;
	(*page)--;

	// Play the page turning sound
	_vm->_sound->playSound(5);

	// Now update the screen :)
	_vm->_gfx->scheduleTransition(3);
	_vm->_gfx->updateScreen();
}

void RivenExternal::xacathbooknextpage(uint16 argc, uint16 *argv) {
	// Get the variable
	uint32 *page = _vm->getVar("acathpage");

	// Increment the page if it's not the last page
	if (*page == 49)
		return;
	(*page)++;

	// Play the page turning sound
	_vm->_sound->playSound(6);

	// Now update the screen :)
	_vm->_gfx->scheduleTransition(2);
	_vm->_gfx->updateScreen();
}

void RivenExternal::xtrapbookback(uint16 argc, uint16 *argv) {
	// Return to where we were before entering the book
	*_vm->getVar("atrap") = 0;
	_vm->changeToStack(*_vm->getVar("returnstackid"));
	_vm->changeToCard(*_vm->getVar("returncardid"));
}

void RivenExternal::xatrapbookclose(uint16 argc, uint16 *argv) {
	// Close the trap book
	*_vm->getVar("atrap") = 0;

	// Play the page turning sound
	_vm->_sound->playSound(8);

	_vm->refreshCard();
}

void RivenExternal::xatrapbookopen(uint16 argc, uint16 *argv) {
	// Open the trap book
	*_vm->getVar("atrap") = 1;

	// Play the page turning sound
	_vm->_sound->playSound(9);

	_vm->refreshCard();
}

void RivenExternal::xarestoregame(uint16 argc, uint16 *argv) {
	// Launch the load game dialog
	_vm->runLoadDialog();
}

void RivenExternal::xadisablemenureturn(uint16 argc, uint16 *argv) {
	// This function would normally enable the Windows menu item for
	// returning to the main menu. Ctrl+r will do this instead.
	// The original also had this shortcut.
}

void RivenExternal::xaenablemenureturn(uint16 argc, uint16 *argv) {
	// This function would normally enable the Windows menu item for
	// returning to the main menu. Ctrl+r will do this instead.
	// The original also had this shortcut.
}

void RivenExternal::xalaunchbrowser(uint16 argc, uint16 *argv) {
	// Well, we can't launch a browser for obvious reasons ;)
	// The original text is as follows (for reference):

	// If you have an auto-dial configured connection to the Internet,
	// please select YES below.
	//
	// America Online and CompuServe users may experience difficulty. If
	// you find that you are unable to connect, please quit the Riven
	// Demo, launch your browser and type in the following URL:
	//
	//     www.redorb.com/buyriven
	//
	// Would you like to attempt to make the connection?
	//
	// [YES] [NO]

	GUI::MessageDialog dialog("At this point, the Riven Demo would\n"
							  "ask if you would like to open a web browser\n"
							  "to bring you to the Red Orb store to buy\n"
							  "the game. ScummVM cannot do that and\n"
							  "the site no longer exists.");
	dialog.runModal();
}

void RivenExternal::xadisablemenuintro(uint16 argc, uint16 *argv) {
	// This function would normally enable the Windows menu item for
	// playing the intro. Ctrl+p will play the intro movies instead.
	// The original also had this shortcut.

	// Hide the "exit" button here
	_vm->_gfx->hideInventory();
}

void RivenExternal::xaenablemenuintro(uint16 argc, uint16 *argv) {
	// This function would normally enable the Windows menu item for
	// playing the intro. Ctrl+p will play the intro movies instead.
	// The original also had this shortcut.

	// Show the "exit" button here
	_vm->_gfx->showInventory();
}

void RivenExternal::xademoquit(uint16 argc, uint16 *argv) {
	// Exactly as it says on the tin. In the demo, this function quits.
	_vm->setGameOver();
}

void RivenExternal::xaexittomain(uint16 argc, uint16 *argv) {
	// One could potentially implement this function, but there would be no
	// point. This function is only used in the demo's aspit card 9 update
	// screen script. However, card 9 is not accessible from the game without
	// jumping to the card and there's nothing going on in the card so it
	// never gets called. There's also no card 9 in the full game, so the
	// functionality of this card was likely removed before release. The
	// demo executable references some other external commands relating to
	// setting and getting the volume, as well as drawing the volume. I'd
	// venture to guess that this would have been some sort of options card
	// replaced with the Windows/Mac API in the final product.
	//
	// Yeah, this function is just dummied and holds a big comment ;)
}

// ------------------------------------------------------------------------------------
// bspit (Bookmaking Island) external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xblabopenbook(uint16 argc, uint16 *argv) {
	// Get the variable
	uint32 page = *_vm->getVar("blabbook");

	// Draw the image of the page based on the blabbook variable
	_vm->_gfx->drawPLST(page);

	if (page == 14) {
		// Draw the dome combination
		// The images for the numbers are tBMP's 364 through 368
		// The start point is at (240, 82)
		uint32 domeCombo = *_vm->getVar("adomecombo");
		static const uint16 kNumberWidth = 32;
		static const uint16 kNumberHeight = 24;
		static const uint16 kDstX = 240;
		static const uint16 kDstY = 82;
		byte numCount = 0;

		for (int bitPos = 24; bitPos >= 0; bitPos--) {
			if (domeCombo & (1 << bitPos)) {
				uint16 offset = (24 - bitPos) * kNumberWidth;
				Common::Rect srcRect = Common::Rect(offset, 0, offset + kNumberWidth, kNumberHeight);
				Common::Rect dstRect = Common::Rect(numCount * kNumberWidth + kDstX, kDstY, (numCount + 1) * kNumberWidth + kDstX, kDstY + kNumberHeight);
				_vm->_gfx->drawImageRect(numCount + 364, srcRect, dstRect);
				numCount++;
			}
		}

		assert(numCount == 5); // Sanity check
	}
}

void RivenExternal::xblabbookprevpage(uint16 argc, uint16 *argv) {
	// Get the page variable
	uint32 *page = _vm->getVar("blabbook");

	// Decrement the page if it's not the first page
	if (*page == 1)
		return;
	(*page)--;

	// Play the page turning sound
	_vm->_sound->playSound(22);

	// Now update the screen :)
	_vm->_gfx->scheduleTransition(1);
	_vm->_gfx->updateScreen();
}

void RivenExternal::xblabbooknextpage(uint16 argc, uint16 *argv) {
	// Get the page variable
	uint32 *page = _vm->getVar("blabbook");

	// Increment the page if it's not the last page
	if (*page == 22)
		return;
	(*page)++;

	// Play the page turning sound
	_vm->_sound->playSound(23);

	// Now update the screen :)
	_vm->_gfx->scheduleTransition(0);
	_vm->_gfx->updateScreen();
}

void RivenExternal::xsoundplug(uint16 argc, uint16 *argv) {
	uint32 heat = *_vm->getVar("bheat");
	uint32 boilerInactive = *_vm->getVar("bcratergg");

	if (heat != 0)
		_vm->_sound->playSLST(1, _vm->getCurCard());
	else if (boilerInactive != 0)
		_vm->_sound->playSLST(2, _vm->getCurCard());
	else
		_vm->_sound->playSLST(3, _vm->getCurCard());
}

void RivenExternal::xbchangeboiler(uint16 argc, uint16 *argv) {
	uint32 heat = *_vm->getVar("bheat");
	uint32 water = *_vm->getVar("bblrwtr");
	uint32 platform = *_vm->getVar("bblrgrt");

	// Stop any background videos
	_vm->_video->stopVideos();

	if (argv[0] == 1) {
		// Water is filling/draining from the boiler
		if (water == 0) {
			if (platform == 1)
				_vm->_video->activateMLST(12, _vm->getCurCard());
			else
				_vm->_video->activateMLST(10, _vm->getCurCard());
		} else if (heat == 1) {
			if (platform == 1)
				_vm->_video->activateMLST(22, _vm->getCurCard());
			else
				_vm->_video->activateMLST(19, _vm->getCurCard());
		} else {
			if (platform == 1)
				_vm->_video->activateMLST(16, _vm->getCurCard());
			else
				_vm->_video->activateMLST(13, _vm->getCurCard());
		}
	} else if (argv[0] == 2 && water != 0) {
		if (heat == 1) {
			// Turning on the heat
			if (platform == 1)
				_vm->_video->activateMLST(23, _vm->getCurCard());
			else
				_vm->_video->activateMLST(20, _vm->getCurCard());
		} else {
			// Turning off the heat
			if (platform == 1)
				_vm->_video->activateMLST(18, _vm->getCurCard());
			else
				_vm->_video->activateMLST(15, _vm->getCurCard());
		}
	} else if (argv[0] == 3) {
		if (platform == 1) {
			// Lowering the platform
			if (water == 1) {
				if (heat == 1)
					_vm->_video->activateMLST(24, _vm->getCurCard());
				else
					_vm->_video->activateMLST(17, _vm->getCurCard());
			} else
				_vm->_video->activateMLST(11, _vm->getCurCard());
		} else {
			// Raising the platform
			if (water == 1) {
				if (heat == 1)
					_vm->_video->activateMLST(21, _vm->getCurCard());
				else
					_vm->_video->activateMLST(14, _vm->getCurCard());
			} else
				_vm->_video->activateMLST(9, _vm->getCurCard());
		}
	}

	if (argc > 1)
		_vm->_sound->playSLST(argv[1], _vm->getCurCard());
	else if (argv[0] == 2)
		_vm->_sound->playSLST(1, _vm->getCurCard());

	_vm->_cursor->setCursor(kRivenHideCursor);
	_vm->_video->playMovieBlocking(11);
}

void RivenExternal::xbupdateboiler(uint16 argc, uint16 *argv) {
	uint32 heat = *_vm->getVar("bheat");
	uint32 platform = *_vm->getVar("bblrgrt");

	if (heat) {
		if (platform == 0) {
			_vm->_video->activateMLST(8, _vm->getCurCard());
			_vm->_video->playMovie(8);
		} else {
			_vm->_video->activateMLST(7, _vm->getCurCard());
			_vm->_video->playMovie(7);
		}
	} else {
		_vm->_video->disableMovie(7);
		_vm->_video->disableMovie(8);
	}
}

void RivenExternal::xbsettrap(uint16 argc, uint16 *argv) {
	// TODO: Set the Ytram trap
}

void RivenExternal::xbcheckcatch(uint16 argc, uint16 *argv) {
	// TODO: Check if we've caught a Ytram
}

void RivenExternal::xbait(uint16 argc, uint16 *argv) {
	// Set the cursor to the pellet
	_vm->_cursor->setCursor(kRivenPelletCursor);

	// Loop until the player lets go (or quits)
	Common::Event event;
	bool mouseDown = true;
	while (mouseDown) {
		while (_vm->_system->getEventManager()->pollEvent(event)) {
			if (event.type == Common::EVENT_LBUTTONUP)
				mouseDown = false;
			else if (event.type == Common::EVENT_MOUSEMOVE)
				_vm->_system->updateScreen();
			else if (event.type == Common::EVENT_QUIT || event.type == Common::EVENT_RTL)
				return;
		}

		_vm->_system->delayMillis(10); // Take it easy on the CPU
	}

	// Set back the cursor
	_vm->_cursor->setCursor(kRivenMainCursor);

	// Set the bait if we put it on the plate
	if (_vm->_hotspots[9].rect.contains(_vm->_system->getEventManager()->getMousePos())) {
		*_vm->getVar("bbait") = 1;
		_vm->_gfx->drawPLST(4);
		_vm->_gfx->updateScreen();
		_vm->_hotspots[3].enabled = false; // Disable bait hotspot
		_vm->_hotspots[9].enabled = true; // Enable baitplate hotspot
	}
}

void RivenExternal::xbfreeytram(uint16 argc, uint16 *argv) {
	// TODO: Play a random Ytram movie
}

void RivenExternal::xbaitplate(uint16 argc, uint16 *argv) {
	// Remove the pellet from the plate and put it in your hand
	_vm->_gfx->drawPLST(3);
	_vm->_gfx->updateScreen();
	_vm->_cursor->setCursor(kRivenPelletCursor);

	// Loop until the player lets go (or quits)
	Common::Event event;
	bool mouseDown = true;
	while (mouseDown) {
		while (_vm->_system->getEventManager()->pollEvent(event)) {
			if (event.type == Common::EVENT_LBUTTONUP)
				mouseDown = false;
			else if (event.type == Common::EVENT_MOUSEMOVE)
				_vm->_system->updateScreen();
			else if (event.type == Common::EVENT_QUIT || event.type == Common::EVENT_RTL)
				return;
		}

		_vm->_system->delayMillis(10); // Take it easy on the CPU
	}

	// Set back the cursor
	_vm->_cursor->setCursor(kRivenMainCursor);

	// Set the bait if we put it on the plate, remove otherwise
	if (_vm->_hotspots[9].rect.contains(_vm->_system->getEventManager()->getMousePos())) {
		*_vm->getVar("bbait") = 1;
		_vm->_gfx->drawPLST(4);
		_vm->_gfx->updateScreen();
		_vm->_hotspots[3].enabled = false; // Disable bait hotspot
		_vm->_hotspots[9].enabled = true; // Enable baitplate hotspot
	} else {
		*_vm->getVar("bbait") = 0;
		_vm->_hotspots[3].enabled = true; // Enable bait hotspot
		_vm->_hotspots[9].enabled = false; // Disable baitplate hotspot
	}
}

void RivenExternal::xbisland190_opencard(uint16 argc, uint16 *argv) {
	checkDomeSliders(27, 28);
}

void RivenExternal::xbisland190_resetsliders(uint16 argc, uint16 *argv) {
	resetDomeSliders(41, 2);
}

void RivenExternal::xbisland190_slidermd(uint16 argc, uint16 *argv) {
	dragDomeSlider(41, 27, 28, 2);
}

void RivenExternal::xbisland190_slidermw(uint16 argc, uint16 *argv) {
	checkSliderCursorChange(2);
}

void RivenExternal::xbscpbtn(uint16 argc, uint16 *argv) {
	runDomeButtonMovie();
}

void RivenExternal::xbisland_domecheck(uint16 argc, uint16 *argv) {
	runDomeCheck();
}

void RivenExternal::xvalvecontrol(uint16 argc, uint16 *argv) {
	Common::Point startPos = _vm->_system->getEventManager()->getMousePos();

	// Get the variable for the valve
	uint32 *valve = _vm->getVar("bvalve");

	int changeX = 0;
	int changeY = 0;
	bool done = false;

	// Set the cursor to the closed position
	_vm->_cursor->setCursor(kRivenClosedHandCursor);
	_vm->_system->updateScreen();

	while (!done) {
		Common::Event event;

		while (_vm->_system->getEventManager()->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_MOUSEMOVE:
				changeX = event.mouse.x - startPos.x;
				changeY = startPos.y - event.mouse.y;
				_vm->_system->updateScreen();
				break;
			case Common::EVENT_LBUTTONUP:
				// FIXME: These values for changes in x/y could be tweaked.
				if (*valve == 0 && changeY <= -10) {
					*valve = 1;
					_vm->_cursor->setCursor(kRivenHideCursor);
					_vm->_video->playMovieBlocking(2);
					_vm->refreshCard();
				} else if (*valve == 1) {
					if (changeX >= 0 && changeY >= 10) {
						*valve = 0;
						_vm->_cursor->setCursor(kRivenHideCursor);
						_vm->_video->playMovieBlocking(3);
						_vm->refreshCard();
					} else if (changeX <= -10 && changeY <= 10) {
						*valve = 2;
						_vm->_cursor->setCursor(kRivenHideCursor);
						_vm->_video->playMovieBlocking(1);
						_vm->refreshCard();
					}
				} else if (*valve == 2 && changeX >= 10) {
					*valve = 1;
					_vm->_cursor->setCursor(kRivenHideCursor);
					_vm->_video->playMovieBlocking(4);
					_vm->refreshCard();
				}
				done = true;
			default:
				break;
			}
		}
		_vm->_system->delayMillis(10);
	}

	// If we changed state and the new state is that the valve is flowing to
	// the boiler, we need to update the boiler state.
	if (*valve == 1) {
		if (*_vm->getVar("bidvlv") == 1) { // Check which way the water is going at the boiler
			if (*_vm->getVar("bblrarm") == 1) {
				// If the pipe is open, make sure the water is drained out
				*_vm->getVar("bheat") = 0;
				*_vm->getVar("bblrwtr") = 0;
			} else {
				// If the pipe is closed, fill the boiler again
				*_vm->getVar("bheat") = *_vm->getVar("bblrvalve");
				*_vm->getVar("bblrwtr") = 1;
			}
		} else {
			// Have the grating inside the boiler match the switch outside
			*_vm->getVar("bblrgrt") = (*_vm->getVar("bblrsw") == 1) ? 0 : 1;
		}
	}
}

void RivenExternal::xbchipper(uint16 argc, uint16 *argv) {
	// Why is this an external command....?
	if (*_vm->getVar("bvalve") == 2)
		_vm->_video->playMovieBlocking(2);
}

// ------------------------------------------------------------------------------------
// gspit (Garden Island) external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xgresetpins(uint16 argc, uint16 *argv) {
	// TODO: Map related
}

void RivenExternal::xgrotatepins(uint16 argc, uint16 *argv) {
	// TODO: Map related
}

void RivenExternal::xgpincontrols(uint16 argc, uint16 *argv) {
	// TODO: Map related
}

void RivenExternal::xgisland25_opencard(uint16 argc, uint16 *argv) {
	checkDomeSliders(28, 29);
}

void RivenExternal::xgisland25_resetsliders(uint16 argc, uint16 *argv) {
	resetDomeSliders(16, 2);
}

void RivenExternal::xgisland25_slidermd(uint16 argc, uint16 *argv) {
	dragDomeSlider(16, 28, 29, 2);
}

void RivenExternal::xgisland25_slidermw(uint16 argc, uint16 *argv) {
	checkSliderCursorChange(2);
}

void RivenExternal::xgscpbtn(uint16 argc, uint16 *argv) {
	runDomeButtonMovie();
}

void RivenExternal::xgisland1490_domecheck(uint16 argc, uint16 *argv) {
	runDomeCheck();
}

void RivenExternal::xgplateau3160_dopools(uint16 argc, uint16 *argv) {
	// Play the deactivation of a pool if one is active and a different one is activated
	_vm->_cursor->setCursor(kRivenHideCursor);
	_vm->_video->playMovieBlocking(*_vm->getVar("glkbtns") * 2);
}

void RivenExternal::xgwt200_scribetime(uint16 argc, uint16 *argv) {
	// Get the current time
	*_vm->getVar("gscribetime") = _vm->_system->getMillis();
}

void RivenExternal::xgwt900_scribe(uint16 argc, uint16 *argv) {
	uint32 *scribeVar = _vm->getVar("gscribe");

	if (*scribeVar == 1 && _vm->_system->getMillis() > *_vm->getVar("gscribetime") + 40000)
		*scribeVar = 2;
}

void RivenExternal::xgplaywhark(uint16 argc, uint16 *argv) {
	// TODO: Whark response to using the lights
}

void RivenExternal::xgrviewer(uint16 argc, uint16 *argv) {
	// TODO: Image viewer related
}

void RivenExternal::xgwharksnd(uint16 argc, uint16 *argv) {
	// TODO: Image viewer related
}

void RivenExternal::xglview_prisonoff(uint16 argc, uint16 *argv) {
	// TODO: Image viewer related
}

void RivenExternal::xglview_villageoff(uint16 argc, uint16 *argv) {
	// TODO: Image viewer related
}

void RivenExternal::xglviewer(uint16 argc, uint16 *argv) {
	// TODO: Image viewer related
}

void RivenExternal::xglview_prisonon(uint16 argc, uint16 *argv) {
	// TODO: Image viewer related
}

void RivenExternal::xglview_villageon(uint16 argc, uint16 *argv) {
	// TODO: Image viewer related
}

// ------------------------------------------------------------------------------------
// jspit (Jungle Island) external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xreseticons(uint16 argc, uint16 *argv) {
	// Reset the icons when going to Tay (rspit)
	*_vm->getVar("jicons") = 0;
	*_vm->getVar("jiconorder") = 0;
	*_vm->getVar("jrbook") = 0;
}

// Count up how many icons are pressed
static byte countDepressedIcons(uint32 iconOrderVar) {
	if (iconOrderVar >= (1 << 20))
		return 5;
	else if (iconOrderVar >= (1 << 15))
		return 4;
	else if (iconOrderVar >= (1 << 10))
		return 3;
	else if (iconOrderVar >= (1 << 5))
		return 2;
	else if (iconOrderVar >= (1 << 1))
		return 1;
	else
		return 0;
}

void RivenExternal::xicon(uint16 argc, uint16 *argv) {
	// Set atemp as the status of whether or not the icon can be depressed.
	if (*_vm->getVar("jicons") & (1 << (argv[0] - 1))) {
		// This icon is depressed. Allow depression only if the last depressed icon was this one.
		if ((*_vm->getVar("jiconorder") & 0x1f) == argv[0])
			*_vm->getVar("atemp") = 1;
		else
			*_vm->getVar("atemp") = 2;
	} else
		*_vm->getVar("atemp") = 0;
}

void RivenExternal::xcheckicons(uint16 argc, uint16 *argv) {
	// Reset the icons if this is the sixth icon
	uint32 *iconOrderVar = _vm->getVar("jiconorder");
	if (countDepressedIcons(*iconOrderVar) == 5) {
		*iconOrderVar = 0;
		*_vm->getVar("jicons") = 0;
		_vm->_sound->playSound(46);
	}
}

void RivenExternal::xtoggleicon(uint16 argc, uint16 *argv) {
	// Get the variables
	uint32 *iconsDepressed = _vm->getVar("jicons");
	uint32 *iconOrderVar = _vm->getVar("jiconorder");

	if (*iconsDepressed & (1 << (argv[0] - 1))) {
		// The icon is depressed, now unpress it
		*iconsDepressed &= ~(1 << (argv[0] - 1));
		*iconOrderVar >>= 5;
	} else {
		// The icon is not depressed, now depress it
		*iconsDepressed |= 1 << (argv[0] - 1);
		*iconOrderVar = (*iconOrderVar << 5) + argv[0];
	}

	// Check if the puzzle is complete now and assign 1 to jrbook if the puzzle is complete.
	if (*iconOrderVar == *_vm->getVar("jiconcorrectorder"))
		*_vm->getVar("jrbook") = 1;
}

void RivenExternal::xjtunnel103_pictfix(uint16 argc, uint16 *argv) {
	// Get the jicons variable which contains which of the stones are depressed in the rebel tunnel puzzle
	uint32 iconsDepressed = *_vm->getVar("jicons");

	// Now, draw which icons are depressed based on the bits of the variable
	if (iconsDepressed & (1 << 0))
		_vm->_gfx->drawPLST(2);
	if (iconsDepressed & (1 << 1))
		_vm->_gfx->drawPLST(3);
	if (iconsDepressed & (1 << 2))
		_vm->_gfx->drawPLST(4);
	if (iconsDepressed & (1 << 3))
		_vm->_gfx->drawPLST(5);
	if (iconsDepressed & (1 << 22))
		_vm->_gfx->drawPLST(6);
	if (iconsDepressed & (1 << 23))
		_vm->_gfx->drawPLST(7);
	if (iconsDepressed & (1 << 24))
		_vm->_gfx->drawPLST(8);
}

void RivenExternal::xjtunnel104_pictfix(uint16 argc, uint16 *argv) {
	// Get the jicons variable which contains which of the stones are depressed in the rebel tunnel puzzle
	uint32 iconsDepressed = *_vm->getVar("jicons");

	// Now, draw which icons are depressed based on the bits of the variable
	if (iconsDepressed & (1 << 9))
		_vm->_gfx->drawPLST(2);
	if (iconsDepressed & (1 << 10))
		_vm->_gfx->drawPLST(3);
	if (iconsDepressed & (1 << 11))
		_vm->_gfx->drawPLST(4);
	if (iconsDepressed & (1 << 12))
		_vm->_gfx->drawPLST(5);
	if (iconsDepressed & (1 << 13))
		_vm->_gfx->drawPLST(6);
	if (iconsDepressed & (1 << 14))
		_vm->_gfx->drawPLST(7);
	if (iconsDepressed & (1 << 15))
		_vm->_gfx->drawPLST(8);
	if (iconsDepressed & (1 << 16))
		_vm->_gfx->drawPLST(9);
}

void RivenExternal::xjtunnel105_pictfix(uint16 argc, uint16 *argv) {
	// Get the jicons variable which contains which of the stones are depressed in the rebel tunnel puzzle
	uint32 iconsDepressed = *_vm->getVar("jicons");

	// Now, draw which icons are depressed based on the bits of the variable
	if (iconsDepressed & (1 << 3))
		_vm->_gfx->drawPLST(2);
	if (iconsDepressed & (1 << 4))
		_vm->_gfx->drawPLST(3);
	if (iconsDepressed & (1 << 5))
		_vm->_gfx->drawPLST(4);
	if (iconsDepressed & (1 << 6))
		_vm->_gfx->drawPLST(5);
	if (iconsDepressed & (1 << 7))
		_vm->_gfx->drawPLST(6);
	if (iconsDepressed & (1 << 8))
		_vm->_gfx->drawPLST(7);
	if (iconsDepressed & (1 << 9))
		_vm->_gfx->drawPLST(8);
}

void RivenExternal::xjtunnel106_pictfix(uint16 argc, uint16 *argv) {
	// Get the jicons variable which contains which of the stones are depressed in the rebel tunnel puzzle
	uint32 iconsDepressed = *_vm->getVar("jicons");

	// Now, draw which icons are depressed based on the bits of the variable
	if (iconsDepressed & (1 << 16))
		_vm->_gfx->drawPLST(2);
	if (iconsDepressed & (1 << 17))
		_vm->_gfx->drawPLST(3);
	if (iconsDepressed & (1 << 18))
		_vm->_gfx->drawPLST(4);
	if (iconsDepressed & (1 << 19))
		_vm->_gfx->drawPLST(5);
	if (iconsDepressed & (1 << 20))
		_vm->_gfx->drawPLST(6);
	if (iconsDepressed & (1 << 21))
		_vm->_gfx->drawPLST(7);
	if (iconsDepressed & (1 << 22))
		_vm->_gfx->drawPLST(8);
	if (iconsDepressed & (1 << 23))
		_vm->_gfx->drawPLST(9);
}

void RivenExternal::xvga1300_carriage(uint16 argc, uint16 *argv) {
	// Run the gallows's carriage

	_vm->_cursor->setCursor(kRivenHideCursor);         // Hide the cursor
	_vm->_video->playMovieBlocking(1);                 // Play handle movie
	_vm->_gfx->scheduleTransition(15);                 // Set pan down transition
	_vm->changeToCard(_vm->matchRMAPToCard(0x18e77));  // Change to card facing up
	_vm->_cursor->setCursor(kRivenHideCursor);         // Hide the cursor (again)
	_vm->_video->playMovieBlocking(4);                 // Play carriage beginning to drop
	_vm->_gfx->scheduleTransition(14);                 // Set pan up transition
	_vm->changeToCard(_vm->matchRMAPToCard(0x183a9));  // Change to card looking straight again
	_vm->_video->playMovieBlocking(2);

	uint32 *gallows = _vm->getVar("jgallows");
	if (*gallows == 1) {
		// If the gallows is open, play the up movie and return
		_vm->_video->playMovieBlocking(3);
		return;
	}

	// Give the player 5 seconds to click (anywhere)
	uint32 startTime = _vm->_system->getMillis();
	bool gotClick = false;
	while (_vm->_system->getMillis() - startTime <= 5000 && !gotClick) {
		Common::Event event;
		while (_vm->_system->getEventManager()->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_MOUSEMOVE:
				_vm->_system->updateScreen();
				break;
			case Common::EVENT_LBUTTONUP:
				gotClick = true;
				break;
			default:
				break;
			}
		}

		_vm->_system->delayMillis(10);
	}

	_vm->_cursor->setCursor(kRivenHideCursor);             // Hide the cursor

	if (gotClick) {
		_vm->_gfx->scheduleTransition(16);                 // Schedule dissolve transition
		_vm->changeToCard(_vm->matchRMAPToCard(0x18d4d));  // Move forward
		_vm->_cursor->setCursor(kRivenHideCursor);         // Hide the cursor
		_vm->_system->delayMillis(500);                    // Delay a half second before changing again
		_vm->_gfx->scheduleTransition(12);                 // Schedule pan left transition
		_vm->changeToCard(_vm->matchRMAPToCard(0x18ab5));  // Turn right
		_vm->_cursor->setCursor(kRivenHideCursor);         // Hide the cursor
		_vm->_video->playMovieBlocking(1);                 // Play carriage ride movie
		_vm->changeToCard(_vm->matchRMAPToCard(0x17167));  // We have arrived at the top
	} else
		_vm->_video->playMovieBlocking(3);                 // Too slow!
}

void RivenExternal::xjdome25_resetsliders(uint16 argc, uint16 *argv) {
	resetDomeSliders(81, 2);
}

void RivenExternal::xjdome25_slidermd(uint16 argc, uint16 *argv) {
	dragDomeSlider(81, 29, 28, 2);
}

void RivenExternal::xjdome25_slidermw(uint16 argc, uint16 *argv) {
	checkSliderCursorChange(2);
}

void RivenExternal::xjscpbtn(uint16 argc, uint16 *argv) {
	runDomeButtonMovie();
}

void RivenExternal::xjisland3500_domecheck(uint16 argc, uint16 *argv) {
	runDomeCheck();
}

int RivenExternal::jspitElevatorLoop() {
	Common::Point startPos = _vm->_system->getEventManager()->getMousePos();

	Common::Event event;
	int changeLevel = 0;

	_vm->_cursor->setCursor(kRivenClosedHandCursor);
	_vm->_system->updateScreen();
	for (;;) {
		while (_vm->_system->getEventManager()->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_MOUSEMOVE:
				if (event.mouse.y > (startPos.y + 10)) {
					changeLevel = -1;
				} else if (event.mouse.y < (startPos.y - 10)) {
					changeLevel = 1;
				} else {
					changeLevel = 0;
				}
				_vm->_system->updateScreen();
				break;
			case Common::EVENT_LBUTTONUP:
				_vm->_cursor->setCursor(kRivenMainCursor);
				_vm->_system->updateScreen();
				return changeLevel;
			default:
				break;
			}
		}
		_vm->_system->delayMillis(10);
	}
}

void RivenExternal::xhandlecontrolup(uint16 argc, uint16 *argv) {
	int changeLevel = jspitElevatorLoop();

	// If we've moved the handle down, go down a floor
	if (changeLevel == -1) {
		_vm->_video->playMovieBlocking(1);
		_vm->_video->playMovieBlocking(2);
		_vm->changeToCard(_vm->matchRMAPToCard(0x1e374));
	}
}

void RivenExternal::xhandlecontroldown(uint16 argc, uint16 *argv) {
	int changeLevel = jspitElevatorLoop();

	// If we've moved the handle up, go up a floor
	if (changeLevel == 1) {
		_vm->_video->playMovieBlocking(1);
		_vm->_video->playMovieBlocking(2);
		_vm->changeToCard(_vm->matchRMAPToCard(0x1e374));
	}
}

void RivenExternal::xhandlecontrolmid(uint16 argc, uint16 *argv) {
	int changeLevel = jspitElevatorLoop();

	if (changeLevel == 0)
		return;

	// Play the handle moving video
	if (changeLevel == 1)
		_vm->_video->playMovieBlocking(7);
	else
		_vm->_video->playMovieBlocking(6);

	// If the whark's mouth is open, close it
	uint32 *mouthVar = _vm->getVar("jwmouth");
	if (*mouthVar == 1) {
		_vm->_video->playMovieBlocking(3);
		_vm->_video->playMovieBlocking(8);
		*mouthVar = 0;
	}

	// Play the elevator video and then change the card
	if (changeLevel == 1) {
		_vm->_video->playMovieBlocking(5);
		_vm->changeToCard(_vm->matchRMAPToCard(0x1e597));
	} else {
		_vm->_video->playMovieBlocking(4);
		_vm->changeToCard(_vm->matchRMAPToCard(0x1e29c));
	}
}

void RivenExternal::xjplaybeetle_550(uint16 argc, uint16 *argv) {
	// Play a beetle animation 25% of the time
	*_vm->getVar("jplaybeetle") = (_vm->_rnd->getRandomNumberRng(0, 3) == 0) ? 1 : 0;
}

void RivenExternal::xjplaybeetle_600(uint16 argc, uint16 *argv) {
	// Play a beetle animation 25% of the time
	*_vm->getVar("jplaybeetle") = (_vm->_rnd->getRandomNumberRng(0, 3) == 0) ? 1 : 0;
}

void RivenExternal::xjplaybeetle_950(uint16 argc, uint16 *argv) {
	// Play a beetle animation 25% of the time
	*_vm->getVar("jplaybeetle") = (_vm->_rnd->getRandomNumberRng(0, 3) == 0) ? 1 : 0;
}

void RivenExternal::xjplaybeetle_1050(uint16 argc, uint16 *argv) {
	// Play a beetle animation 25% of the time
	*_vm->getVar("jplaybeetle") = (_vm->_rnd->getRandomNumberRng(0, 3) == 0) ? 1 : 0;
}

void RivenExternal::xjplaybeetle_1450(uint16 argc, uint16 *argv) {
	// Play a beetle animation 25% of the time as long as the girl is not present
	*_vm->getVar("jplaybeetle") = (_vm->_rnd->getRandomNumberRng(0, 3) == 0 && *_vm->getVar("jgirl") != 1) ? 1 : 0;
}

void RivenExternal::xjlagoon700_alert(uint16 argc, uint16 *argv) {
	// TODO: Sunner related
}

void RivenExternal::xjlagoon800_alert(uint16 argc, uint16 *argv) {
	// TODO: Sunner related
}

void RivenExternal::xjlagoon1500_alert(uint16 argc, uint16 *argv) {
	// Have the sunners move a bit as you get closer ;)
	uint32 *sunners = _vm->getVar("jsunners");
	if (*sunners == 0) {
		_vm->_video->playMovieBlocking(3);
	} else if (*sunners == 1) {
		_vm->_video->playMovieBlocking(2);
		*sunners = 2;
	}
}

void RivenExternal::xjschool280_resetleft(uint16 argc, uint16 *argv) {
	// Dummy function. This resets the unneeded video timing variable (dropLeftStart) in
	// the DVD version.
}

void RivenExternal::xjschool280_resetright(uint16 argc, uint16 *argv) {
	// Dummy function. This resets the unneeded video timing variable (dropRightStart) in
	// the DVD version.
}

void RivenExternal::redrawWharkNumberPuzzle(uint16 overlay, uint16 number) {
	// Update the screen for the whark number puzzle
	// We don't update the whole screen here because we don't want to overwrite the video data
	_vm->_gfx->drawPLST(overlay);
	_vm->_gfx->drawPLST(number + 1);
	_vm->_gfx->updateScreen(Common::Rect(80, 212, 477, 392));
	_vm->_system->updateScreen();
}

void RivenExternal::xschool280_playwhark(uint16 argc, uint16 *argv) {
	// The "monstrous" whark puzzle that teaches the number system

	uint32 *posVar;
	uint16 spinMLST, overlayPLST, doomMLST, snackMLST;

	// Choose left or right based on jwharkpos (which is set by the scripts)
	if (*_vm->getVar("jwharkpos") == 1) {
		posVar = _vm->getVar("jleftpos");
		spinMLST = 1;
		overlayPLST = 12;
		doomMLST = 3;
		snackMLST = 4;
	} else {
		posVar = _vm->getVar("jrightpos");
		spinMLST = 2;
		overlayPLST = 13;
		doomMLST = 5;
		snackMLST = 6;
	}

	// Hide the cursor
	_vm->_cursor->setCursor(kRivenHideCursor);

	// Play the spin movie
	_vm->_video->playMovieBlocking(spinMLST);

	// Get our random number and redraw the area
	uint16 number = _vm->_rnd->getRandomNumberRng(1, 10);
	redrawWharkNumberPuzzle(overlayPLST, number);

	// Handle movement
	// (11560/600)s is the length of each of the two movies. We divide it into 19 parts
	// (one for each of the possible positions the villager can have).
	VideoHandle handle = _vm->_video->playMovie(doomMLST);
	Graphics::VideoTimestamp startTime = Graphics::VideoTimestamp((11560 / 19) * (*posVar), 600);
	*posVar += number; // Adjust to the end
	Graphics::VideoTimestamp endTime = Graphics::VideoTimestamp((11560 / 19) * (*posVar), 600);
	_vm->_video->setVideoBounds(handle, startTime, endTime);
	_vm->_video->waitUntilMovieEnds(handle);

	if (*posVar > 19) {
		// The villager has died :(
		_vm->_video->playMovieBlocking(snackMLST);
		redrawWharkNumberPuzzle(overlayPLST, number);
		*posVar = 0;
	}

	// Enable the correct hotspots for the movement now
	_vm->_hotspots[2].enabled = !_vm->_hotspots[2].enabled;
	_vm->_hotspots[3].enabled = !_vm->_hotspots[3].enabled;

	// Update the cursor
	_vm->_curHotspot = -1;
	_vm->checkHotspotChange();
}

void RivenExternal::xjatboundary(uint16 argc, uint16 *argv) {
	runDemoBoundaryDialog();
}

// ------------------------------------------------------------------------------------
// ospit (Gehn's Office) external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xorollcredittime(uint16 argc, uint16 *argv) {
	// WORKAROUND: The special change stuff only handles one destination and it would
	// be messy to modify the way that currently works. If we use the trap book on Tay,
	// we should be using the Tay end game sequences.
	if (*_vm->getVar("returnstackid") == rspit) {
		_vm->changeToStack(rspit);
		_vm->changeToCard(2);
		return;
	}

	// You used the trap book... why? What were you thinking?
	uint32 *gehnState = _vm->getVar("agehn");

	if (*gehnState == 0)		// Gehn who?
		runEndGame(1);
	else if (*gehnState == 4)	// You freed him? Are you kidding me?
		runEndGame(2);
	else						// You already spoke with Gehn. What were you thinking?
		runEndGame(3);
}

void RivenExternal::xbookclick(uint16 argc, uint16 *argv) {
	// Hide the cursor
	_vm->_cursor->setCursor(kRivenHideCursor);

	// Let's hook onto our video
	VideoHandle video = _vm->_video->findVideoHandleRiven(argv[0]);

	// Convert from the standard QuickTime base time to milliseconds
	// The values are in terms of 1/600 of a second.
	// Have I said how much I just *love* QuickTime? </sarcasm>
	uint32 startTime = argv[1] * 1000 / 600;
	uint32 endTime = argv[2] * 1000 / 600;

	// Track down our hotspot
	// Of course, they're not in any sane order...
	static const uint16 hotspotMap[] = { 1, 3, 2, 0 };
	Common::Rect hotspotRect = _vm->_hotspots[hotspotMap[argv[3] - 1]].rect;

	debug(0, "xbookclick:");
	debug(0, "\tVideo Code = %d", argv[0]);
	debug(0, "\tStart Time = %dms", startTime);
	debug(0, "\tEnd Time   = %dms", endTime);
	debug(0, "\tHotspot    = %d -> %d", argv[3], hotspotMap[argv[3] - 1]);

	// Just let the video play while we wait until Gehn opens the trap book for us
	while (_vm->_video->getElapsedTime(video) < startTime && !_vm->shouldQuit()) {
		if (_vm->_video->updateBackgroundMovies())
			_vm->_system->updateScreen();

		Common::Event event;
		while (_vm->_system->getEventManager()->pollEvent(event))
			;

		_vm->_system->delayMillis(10);
	}

	// Break out if we're quitting
	if (_vm->shouldQuit())
		return;

	// Update our hotspot stuff
	if (hotspotRect.contains(_vm->_system->getEventManager()->getMousePos()))
		_vm->_cursor->setCursor(kRivenOpenHandCursor);
	else
		_vm->_cursor->setCursor(kRivenMainCursor);
	
	// OK, Gehn has opened the trap book and has asked us to go in. Let's watch
	// and see what the player will do...
	while (_vm->_video->getElapsedTime(video) < endTime && !_vm->shouldQuit()) {
		bool updateScreen = _vm->_video->updateBackgroundMovies();

		Common::Event event;
		while (_vm->_system->getEventManager()->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_MOUSEMOVE:
				if (hotspotRect.contains(_vm->_system->getEventManager()->getMousePos()))
					_vm->_cursor->setCursor(kRivenOpenHandCursor);
				else
					_vm->_cursor->setCursor(kRivenMainCursor);
				updateScreen = false; // Don't update twice, changing the cursor already updates the screen
				break;
			case Common::EVENT_LBUTTONUP:
				if (hotspotRect.contains(_vm->_system->getEventManager()->getMousePos())) {
					// OK, we've used the trap book! We go for ride lady!
					_vm->_scriptMan->stopAllScripts();                  // Stop all running scripts (so we don't remain in the cage)
					_vm->_video->stopVideos();                          // Stop all videos
					_vm->_cursor->setCursor(kRivenHideCursor);          // Hide the cursor
					_vm->_gfx->drawPLST(3);                             // Black out the screen
					_vm->_gfx->updateScreen();                          // Update the screen
					_vm->_sound->playSound(0);                          // Play the link sound
					_vm->_video->activateMLST(7, _vm->getCurCard());    // Activate Gehn Link Video
					_vm->_video->playMovieBlocking(1);                  // Play Gehn Link Video
					*_vm->getVar("agehn") = 4;                          // Set Gehn to the trapped state
					*_vm->getVar("atrapbook") = 1;                      // We've got the trap book again
					_vm->_sound->playSound(0);                          // Play the link sound again
					_vm->changeToCard(_vm->matchRMAPToCard(0x2885));    // Link out! (TODO: Shouldn't this card change?)
					return;
				}
				break;
			default:
				break;
			}
		}

		if (updateScreen && !_vm->shouldQuit())
			_vm->_system->updateScreen();

		_vm->_system->delayMillis(10);
	}

	// Break out if we're quitting
	if (_vm->shouldQuit())
		return;

	// Hide the cursor again
	_vm->_cursor->setCursor(kRivenHideCursor);

	// If there was no click and this is the third time Gehn asks us to
	// use the trap book, he will shoot the player. Dead on arrival.
	// Run the credits from here.
	if (*_vm->getVar("agehn") == 3) {
		_vm->_scriptMan->stopAllScripts();
		runCredits(argv[0]);
		return;
	}

	// There was no click, so just play the rest of the video.
	_vm->_video->waitUntilMovieEnds(video);
}

void RivenExternal::xooffice30_closebook(uint16 argc, uint16 *argv) {
	// Close the blank linking book if it's open
	uint32 *book = _vm->getVar("odeskbook");
	if (*book != 1)
		return;

	// Set the variable to be "closed"
	*book = 0;

	// Play the movie
	_vm->_video->playMovieBlocking(1);

	// Set the hotspots into their correct states
	_vm->_hotspots[2].enabled = false;
	_vm->_hotspots[5].enabled = false;
	_vm->_hotspots[6].enabled = true;

	// We now need to draw PLST 1 and refresh, but PLST 1 is
	// drawn when refreshing anyway, so don't worry about that.
	_vm->refreshCard();
}

void RivenExternal::xobedroom5_closedrawer(uint16 argc, uint16 *argv) {
	// Close the drawer if open when clicking on the journal.
	_vm->_video->playMovieBlocking(2);
	*_vm->getVar("ostanddrawer") = 0;
}

void RivenExternal::xogehnopenbook(uint16 argc, uint16 *argv) {
	_vm->_gfx->drawPLST(*_vm->getVar("ogehnpage"));
}

void RivenExternal::xogehnbookprevpage(uint16 argc, uint16 *argv) {
	// Get the page variable
	uint32 *page = _vm->getVar("ogehnpage");

	// Decrement the page if it's not the first page
	if (*page == 1)
		return;
	(*page)--;

	// Play the page turning sound
	_vm->_sound->playSound(12);

	// Now update the screen :)
	_vm->_gfx->scheduleTransition(1);
	_vm->_gfx->updateScreen();
}

void RivenExternal::xogehnbooknextpage(uint16 argc, uint16 *argv) {
	// Get the page variable
	uint32 *page = _vm->getVar("ogehnpage");

	// Increment the page if it's not the last page
	if (*page == 13)
		return;
	(*page)++;

	// Play the page turning sound
	_vm->_sound->playSound(13);

	// Now update the screen :)
	_vm->_gfx->scheduleTransition(0);
	_vm->_gfx->updateScreen();
}

uint16 RivenExternal::getComboDigit(uint32 correctCombo, uint32 digit) {
	static const uint32 powers[] = { 100000, 10000, 1000, 100, 10, 1 };
	return (correctCombo % powers[digit]) / powers[digit + 1];
}

void RivenExternal::xgwatch(uint16 argc, uint16 *argv) {
	// Hide the cursor
	_vm->_cursor->setCursor(kRivenHideCursor);

	uint32 *prisonCombo = _vm->getVar("pcorrectorder");
	uint32 soundTime = _vm->_system->getMillis() - 500; // Start the first sound instantly
	byte curSound = 0;

	while (!_vm->shouldQuit()) {
		// Play the next sound every half second
		if (_vm->_system->getMillis() - soundTime >= 500) {
			if (curSound == 5) // Break out after the last sound is done
				break;

			_vm->_sound->playSound(getComboDigit(*prisonCombo, curSound) + 13);
			curSound++;
			soundTime = _vm->_system->getMillis();
		}
		
		// Poll events just to check for quitting
		Common::Event event;
		while (_vm->_system->getEventManager()->pollEvent(event)) {}

		// Cut down on CPU usage
		_vm->_system->delayMillis(10);
	}

	// Now play the video for the watch
	_vm->_video->activateMLST(1, _vm->getCurCard());
	_vm->_video->playMovieBlocking(1);

	// And, finally, refresh
	_vm->refreshCard();
}

// ------------------------------------------------------------------------------------
// pspit (Prison Island) external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xpisland990_elevcombo(uint16 argc, uint16 *argv) {
	// Play button sound based on argv[0]
	_vm->_sound->playSound(argv[0] + 5);

	// It is impossible to get here if Gehn is not trapped. However,
	// the original also disallows brute forcing the ending if you have
	// not yet trapped Gehn.
	if (*_vm->getVar("agehn") != 4)
		return;

	uint32 *correctDigits = _vm->getVar("pelevcombo");

	// pelevcombo keeps count of how many buttons we have pressed in the correct order.
	// When pelevcombo is 5, clicking the handle will show the video freeing Catherine.
	if (*correctDigits < 5 && argv[0] == getComboDigit(*_vm->getVar("pcorrectorder"), *correctDigits))
		*correctDigits += 1;
	else
		*correctDigits = 0;
}

void RivenExternal::xpscpbtn(uint16 argc, uint16 *argv) {
	runDomeButtonMovie();
}

void RivenExternal::xpisland290_domecheck(uint16 argc, uint16 *argv) {
	runDomeCheck();
}

void RivenExternal::xpisland25_opencard(uint16 argc, uint16 *argv) {
	checkDomeSliders(31, 5);
}

void RivenExternal::xpisland25_resetsliders(uint16 argc, uint16 *argv) {
	resetDomeSliders(10, 6);
}

void RivenExternal::xpisland25_slidermd(uint16 argc, uint16 *argv) {
	dragDomeSlider(10, 31, 5, 6);
}

void RivenExternal::xpisland25_slidermw(uint16 argc, uint16 *argv) {
	checkSliderCursorChange(6);
}

// ------------------------------------------------------------------------------------
// rspit (Rebel Age) external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xrcredittime(uint16 argc, uint16 *argv) {
	// Nice going, you used the trap book on Tay.

	// The game chooses what ending based on agehn for us,
	// so we just have to play the video and credits.
	// For the record, when agehn == 4, Gehn will thank you for
	// showing him the rebel age and then leave you to die.
	// Otherwise, the rebels burn the book. Epic fail either way.
	runEndGame(1);
}

void RivenExternal::xrshowinventory(uint16 argc, uint16 *argv) {
	// Give the trap book and Catherine's journal to the player
	*_vm->getVar("atrapbook") = 1;
	*_vm->getVar("acathbook") = 1;
	_vm->_gfx->showInventory();
}

void RivenExternal::xrhideinventory(uint16 argc, uint16 *argv) {
	_vm->_gfx->hideInventory();
}

void RivenExternal::xrwindowsetup(uint16 argc, uint16 *argv) {
	// TODO: Randomizing what effect happens when you look out into the middle of Tay (useless! :P)
}

// ------------------------------------------------------------------------------------
// tspit (Temple Island) external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xtexterior300_telescopedown(uint16 argc, uint16 *argv) {
	// First, show the button movie
	_vm->_video->playMovieBlocking(3);

	// Don't do anything else if the telescope power is off
	if (*_vm->getVar("ttelevalve") == 0)
		return;

	uint32 *telescopePos = _vm->getVar("ttelescope");
	uint32 *telescopeCover = _vm->getVar("ttelecover");

	if (*telescopePos == 1) {
		// We're at the bottom, which means one of two things can happen...
		if (*telescopeCover == 1 && *_vm->getVar("ttelepin") == 1) {
			// ...if the cover is open and the pin is up, the game is now over.
			if (*_vm->getVar("pcage") == 2) {
				// The best ending: Catherine is free, Gehn is trapped, Atrus comes to rescue you.
				// And now we fall back to Earth... all the way...
				warning("xtexterior300_telescopedown: Good ending");
				_vm->_video->activateMLST(8, _vm->getCurCard());
				runEndGame(8);
			} else if (*_vm->getVar("agehn") == 4) {
				// The ok ending: Catherine is still trapped, Gehn is trapped, Atrus comes to rescue you.
				// Nice going! Catherine and the islanders are all dead now! Just go back to your home...
				warning("xtexterior300_telescopedown: OK ending");
				_vm->_video->activateMLST(9, _vm->getCurCard());
				runEndGame(9);
			} else if (*_vm->getVar("atrapbook") == 1) {
				// The bad ending: Catherine is trapped, Gehn is free, Atrus gets shot by Gehn,
				// And then you get shot by Cho. Nice going! Catherine and the islanders are dead
				// and you have just set Gehn free from Riven, not to mention you're dead.
				warning("xtexterior300_telescopedown: Bad ending");
				_vm->_video->activateMLST(10, _vm->getCurCard());
				runEndGame(10);
			} else {
				// The impossible ending: You don't have Catherine's journal and yet you were somehow
				// able to open the hatch on the telescope. The game provides an ending for those who
				// cheat, load a saved game with the combo, or just guess the telescope combo. Atrus
				// doesn't come and you just fall into the fissure.
				warning("xtexterior300_telescopedown: Wtf ending");
				_vm->_video->activateMLST(11, _vm->getCurCard());
				runEndGame(11);
			}
		} else {
			// ...the telescope can't move down anymore.
			// Play the sound of not being able to move
			_vm->_cursor->setCursor(kRivenHideCursor);
			_vm->_sound->playSoundBlocking(13);
		}
	} else {
		// We're not at the bottom, and we can move down again

		// Play a piece of the moving down movie
		static const uint32 timeIntervals[] = { 4320, 3440, 2560, 1760, 880, 0 };
		uint16 movieCode = (*telescopeCover) ? 1 : 2;
		VideoHandle handle = _vm->_video->playMovie(movieCode);
		_vm->_video->setVideoBounds(handle, Graphics::VideoTimestamp(timeIntervals[*telescopePos], 600), Graphics::VideoTimestamp(timeIntervals[*telescopePos - 1], 600));
		_vm->_sound->playSound(14); // Play the moving sound
		_vm->_video->waitUntilMovieEnds(handle);

		// Now move the telescope down a position and refresh
		*telescopePos -= 1;
		_vm->refreshCard();
	}
}

void RivenExternal::xtexterior300_telescopeup(uint16 argc, uint16 *argv) {
	// First, show the button movie
	_vm->_video->playMovieBlocking(3);

	// Don't do anything else if the telescope power is off
	if (*_vm->getVar("ttelevalve") == 0)
		return;

	uint32 *telescopePos = _vm->getVar("ttelescope");

	// Check if we can't move up anymore
	if (*telescopePos == 5) {
		// Play the sound of not being able to move
		_vm->_cursor->setCursor(kRivenHideCursor);
		_vm->_sound->playSoundBlocking(13);
		return;
	}

	// Play a piece of the moving up movie
	static const uint32 timeIntervals[] = { 0, 800, 1680, 2560, 3440, 4320 };
	uint16 movieCode = (*_vm->getVar("ttelecover")) ? 4 : 5;
	VideoHandle handle = _vm->_video->playMovie(movieCode);
	_vm->_video->setVideoBounds(handle, Graphics::VideoTimestamp(timeIntervals[*telescopePos - 1], 600), Graphics::VideoTimestamp(timeIntervals[*telescopePos], 600));
	_vm->_sound->playSound(14); // Play the moving sound
	_vm->_video->waitUntilMovieEnds(handle);

	// Now move the telescope up a position and refresh
	*telescopePos += 1;
	_vm->refreshCard();
}

void RivenExternal::xtisland390_covercombo(uint16 argc, uint16 *argv) {
	// Called when clicking the telescope cover buttons. argv[0] is the button number (1...5).
	uint32 *correctDigits = _vm->getVar("tcovercombo");

	if (*correctDigits < 5 && argv[0] == getComboDigit(*_vm->getVar("tcorrectorder"), *correctDigits))
		*correctDigits += 1;
	else
		*correctDigits = 0;

	// If we have hit the correct 5 buttons in a row, activate the hotspot to open up the
	// telescope cover.
	if (*correctDigits == 5)
		_vm->_hotspots[9].enabled = true;
	else
		_vm->_hotspots[9].enabled = false;
}

// Atrus' Journal and Trap Book are added to inventory
void RivenExternal::xtatrusgivesbooks(uint16 argc, uint16 *argv) {
	// Give the player Atrus' Journal and the Trap book
	*_vm->getVar("aatrusbook") = 1;
	*_vm->getVar("atrapbook") = 1;
}

// Trap Book is removed from inventory
void RivenExternal::xtchotakesbook(uint16 argc, uint16 *argv) {
	// And now Cho takes the trap book. Sure, this isn't strictly
	// necessary to add and them remove the trap book... but it
	// seems better to do this ;)
	*_vm->getVar("atrapbook") = 0;
}

void RivenExternal::xthideinventory(uint16 argc, uint16 *argv) {
	_vm->_gfx->hideInventory();
}

// Marble Puzzle related constants
static const uint32 kMarbleCount = 6;
static const int kSmallMarbleWidth = 4;
static const int kSmallMarbleHeight = 2;
static const int kLargeMarbleSize = 8;
static const int kMarbleHotspotSize = 13;
static const char *s_marbleNames[] = { "tred", "torange", "tyellow", "tgreen", "tblue", "tviolet" };

// Marble Puzzle helper functions
// The y portion takes the upper 16 bits, while the x portion takes the lower 16 bits
static void setMarbleX(uint32 *var, byte x) {
	*var = (*var & 0xff00) | (x + 1);
}

static void setMarbleY(uint32 *var, byte y) {
	*var = ((y + 1) << 16) | (*var & 0xff);
}

static byte getMarbleX(uint32 *var) {
	return (*var & 0xff) - 1;
}

static byte getMarbleY(uint32 *var) { // Give that that Y you old hag! </bad Seinfeld reference>
	return ((*var >> 16) & 0xff) - 1;
}

static Common::Rect generateMarbleGridRect(uint16 x, uint16 y) {
	// x/y in terms of 0!
	static const int marbleGridOffsetX[] = { 134, 202, 270, 338, 406 };
	static const int marbleGridOffsetY[] = {  24,  92, 159, 227, 295 };

	uint16 offsetX = marbleGridOffsetX[x / 5] + (x % 5) * kMarbleHotspotSize;
	uint16 offsetY = marbleGridOffsetY[y / 5] + (y % 5) * kMarbleHotspotSize;
	return Common::Rect(offsetX, offsetY, offsetX + kMarbleHotspotSize, offsetY + kMarbleHotspotSize);
}

void RivenExternal::xt7500_checkmarbles(uint16 argc, uint16 *argv) {
	// Set apower if the marbles are in their correct spot. 

	bool valid = true;
	static const uint32 marbleFinalValues[] = { 1114121, 1441798, 0, 65552, 65558, 262146 };

	for (uint16 i = 0; i < kMarbleCount; i++)
		if (*_vm->getVar(s_marbleNames[i]) != marbleFinalValues[i]) {
			valid = false;
			break;
		}

	// If we have the correct combo, activate the power and reset the marble positions
	// Otherwise, make sure the power is off
	if (valid) {
		*_vm->getVar("apower") = 1;
		for (uint16 i = 0; i < kMarbleCount; i++)
			*_vm->getVar(s_marbleNames[i]) = 0;
	} else
		*_vm->getVar("apower") = 0;
}

void RivenExternal::xt7600_setupmarbles(uint16 argc, uint16 *argv) {
	// Draw the small marbles when we're a step away from the waffle
	uint16 baseBitmapId = _vm->findResourceID(ID_TBMP, "*tsmallred");
	bool waffleDown = *_vm->getVar("twaffle") != 0;

	// Note that each of the small marble images is exactly 4x2

	for (uint16 i = 0; i < kMarbleCount; i++) {
		uint32 *var = _vm->getVar(s_marbleNames[i]);

		if (*var == 0) {
			// The marble is still in its initial place
			// (Note that this is still drawn even if the waffle is down)
			int marbleX = 376 + i * 2;
			int marbleY = 253 + i * 4;
			_vm->_gfx->copyImageToScreen(baseBitmapId + i, marbleX, marbleY, marbleX + kSmallMarbleWidth, marbleY + kSmallMarbleHeight);
		} else if (waffleDown) {
			// The marble is on the grid and the waffle is down
			// (Nothing to draw here)
		} else {
			// The marble is on the grid and the waffle is up
			// TODO: Draw them onto the grid
		}
	}
}

void RivenExternal::setMarbleHotspots() {
	// Set the hotspots
	for (uint16 i = 0; i < kMarbleCount; i++) {
		uint32 *marblePos = _vm->getVar(s_marbleNames[i]);

		if (*marblePos == 0) // In the receptacle
			_vm->_hotspots[i + 3].rect = _marbleBaseHotspots[i];
		else                 // On the grid
			_vm->_hotspots[i + 3].rect = generateMarbleGridRect(getMarbleX(marblePos), getMarbleY(marblePos));
	}
}

void RivenExternal::xt7800_setup(uint16 argc, uint16 *argv) {
	// First, let's store the base receptacle hotspots for the marbles
	if (_marbleBaseHotspots.empty())
		for (uint16 i = 0; i < kMarbleCount; i++)
			_marbleBaseHotspots.push_back(_vm->_hotspots[i + 3].rect);

	// Move the marble hotspots based on their position variables
	setMarbleHotspots();
	*_vm->getVar("themarble") = 0;
}

void RivenExternal::drawMarbles() {
	for (uint32 i = 0; i < kMarbleCount; i++) {
		// Don't draw the marble if we're holding it
		if (*_vm->getVar("themarble") - 1 == i)
			continue;

		Common::Rect rect = _vm->_hotspots[i + 3].rect;
		// Trim the rect down a bit
		rect.left += 3;
		rect.top += 3;
		rect.right -= 2;
		rect.bottom -= 2;
		_vm->_gfx->drawExtrasImage(i + 200, rect);
	}
}

void RivenExternal::xdrawmarbles(uint16 argc, uint16 *argv) {
	// Draw marbles in the closeup
	drawMarbles();

	// We have to re-enable the updates here
	// Would be really nice if the scripts did this for us, but alas...
	_vm->_gfx->_updatesEnabled = true;
}

void RivenExternal::xtakeit(uint16 argc, uint16 *argv) {
	// Pick up and move a marble

	// First, let's figure out what marble we're now holding
	uint32 *marble = _vm->getVar("themarble");
	*marble = 0;

	for (uint32 i = 0; i < kMarbleCount; i++)
		if (_vm->_hotspots[i + 3].rect.contains(_vm->_system->getEventManager()->getMousePos())) {
			*marble = i + 1;
			break;
		}

	// xtakeit() shouldn't be called if we're not on a marble hotspot
	assert(*marble);

	// Redraw the background
	_vm->_gfx->drawPLST(1);
	_vm->_gfx->updateScreen();

	// Loop until the player lets go (or quits)
	Common::Event event;
	bool mouseDown = true;
	while (mouseDown) {
		while (_vm->_system->getEventManager()->pollEvent(event)) {
			if (event.type == Common::EVENT_LBUTTONUP)
				mouseDown = false;
			else if (event.type == Common::EVENT_MOUSEMOVE)
				_vm->_system->updateScreen();
			else if (event.type == Common::EVENT_QUIT || event.type == Common::EVENT_RTL)
				return;
		}

		_vm->_system->delayMillis(10); // Take it easy on the CPU
	}

	// Check if we landed in a valid location and no other marble has that location
	uint32 *marblePos = _vm->getVar(s_marbleNames[*marble - 1]);

	bool foundMatch = false;
	for (int y = 0; y < 25 && !foundMatch; y++) {
		for (int x = 0; x < 25 && !foundMatch; x++) {
			Common::Rect testHotspot = generateMarbleGridRect(x, y);

			// Let's try to place the marble!
			if (testHotspot.contains(_vm->_system->getEventManager()->getMousePos())) {
				// Set this as the position
				setMarbleX(marblePos, x);
				setMarbleY(marblePos, y);

				// Let's make sure no other marble is in this spot...
				for (uint16 i = 0; i < kMarbleCount; i++)
					if (i != *marble - 1 && *_vm->getVar(s_marbleNames[i]) == *marblePos)
						*marblePos = 0;

				// We have a match
				foundMatch = true;
			}
		}
	}

	// If we still don't have a match, reset it to the original location
	if (!foundMatch)
		*marblePos = 0;

	// Check the new hotspots and refresh everything
	*marble = 0;
	setMarbleHotspots();
	_vm->_curHotspot = -1;
	_vm->checkHotspotChange();
	_vm->_gfx->updateScreen();
}

void RivenExternal::xtscpbtn(uint16 argc, uint16 *argv) {
	runDomeButtonMovie();
}

void RivenExternal::xtisland4990_domecheck(uint16 argc, uint16 *argv) {
	runDomeCheck();
}

void RivenExternal::xtisland5056_opencard(uint16 argc, uint16 *argv) {
	checkDomeSliders(29, 30);
}

void RivenExternal::xtisland5056_resetsliders(uint16 argc, uint16 *argv) {
	resetDomeSliders(37, 3);
}

void RivenExternal::xtisland5056_slidermd(uint16 argc, uint16 *argv) {
	dragDomeSlider(37, 29, 30, 3);
}

void RivenExternal::xtisland5056_slidermw(uint16 argc, uint16 *argv) {
	checkSliderCursorChange(3);
}

void RivenExternal::xtatboundary(uint16 argc, uint16 *argv) {
	runDemoBoundaryDialog();
}

// ------------------------------------------------------------------------------------
// Common external commands
// ------------------------------------------------------------------------------------

void RivenExternal::xflies(uint16 argc, uint16 *argv) {
	// TODO: Activate the "flies" effect
}

} // End of namespace Mohawk
