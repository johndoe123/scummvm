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

#include "neverhood/module2400.h"

namespace Neverhood {

Module2400::Module2400(NeverhoodEngine *vm, Module *parentModule, int which)
	: Module(vm, parentModule) {
	
	_vm->_soundMan->addMusic(0x202D1010, 0xB110382D);

	if (which < 0) {
		createScene(_vm->gameState().sceneNum, _vm->gameState().which);
	} else {
		createScene(0, 0);
	}

}

Module2400::~Module2400() {
	_vm->_soundMan->deleteMusicGroup(0x202D1010);
}

void Module2400::createScene(int sceneNum, int which) {
	debug("Module2400::createScene(%d, %d)", sceneNum, which);
	_vm->gameState().sceneNum = sceneNum;
	switch (_vm->gameState().sceneNum) {
	case 0:
		_vm->_soundMan->stopMusic(0xB110382D, 0, 0);
		_childObject = new Scene2401(_vm, this, which);
		break;
	case 1:
		_vm->_soundMan->startMusic(0xB110382D, 0, 2);
		_childObject = new Scene2402(_vm, this, which);
		break;
	case 2:
		_vm->_soundMan->startMusic(0xB110382D, 0, 0);
		_childObject = new Scene2403(_vm, this, which);
		break;
	case 4:
		_vm->_soundMan->stopMusic(0xB110382D, 0, 2);
		_childObject = new DiskplayerScene(_vm, this, 0);
		break;
	case 5:
		_vm->_soundMan->startMusic(0xB110382D, 0, 2);
		_childObject = new Scene2406(_vm, this, which);
		break;
	case 6:
		_vm->_soundMan->stopMusic(0xB110382D, 0, 2);
		createSmackerScene(0x20D80001, true, true, false);
		break;
	case 7:
		createStaticScene(0x81523218, 0x2321C81D);
		break;
	case 8:
		createStaticScene(0x08100210, 0x00214089);
		break;
	case 9:
		createStaticScene(0x8C020505, 0x205018C8);
		break;
	}
	SetUpdateHandler(&Module2400::updateScene);
	_childObject->handleUpdate();
}

void Module2400::updateScene() {
	if (!updateChild()) {
		switch (_vm->gameState().sceneNum) {
		case 0:
			if (_moduleResult == 1)
				createScene(1, 0);
			else
				leaveModule(0);
			break;
		case 1:
			if (_moduleResult == 1)
				createScene(5, 0);
			else if (_moduleResult == 2)
				createScene(7, -1);
			else
				createScene(0, 1);
			break;
		case 2:
			if (_moduleResult == 1)
				createScene(9, -1);
			else if (_moduleResult == 2)
				createScene(6, -1);
			else
				createScene(5, 1);
			break;
		case 4:
			createScene(5, 2);
			break;
		case 5:
			if (_moduleResult == 1)
				createScene(2, 0);
			else if (_moduleResult == 2)
				createScene(4, 0);
			else if (_moduleResult == 3)
				createScene(8, -1);
			else
				createScene(1, 1);
			break;
		case 6:
			createScene(2, 2);
			break;
		case 7:
			createScene(1, 2);
			break;
		case 8:
			createScene(5, 3);
			break;
		case 9:
			createScene(2, 1);
			break;
		}
	}
}

static const NPoint kScene2401Points[] = {
	{384, 389},
	{406, 389},
	{429, 389},
	{453, 389},
	{477, 389}
};

static const uint32 kScene2401FileHashes1[] = {
	0x02842920,
	0x02882920,
	0x02902920,
	0x02A02920,
	0x02C02920,
	0x02002920,
	0x03802920,
	0x00802920,
	0x06802920,
	0x03842920
};

static const uint32 kScene2401FileHashes2[] = {
	0xD0910020,
	0xD0910038,
	0xD0910008,
	0xD0910068,
	0xD09100A8,
	0
};

static const uint32 kScene2401FileHashes3[] = {
	0xD0910020,
	0xD0910038,
	0xD0910008,
	0xD0910068,
	0xD09100A8,
	0
};

static const NRect kScene2401Rects[] = {
	NRect(369, 331, 394, 389),
	NRect(395, 331, 419, 389),
	NRect(420, 331, 441, 389),
	NRect(442, 331, 464, 389),
	NRect(465, 331, 491, 389)
};

static const uint32 kAsScene2401WaterSpitFileHashes2[] = {
	0x5C044690,
	0x5C644690,
	0x5CA44690,
	0x5D244690,
	0x5E244690
};

static const uint32 kAsScene2401WaterSpitFileHashes1[] = {
	0xF4418408,
	0xF4418808,
	0xF4419008,
	0xF441A008,
	0xCD4F8411
};

AsScene2401WaterSpit::AsScene2401WaterSpit(NeverhoodEngine *vm)
	: AnimatedSprite(vm, 1200) {
	
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene2401WaterSpit::handleMessage);
	SetSpriteUpdate(&AnimatedSprite::updateDeltaXY);
	createSurface(100, 146, 74);
	_x = 240;
	_y = 447;
	setVisible(false);
}

