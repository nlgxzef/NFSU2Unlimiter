#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

int(*DialogInterface_ShowDialog)(feDialogConfig* conf) = (int(*)(feDialogConfig*))0x558020;

int DialogInterface_ShowThreeButtons(char const* from_pkg, char const* dlg_pkg, unsigned int button1_text_hash, unsigned int button2_text_hash, unsigned int button3_text_hash, unsigned int button1_pressed_message, unsigned int button2_pressed_message, unsigned int button3_pressed_message, unsigned int cancel_message, bool dismissable, unsigned int first_button, unsigned int blurb_fmt)
{
	feDialogConfig conf;

	conf.ParentPackage = from_pkg;
	conf.DialogPackage = dlg_pkg;
	conf.Button3TextHash = button1_text_hash; // reversed for some reason
	conf.Button3PressedMessage = button1_pressed_message;
	conf.Button2TextHash = button2_text_hash;
	conf.Button2PressedMessage = button2_pressed_message;
	conf.Button1TextHash = button3_text_hash;
	conf.Button1PressedMessage = button3_pressed_message;
	conf.DialogCancelledMessage = cancel_message;
	conf.FirstButton = first_button;
	conf.NumButtons = 3;
	conf.bIsDismissable = dismissable;

	bSPrintf(conf.BlurbString, GetLocalizedString(blurb_fmt));

	return DialogInterface_ShowDialog(&conf);
}