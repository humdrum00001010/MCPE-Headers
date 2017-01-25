#pragma once

// Size : 16
class MouseAction {
public:
	char filler[16];
	
	MouseAction(char, char, short, short, int);
	MouseAction(char, char, short, short, short, short, int);
	bool isButton();
};