uint32 AsScene2401WaterSpit::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x120A0013)
			playSound(0, kAsScene2401WaterSpitFileHashes1[_soundIndex]);
		break;
	case 0x2000:
		_x = 240;
		_y = 447;
		_soundIndex = getSubVar(VA_CURR_WATER_PIPES_LEVEL, param.asInteger());
		playSound(0, 0x48640244);
		startAnimation(kAsScene2401WaterSpitFileHashes2[param.asInteger()], 0, -1);
		setVisible(true);
		break;
	case 0x3002:
		stopAnimation();
		setVisible(false);
		break;
	}
	return messageResult;
}

AsScene2401FlowingWater::AsScene2401FlowingWater(NeverhoodEngine *vm)
	: AnimatedSprite(vm, 1200), _isWaterFlowing(false) {
	
	createSurface1(0x10203116, 100);
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene2401FlowingWater::handleMessage);
	_x = 88;
	_y = 421;
	setVisible(false);
}

AsScene2401FlowingWater::~AsScene2401FlowingWater() {
	_vm->_soundMan->deleteSoundGroup(0x40F11C09);
}

uint32 AsScene2401FlowingWater::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (_isWaterFlowing && param.asInteger() == 0x02421405) {
			startAnimationByHash(0x10203116, 0x01084280, 0);
		}
		break;
	case 0x2002:
		if (!_isWaterFlowing) {
			_vm->_soundMan->addSound(0x40F11C09, 0x980C1420);
			_vm->_soundMan->playSoundLooping(0x980C1420);
			startAnimation(0x10203116, 0, -1);
			setVisible(true);
			_isWaterFlowing = true;
		}
		break;
	case 0x2003:
		_vm->_soundMan->deleteSound(0x980C1420);
		_isWaterFlowing = false;
		break;
	case 0x3002:
		stopAnimation();
		setVisible(false);
		break;
	}
	return messageResult;
}
	
AsScene2401WaterFlushing::AsScene2401WaterFlushing(NeverhoodEngine *vm, int16 x, int16 y)
	: AnimatedSprite(vm, 1200), _countdown(0), _flushLoopCount(0) {
	
	createSurface1(0xB8596884, 100);
	SetUpdateHandler(&AsScene2401WaterFlushing::update);
	SetMessageHandler(&AsScene2401WaterFlushing::handleMessage);
	_x = x;
	_y = y;
	setVisible(false);
}

void AsScene2401WaterFlushing::update() {
	if (_countdown != 0 && (--_countdown) == 0) {
		setDoDeltaX(_vm->_rnd->getRandomNumber(1));
		startAnimation(0xB8596884, 0, -1);
		setVisible(true);
	}
	AnimatedSprite::update();
}

uint32 AsScene2401WaterFlushing::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (_flushLoopCount > 0 && param.asInteger() == 0x02421405) {
			startAnimationByHash(0xB8596884, 0x01084280, 0);
			_flushLoopCount--;
		}
		break;
	case 0x2002:
		if (param.asInteger() > 0) {
			_flushLoopCount = param.asInteger() - 1;
			_countdown = _vm->_rnd->getRandomNumber(3) + 1;
		}
		break;
	case 0x3002:
		stopAnimation();
		setVisible(false);
		break;
	}
	return messageResult;
}

AsScene2401Door::AsScene2401Door(NeverhoodEngine *vm, bool isOpen)
	: AnimatedSprite(vm, 1100), _countdown(0), _isOpen(isOpen) {
	
	createSurface1(0x44687810, 100);
	_x = 320;
	_y = 240;
	_newStickFrameIndex = -2;
	SetUpdateHandler(&AsScene2401Door::update);
	SetMessageHandler(&AsScene2401Door::handleMessage);
	if (_isOpen) {
		stopAnimation();
		setVisible(false);
		_countdown = 48;
	} else {
		startAnimation(0x44687810, 0, -1);
		_newStickFrameIndex = 0;
	}
}

