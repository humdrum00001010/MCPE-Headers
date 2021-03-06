#pragma once

#include "GuiElement.h"

// Size : 108
class Button : public GuiElement {
public:
	char filler1[60]; // 48

	Button(int, std::string const&, bool);
	Button(int, int, int, std::string const&);
	Button(int, int, int, int, int, std::string const&, bool);
	virtual ~Button() {};
	virtual void tick();
	virtual void tick(MinecraftClient*);
	virtual void render(MinecraftClient*, int, int);
	virtual void topRender(MinecraftClient*, int, int);
	virtual void setupPositions();
	virtual void pointerPressed(MinecraftClient*, int, int);
	virtual void pointerReleased(MinecraftClient*, int, int);
	virtual void focusedMouseClicked(MinecraftClient*, int, int);
	virtual void focusedMouseReleased(MinecraftClient*, int, int);
	virtual void handleButtonPress(MinecraftClient*, short);
	virtual void handleButtonRelease(MinecraftClient*, short);
	virtual void handleTextChar(MinecraftClient*, std::string const&, bool);
	virtual void handleCaretLocation(int);
	virtual void backPressed(MinecraftClient*, bool);
	virtual void setKeyboardHeight(MinecraftClient*, float, Vec2 const&);
	virtual void pointInside(int, int);
	virtual void suppressOtherGUI();
	virtual void setTextboxText(std::string const&);
	virtual void hasFocus() const;
	virtual void setFocus(bool);
	virtual void handleControllerDirectionHeld(int, StickDirection);
	virtual void drawSelected(int);
	virtual void drawPressed(int);
	virtual void drawSliderSelected();
	virtual void onSelectedChanged();
	virtual void hasChildren() const;
	virtual void clicked(MinecraftClient*, int, int);
	virtual void released(int, int);
	virtual void setPressed();
	virtual void setPressed(bool);
	virtual void setMsg(std::string const&);
	virtual void getYImage(bool);
	virtual void renderBg(MinecraftClient*, int, int);
	virtual void renderFace(MinecraftClient*, int, int);
};