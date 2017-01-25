#pragma once

class Mouse {
public:
	static Mouse* _instance;
	
	void reset();
	bool getButtonState(int);
	bool isButtonDown(int);
	short getX();
	short getY();
	short getDX();
	short getDY();
	void reset2();
	void next();
	void rewind();
	void getEventButtonState();
	void getEventButton();
	void getEvent();
	void feed(char, char, short, short);
	void feed(char, char, short, short, short, short);
	void feedRelative(char, char, short, short);
};