void AsScene2401Door::update() {
	if (_isOpen && _countdown != 0 && (--_countdown) == 0) {
		_isOpen = false;
		setVisible(true);
		startAnimation(0x44687810, -1, -1);
		_newStickFrameIndex = 0;
		_playBackwards = true;
		playSound(0, calcHash("fxDoorClose38"));
	}
	AnimatedSprite::update();
}

uint32 AsScene2401Door::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x2004:
		if (_isOpen)
			_countdown = 168;
		messageResult = _isOpen ? 1 : 0;
		break;
	case 0x3002:
		gotoNextState();
		break;
	case 0x4808:
		if (!_isOpen) {
			_countdown = 168;
			_isOpen = true;
			setVisible(true);
			startAnimation(0x44687810, 0, -1);
			playSound(0, calcHash("fxDoorOpen38"));
			NextState(&AsScene2401Door::stDoorOpenFinished);
		}
		break;
	}
	return messageResult;
}

void AsScene2401Door::stDoorOpenFinished() {
	stopAnimation();
	setVisible(false);
}

Scene2401::Scene2401(NeverhoodEngine *vm, Module *parentModule, int which)
	: Scene(vm, parentModule, true), _countdown1(0), _countdown2(0), _flag(false),
	_soundToggle(false), _asWaterSpitIndex(0) {

	_vm->gameModule()->initScene2401Vars();

	setGlobalVar(V_WATER_RUNNING, 1); //DEBUG! Enables water
	setGlobalVar(V_HAS_TEST_TUBE, 1); //DEBUG! Gives the test tube

	_surfaceFlag = true;
	SetMessageHandler(&Scene2401::handleMessage);
	SetUpdateHandler(&Scene2401::update);

	setRectList(0x004B3140);
	setBackground(0x8C030206);
	setPalette(0x8C030206);
	addEntity(_palette);
	_palette->addBasePalette(0x8C030206, 0, 256, 0);
	_palette->addPalette(0x91D3A391, 0, 65, 0);
	insertMouse433(0x302028C8);

	_sprite1 = insertStaticSprite(0x2E068A23, 200);
	insertStaticSprite(0x401410A6, 200);
	_asFlowingWater = insertSprite<AsScene2401FlowingWater>();
	insertStaticSprite(0x90C0A4B4, 200);
	_ssButton = insertSprite<SsCommonButtonSprite>(this, 0x0092916A, 100, 0);
	_ssFloorButton = insertSprite<SsCommonFloorButton>(this, 0x28001120, 0x00911068, 100, 0);

	for (uint i = 0; i < 5; i++)
		_asWaterFlushing[i] = insertSprite<AsScene2401WaterFlushing>(kScene2401Points[i].x, kScene2401Points[i].y);

	for (uint i = 0; i < 10; i++) {
		_ssWaterPipes[i] = insertStaticSprite(kScene2401FileHashes1[i], 300);
		_ssWaterPipes[i]->setVisible(false);
	}
	
	_asWaterSpit[0] = insertSprite<AsScene2401WaterSpit>();
	_asWaterSpit[1] = insertSprite<AsScene2401WaterSpit>();

	if (which < 0) {
		insertKlayman<KmScene2401>(200, 447);
		setMessageList(0x004B2F70);
		_asDoor = insertSprite<AsScene2401Door>(false);
	} else if (which == 1) {
		insertKlayman<KmScene2401>(280, 413);
		setMessageList(0x004B2F80);
		_palette->addBasePalette(0xB103B604, 0, 65, 0);
		_palette->addPalette(0xB103B604, 0, 65, 0);
		_asDoor = insertSprite<AsScene2401Door>(true);
	} else {
		insertKlayman<KmScene2401>(-20, 447);
		setMessageList(0x004B2F78);
		_asDoor = insertSprite<AsScene2401Door>(false);
	}

}

