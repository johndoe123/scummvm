/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 */

#include "stdafx.h"
#include "chooser.h"
#include "newgui.h"
#include "ListWidget.h"

enum {
	kChooseCmd = 'Chos'
};

ChooserDialog::ChooserDialog(const String title, const StringList& list)
	: Dialog(8, 24, 320 -2 * 8, 141) {
	// Headline
	new StaticTextWidget(this, 10, 8, _w - 2 * 10, kLineHeight, title, kTextAlignCenter);

	// Add choice list
	_list = new ListWidget(this, 10, 22, _w - 2 * 10, _h - 22 - 24 - 10);
	_list->setNumberingMode(kListNumberingOff);
	_list->setList(list);
	
	// Buttons
	addButton(_w - 2 * (kButtonWidth + 10), _h - 24, "Cancel", kCloseCmd, 0);
	_chooseButton = addButton(_w-(kButtonWidth + 10), _h - 24, "Choose", kChooseCmd, 0);
	_chooseButton->setEnabled(false);

	// Result = -1 -> no choice was made
	setResult(-1);
}

void ChooserDialog::handleCommand(CommandSender *sender, uint32 cmd, uint32 data) {
	int item = _list->getSelected();
	switch (cmd) {
	case kChooseCmd:
	case kListItemDoubleClickedCmd:
		setResult(item);
		close();
		break;
	case kListSelectionChangedCmd:
		_chooseButton->setEnabled(item >= 0);
		_chooseButton->draw();
		break;
	default:
		Dialog::handleCommand(sender, cmd, data);
	}
}
