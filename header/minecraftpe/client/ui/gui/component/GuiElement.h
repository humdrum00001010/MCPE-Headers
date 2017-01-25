#pragma once

#include "GuiComponent.h"

#include <string>

class MinecraftClient;
class Vec2;
enum class StickDirection;

// Size : 48
class GuiElement : public GuiComponent {
public:
	char filler[44]; // 4
	
	GuiElement(bool, bool, int, int, int, int);
	virtual ~GuiElement();
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
};