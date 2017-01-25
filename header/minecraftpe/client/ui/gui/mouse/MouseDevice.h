#pragma once

class MouseDevice {
public:
	char filler[40];
	
	MouseDevice();
	~MouseDevice();
	void reset();
	int getButtonState(int);
	bool isButtonDown(int);
	bool wasFirstMovement();
	short getX();
	short getY();
	short getDX();
	short getDY();
	void reset2();
	bool next();
	void rewind();
	int getEventButtonState();
	int getEventButton();
	int getEvent();
	void feed(char, char, short, short);
	void feed(char, char, short, short, short, short);
};