void Scene2401::update() {

	if (_countdown1 != 0 && (--_countdown1) == 0) {
		if (_pipeStatus > 0 && _pipeStatus <= 10)
			_ssWaterPipes[_pipeStatus - 1]->setVisible(false);
		if (_pipeStatus >= 10) {
			bool puzzleSolved = true, waterInside = false;
			for (uint i = 0; i < 5; i++) {
				if (getSubVar(VA_CURR_WATER_PIPES_LEVEL, i) != getSubVar(VA_GOOD_WATER_PIPES_LEVEL, i))
					puzzleSolved = false;
				if (getSubVar(VA_CURR_WATER_PIPES_LEVEL, i) != 0)
					waterInside = true;
			}
			if (puzzleSolved) {
				setGlobalVar(V_NOTES_DOOR_UNLOCKED, 1);
				setGlobalVar(V_NOTES_PUZZLE_SOLVED, 1);
				sendMessage(_asDoor, 0x4808, 0);
			} else if (waterInside) {
				playPipeSound(0xD0431020);
				for (uint i = 0; i < 5; i++) {
					sendMessage(_asWaterFlushing[i], 0x2002, getSubVar(VA_CURR_WATER_PIPES_LEVEL, i));
					setSubVar(VA_CURR_WATER_PIPES_LEVEL, i, 0);
				}
			}
		} else if (_pipeStatus >= 5) {
			_ssWaterPipes[_pipeStatus]->setVisible(true);
			_countdown1 = 8;
			playPipeSound(kScene2401FileHashes3[getSubVar(VA_CURR_WATER_PIPES_LEVEL, _pipeStatus - 5)]);
		} else {
			_ssWaterPipes[_pipeStatus]->setVisible(true);
			_countdown1 = _pipeStatus == 4 ? 16 : 8;
			playPipeSound(kScene2401FileHashes3[getSubVar(VA_GOOD_WATER_PIPES_LEVEL, _pipeStatus)]);
		}
		_pipeStatus++;
	}

	if (_countdown2 != 0 && (--_countdown2) == 0) {
		sendMessage(_asFlowingWater, 0x2003, 0);
	}
	
	Scene::update();

}

uint32 Scene2401::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Scene::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x402064D8)
			sendEntityMessage(_klayman, 0x1014, _ssButton);
		else if (param.asInteger() == 0x02144CB1)
			sendEntityMessage(_klayman, 0x1014, _ssFloorButton);
		else if (param.asInteger() == 0x11C40840) {
			if (getGlobalVar(V_NOTES_DOOR_UNLOCKED) && sendMessage(_asDoor, 0x2004, 0)) {
				setMessageList(0x004B3090);
			} else {
				setMessageList(0x004B30B0);
			}
		} else if (param.asInteger() == 0x412722C0) {
			if (_countdown2 > 0 && getGlobalVar(V_HAS_TEST_TUBE)) {
				_countdown2 = 144;
				setMessageList(0x004B3020);
			} else {
				setMessageList(0x004B3050);
			}
		} else if (param.asInteger() == 0x21142050) {
			if (_flag && _countdown1 == 0 && getGlobalVar(V_NOTES_PUZZLE_SOLVED) == 0) {
				setMessageList(0x004B2FA8);
			} else {
				setMessageList(0x004B2FC8);
			}
		} else if (param.asInteger() == 0x87441031) {
			setSurfacePriority(_sprite1->getSurface(), 1100);
		} else if (param.asInteger() == 0x80C40322) {
			setSurfacePriority(_sprite1->getSurface(), 200);
			cancelMessageList();
			_flag = true;
		} else if (param.asInteger() == 0x09C4B40A && _countdown2 > 12) {
			_countdown2 = 12;
		}
		break;
	case 0x2000:
		messageResult = 0;
		for (uint32 i = 0; i < 5; i++)
			if (kScene2401Rects[i].contains(_mouseClickPos.x, _mouseClickPos.y)) {
				messageResult = i;
				break;
			}
		break;
	case 0x2001:
		sendMessage(_asWaterSpit[_asWaterSpitIndex], 0x2000, param.asInteger());
		_asWaterSpitIndex = (_asWaterSpitIndex + 1) & 1;
		incSubVar(VA_CURR_WATER_PIPES_LEVEL, param.asInteger(), 1);
		if (getSubVar(VA_CURR_WATER_PIPES_LEVEL, param.asInteger()) >= 5)
			setSubVar(VA_CURR_WATER_PIPES_LEVEL, param.asInteger(), 4);
		break;
	case 0x480B:
		if (sender == _ssButton) {
			_pipeStatus = 0;
			_countdown1 = 8;
		} else if (sender == _ssFloorButton && getGlobalVar(V_WATER_RUNNING)) {
			_countdown2 = 144;
			sendMessage(_asFlowingWater, 0x2002, 0);
			playSound(0, 0xE1130324);
		}
		break;
	case 0x482A:
		_palette->addBasePalette(0xB103B604, 0, 65, 0);
		_palette->startFadeToPalette(12);
		break;
	case 0x482B:
		_palette->addBasePalette(0x91D3A391, 0, 65, 0);
		_palette->startFadeToPalette(12);
		break;
	}
	return messageResult;
}

void Scene2401::playPipeSound(uint32 fileHash) {
	if (_soundToggle)
		playSound(0, fileHash);
	else
		playSound(1, fileHash);
	_soundToggle = !_soundToggle;
}

static const uint32 kScene2402FileHashes[] = {
	0xD0910020,
	0xD0910038,
	0xD0910008,
	0xD0910068,
	0xD09100A8
};

AsScene2402Door::AsScene2402Door(NeverhoodEngine *vm, Scene *parentScene, bool isOpen)
	: AnimatedSprite(vm, 1100), _parentScene(parentScene), _isOpen(isOpen) {

	SetUpdateHandler(&AsScene2402Door::update);
	SetMessageHandler(&AsScene2402Door::handleMessage);
	createSurface1(0x80495831, 100);
	_x = 320;
	_y = 240;
	_newStickFrameIndex = -2;
	if (_isOpen) {
		startAnimation(0x80495831, -1, -1);
		_countdown = 48;
	} else {
		stopAnimation();
		setVisible(false);
	}	
}

void AsScene2402Door::update() {
	if (_isOpen && _countdown != 0 && (--_countdown) == 0) {
		_isOpen = false;
		setVisible(true);
		startAnimation(0x80495831, -1, -1);
		_playBackwards = true;
		playSound(0, calcHash("fxDoorClose38"));
		NextState(&AsScene2402Door::stDoorClosingFinished);
	}
	AnimatedSprite::update();
}

uint32 AsScene2402Door::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x2000:
		if (_isOpen)
			_countdown = 144;
		messageResult = _isOpen ? 1 : 0;
		break;
	case 0x3002:
		gotoNextState();
		break;
	case 0x4808:
		_countdown = 144;
		_isOpen = true;
		setVisible(true);
		startAnimation(0x80495831, 0, -1);
		_newStickFrameIndex = -2;
		playSound(0, calcHash("fxDoorOpen38"));
		break;
	}
	return messageResult;
}

void AsScene2402Door::stDoorClosingFinished() {
	sendMessage(_parentScene, 0x2001, 0);
	setVisible(false);
}

AsScene2402TV::AsScene2402TV(NeverhoodEngine *vm, Klayman *klayman)
	: AnimatedSprite(vm, 1100), _klayman(klayman), _countdown1(0), _countdown2(0) {

	createSurface(100, 640, 480); // TODO Use correct size	from the two hashes
	SetMessageHandler(&Sprite::handleMessage);
	_x = 260;
	_y = 210;
	setDoDeltaX(1);

	if (!getGlobalVar(V_TV_JOKE_TOLD)) {
		loadSound(0, 0x58208810);
		_countdown1 = 48;
		startAnimation(0x4919397A, 0, -1);
		_newStickFrameIndex = 0;
		SetUpdateHandler(&AsScene2402TV::upWait);
	} else {
		int16 frameIndex;
		if (_klayman->getX() > 320)
			_currFrameIndex = 29;
		frameIndex = CLIP<int16>((_klayman->getX() - _x + 150) / 10, 0, 29);
		startAnimation(0x050A0103, frameIndex, -1);
		_newStickFrameIndex = frameIndex;
		_countdown1 = 0;
		SetUpdateHandler(&AsScene2402TV::upFocusKlayman);
	}

}

AsScene2402TV::~AsScene2402TV() {
	_vm->_soundMan->deleteSoundGroup(0x01520123);
}

void AsScene2402TV::upWait() {
	if (_countdown1 != 0 && (--_countdown1) == 0) {
		startAnimation(0x4919397A, 0, -1);
		SetMessageHandler(&AsScene2402TV::hmJoke);
		NextState(&AsScene2402TV::stJokeFinished);
	}
	AnimatedSprite::update();
}

void AsScene2402TV::upFocusKlayman() {
	int16 frameIndex = CLIP<int16>((_klayman->getX() - _x + 150) / 10, 0, 29);
	if (frameIndex != _currFrameIndex) {
		if (frameIndex > _currFrameIndex) {
			_currFrameIndex++;
		} else if (frameIndex < _currFrameIndex) {
			_currFrameIndex--;
		}
		startAnimation(0x050A0103, _currFrameIndex, -1);
		_newStickFrameIndex = _currFrameIndex;
		if (_countdown2 == 0) {
			_vm->_soundMan->addSound(0x01520123, 0xC42D4528);
			_vm->_soundMan->playSoundLooping(0xC42D4528); 
		}
		_countdown2 = 5;
	} else if (_countdown2 != 0 && (--_countdown2 == 0)) {
		_vm->_soundMan->deleteSound(0xC42D4528);
	}
	AnimatedSprite::update();
}

void AsScene2402TV::stJokeFinished() {
	setGlobalVar(V_TV_JOKE_TOLD, 1);
	startAnimation(0x050A0103, 0, -1);
	_newStickFrameIndex = 0;
	SetUpdateHandler(&AsScene2402TV::upFocusKlayman);
}

uint32 AsScene2402TV::hmJoke(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x431EA0B0) {
			playSound(0);
		}
		break;
	case 0x3002:
		gotoNextState();
		break;
	}
	return messageResult;
}

Scene2402::Scene2402(NeverhoodEngine *vm, Module *parentModule, int which)
	: Scene(vm, parentModule, true), _countdown(0), _soundToggle(false) {

	Sprite *tempSprite;

	_surfaceFlag = true;
	SetMessageHandler(&Scene2402::handleMessage);
	SetUpdateHandler(&Scene2402::update);

	setRectList(0x004AF900);
	setBackground(0x81660220);
	setPalette(0x81660220);
	insertMouse433(0x6022481E);
	_asTape = insertSprite<AsScene1201Tape>(this, 9, 1100, 286, 409, 0x9148A011);
	_vm->_collisionMan->addSprite(_asTape);
	_ssButton = insertSprite<SsCommonButtonSprite>(this, 0x15288120, 100, 0);
	
	if (which < 0) {
		insertKlayman<KmScene2402>(198, 404);
		setMessageList(0x004AF7C8);
	} else if (which == 1) {
		insertKlayman<KmScene2402>(660, 404);
		setMessageList(0x004AF7D8);
	} else if (which == 2) {
		insertKlayman<KmScene2402>(409, 404);
		_klayman->setDoDeltaX(getGlobalVar(V_KLAYMAN_IS_DELTA_X) ? 1 : 0);
		setMessageList(0x004AF888);
	} else {
		insertKlayman<KmScene2402>(0, 404);
		setMessageList(0x004AF7D0);
	}

	tempSprite = insertStaticSprite(0x081A60A8, 1100);
	_ssDoorFrame = (StaticSprite*)insertStaticSprite(0x406C0AE0, 1100);
	_klayman->setClipRect(_ssDoorFrame->getDrawRect().x, 0, 639, tempSprite->getDrawRect().y2());
	_asDoor = insertSprite<AsScene2402Door>(this, which == 1/*CHECKME or != ?*/);
	insertSprite<AsScene2402TV>(_klayman);
	insertStaticSprite(0x3A01A020, 200);

}

Scene2402::~Scene2402() {
	setGlobalVar(V_KLAYMAN_IS_DELTA_X, _klayman->isDoDeltaX() ? 1 : 0);
}

void Scene2402::update() {
	if (_countdown != 0 && (--_countdown) == 0) {
		if (_pipeStatus >= 10) {
			sendMessage(_asDoor, 0x4808, 0);
			_ssDoorFrame->load(0x00B415E0, true, true);
			_ssDoorFrame->update();
		} else if (_pipeStatus >= 5) {
			_countdown = 8;
			playPipeSound(kScene2402FileHashes[getSubVar(VA_CURR_WATER_PIPES_LEVEL, _pipeStatus - 5)]);
		} else {
			_countdown = _pipeStatus == 4 ? 16 : 8;
			playPipeSound(kScene2402FileHashes[getSubVar(VA_GOOD_WATER_PIPES_LEVEL, _pipeStatus)]);
		}
		_pipeStatus++;
	}
	Scene::update();
}

uint32 Scene2402::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Scene::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x402064D8) {
			sendEntityMessage(_klayman, 0x1014, _ssButton);
		} else if (param.asInteger() == 0x01C66840) {
			if (sendMessage(_asDoor, 0x2000, 0))
				setMessageList(0x004AF800);
			else
				setMessageList(0x004AF818);
		}
		break;
	case 0x2001:
		_ssDoorFrame->load(0x406C0AE0, true, true);
		_ssDoorFrame->update();
		break;
	case 0x480B:
		if (sender == _ssButton) {
			_pipeStatus = 0;
			_countdown = 8;
		}
		break;
	case 0x4826:
		if (sender == _asTape) {
			sendEntityMessage(_klayman, 0x1014, _asTape);
			setMessageList(0x004AF890);
		}
		break;
	}
	return messageResult;
}
		
void Scene2402::playPipeSound(uint32 fileHash) {
	if (_soundToggle)
		playSound(0, fileHash);
	else
		playSound(1, fileHash);
	_soundToggle = !_soundToggle;
}

Scene2403::Scene2403(NeverhoodEngine *vm, Module *parentModule, int which)
	: Scene(vm, parentModule, true) {
	
	Sprite *tempSprite1, *tempSprite2, *tempSprite3;

	_surfaceFlag = true;
	SetMessageHandler(&Scene2403::handleMessage);
	setBackground(0x0C05060C);
	setPalette(0x0C05060C);
	_palette->addPalette(0x414364B0, 0, 65, 0);
	insertMouse433(0x506080C8);
	_asTape = insertSprite<AsScene1201Tape>(this, 2, 1100, 480, 454, 0x9148A011);
	_vm->_collisionMan->addSprite(_asTape);
	_asLightCord = insertSprite<AsScene2803LightCord>(this, 0xA1095A10, 0x836D3813, 368, 200);
	_asLightCord->setClipRect(0, 25, 640, 480);
	
	if (which < 0) {
		_isClimbingLadder = false;
		insertKlayman<KmScene2403>(220, 449);
		setMessageList(0x004B5C98);
		setRectList(0x004B5E18);
	} else if (which == 1) {
		_isClimbingLadder = false;
		insertKlayman<KmScene2403>(433, 449);
		setMessageList(0x004B5D70);
		setRectList(0x004B5E18);
	} else if (which == 2) {
		_isClimbingLadder = false;
		insertKlayman<KmScene2403>(440, 449);
		_klayman->setDoDeltaX(1);
		setMessageList(0x004B5C98);
		setRectList(0x004B5E18);
	} else {
		_isClimbingLadder = true;
		insertKlayman<KmScene2403>(122, 599);
		setMessageList(0x004B5CA0);
		setRectList(0x004B5E28);
	}

	_ssButton = insertSprite<SsCommonButtonSprite>(this, 0x3130B0EB, 100, 0);
	tempSprite1 = insertStaticSprite(0x20C24220, 1100);	
	tempSprite2 = insertStaticSprite(0x03080900, 1300);
	tempSprite3 = insertSprite<AsScene1002KlaymanLadderHands>(_klayman);
	tempSprite3->setClipRect(tempSprite1->getDrawRect().x, 0, 640, tempSprite2->getDrawRect().y2());
	_klayman->setClipRect(tempSprite1->getDrawRect().x, 0, 640, tempSprite2->getDrawRect().y2());	

	loadSound(1, calcHash("fxFogHornSoft"));

}

uint32 Scene2403::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Scene::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x040424D0) {
			sendEntityMessage(_klayman, 0x1014, _ssButton);
		} else if (param.asInteger() == 0x180CE614) {
			sendEntityMessage(_klayman, 0x1014, _asLightCord);
		}
		break;
	case 0x2000:
		_isClimbingLadder = true;
		setRectList(0x004B5E28);
		break;
	case 0x2001:
		_isClimbingLadder = false;
		setRectList(0x004B5E18);
		break;
	case 0x480B:
		if (sender == _ssButton) {
			if (getSubVar(VA_LOCKS_DISABLED, 0x304008D2)) {
				setSubVar(VA_LOCKS_DISABLED, 0x304008D2, 0);
				playSound(0, calcHash("fx3LocksDisable"));
			} else {
				setSubVar(VA_LOCKS_DISABLED, 0x304008D2, 1);
				playSound(1);
			}
		}
		break;
	case 0x480F:
		if (sender == _asLightCord) {
			leaveScene(2);
		}
		break;
	case 0x4826:
		if (sender == _asTape && !_isClimbingLadder) {
			sendEntityMessage(_klayman, 0x1014, _asTape);
			setMessageList(0x004B5D98);
		}
		break;
	}
	return messageResult;
}

Scene2406::Scene2406(NeverhoodEngine *vm, Module *parentModule, int which)
	: Scene(vm, parentModule, true) {

	Sprite *tempSprite1, *tempSprite2;

	if (getGlobalVar(V_HAS_FINAL_KEY) && getGlobalVar(V_KEY3_LOCATION) == 0)
		setGlobalVar(V_KEY3_LOCATION, 2);

	_surfaceFlag = true;
	SetMessageHandler(&Scene2406::handleMessage);
	
	setRectList(0x004B78C8);
	insertMouse433(0xB03001A8);

	if (getGlobalVar(V_KEY3_LOCATION) == 2) {
		_asKey = insertSprite<AsCommonKey>(this, 2, 1100, 560, 409);
		_vm->_collisionMan->addSprite(_asKey);
	}

	_asTape = insertSprite<AsScene1201Tape>(this, 5, 1100, 456, 409, 0x9148A011);
	_vm->_collisionMan->addSprite(_asTape);

	tempSprite2 = insertStaticSprite(0x19625293, 1100);
	_clipRects[0].x1 = 0;
	_clipRects[0].y1 = 0;
	_clipRects[0].x2 = tempSprite2->getDrawRect().x2();
	_clipRects[0].y2 = 480;

	if (getGlobalVar(V_SPIKES_RETRACTED)) {
		setBackground(0x1A0B0304);
		setPalette(0x1A0B0304);
		tempSprite1 = insertStaticSprite(0x32923922, 1100);
	} else {
		setBackground(0x0A038595);
		setPalette(0x0A038595);
		tempSprite1 = insertStaticSprite(0x1712112A, 1100);
	}
	
	tempSprite2 = insertStaticSprite(0x22300924, 1300);

	_clipRects[1].x1 = tempSprite1->getDrawRect().x;
	_clipRects[1].y1 = tempSprite2->getDrawRect().y;
	_clipRects[1].x2 = 640;
	_clipRects[1].y2 = 480;

	if (which < 0) {
		_isClimbingLadder = false;
		insertKlayman<KmScene2406>(307, 404, _clipRects, 2);
		setMessageList(0x004B76C8);
		setRectList(0x004B78C8);
	} else if (which == 1) {
		_isClimbingLadder = true;
		insertKlayman<KmScene2406>(253, -16, _clipRects, 2);
		setMessageList(0x004B76D8);
		setRectList(0x004B78D8);
	} else if (which == 2) {
		_isClimbingLadder = false;
		insertKlayman<KmScene2406>(480, 404, _clipRects, 2);
		setMessageList(0x004B77C0);
		setRectList(0x004B78C8);
	} else if (which == 3) {
		_isClimbingLadder = false;
		insertKlayman<KmScene2406>(387, 404, _clipRects, 2);
		setMessageList(0x004B7810);
		setRectList(0x004B78C8);
	} else {
		_isClimbingLadder = false;
		insertKlayman<KmScene2406>(0, 404, _clipRects, 2);
		setMessageList(0x004B76D0);
		setRectList(0x004B78C8);
	}

	tempSprite2 = insertSprite<AsScene1002KlaymanLadderHands>(_klayman);
	tempSprite2->setClipRect(_clipRects[1]);

}

uint32 Scene2406::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Scene::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x41062804) {
			if (getGlobalVar(V_SPIKES_RETRACTED)) {
				setMessageList(0x004B7758);
			} else {
				setMessageList(0x004B7738);
			}
		}
		break;
	case 0x2000:
		_isClimbingLadder = true;
		setRectList(0x004B78D8);
		break;
	case 0x2001:
		_isClimbingLadder = false;
		setRectList(0x004B78C8);
		break;
	case 0x4826:
		if (sender == _asTape && !_isClimbingLadder) {
			sendEntityMessage(_klayman, 0x1014, _asTape);
			setMessageList(0x004B77C8);
		} else if (sender == _asKey && !_isClimbingLadder) {
			sendEntityMessage(_klayman, 0x1014, _asKey);
			setMessageList(0x004B77D8);
		}
		break;
	}
	return messageResult;
}

} // End of namespace Neverhood