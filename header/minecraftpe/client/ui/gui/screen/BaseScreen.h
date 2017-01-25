#pragma once

#include "AbstractScreen.h"

#include <memory>

class AbstractScreenSetupCleanupStrategy;

// Size : 8
class BaseScreen : public AbstractScreen {
public:
	// void **vtable; 0
	std::unique_ptr<AbstractScreenSetupCleanupStrategy> strategy; // 4
	
	BaseScreen();
	virtual ~BaseScreen();
	virtual void _init(int, int) = 0;
	virtual void setSize(int, int) = 0;
	virtual void onSetKeyboardHeight(float) = 0;
	virtual void setAssociatedBlockPos(BlockPos const&) = 0;
	virtual void onInternetUpdate() = 0;
	virtual void onFocusGained() = 0;
	virtual void onFocusLost() = 0;
	virtual void terminate() = 0;
	virtual void onGameEventNotification(ui::GameEventNotification);
	virtual void _handleDirtyVisualTree(bool);
	virtual void leaveScreen();
	virtual void tick();
	virtual void tick(int, int) = 0;
	virtual void updateEvents() = 0;
	virtual void applyInput(float);
	virtual void render(ScreenContext&) = 0;
	virtual void setupAndRender(UIScreenContext&);
	virtual void handleInputModeChanged(InputMode) = 0;
	virtual void handleHoloInputModeChanged(HoloUIInputMode) = 0;
	virtual void handleButtonPress(short, FocusImpact) = 0;
	virtual void handleButtonRelease(short, FocusImpact) = 0;
	virtual void handleRawInputEvent(int, RawInputType, ButtonState, bool) = 0;
	virtual void handlePointerLocation(short, short, FocusImpact) = 0;
	virtual void handlePointerPressed(bool) = 0;
	virtual void handleDirection(DirectionId, float, float, FocusImpact) = 0;
	virtual void handleBackEvent(bool) = 0;
	virtual void handleTextChar(std::string const&, bool, FocusImpact) = 0;
	virtual void handleCaretLocation(int, FocusImpact) = 0;
	virtual void setTextboxText(std::string const&) = 0;
	virtual void handleLicenseChanged() = 0;
	virtual void handleIdentityGained() = 0;
	virtual void handleIdentityLost() = 0;
	virtual void handleGazeGestureInput(short, float, float, float, FocusImpact) = 0;
	virtual void handleDictationEvent() = 0;
	virtual void handleCommandEvent(VoiceCommand const&) = 0;
	virtual void renderGameBehind() const = 0;
	virtual void absorbsInput() const = 0;
	virtual void closeOnPlayerHurt() const = 0;
	virtual void isModal() const;
	virtual void isShowingMenu() const;
	virtual void shouldStealMouse() const;
	virtual void screenIsNotFlushable() const;
	virtual void screenDrawsLast() const;
	virtual void getFocusedControl();
	virtual void isWorldViewer() const;
	virtual void isPauseScreen() const;
	virtual void isPlayScreen() const;
	virtual void renderOnlyWhenTopMost() const;
	virtual void lowFreqRendering() const;
	virtual void ignoreAsTop() const;
	virtual void getWidth() = 0;
	virtual void getHeight() = 0;
	virtual void getRenderingAABB();
	virtual void getNumberOfRenderPasses() const;
	virtual void getEyeRenderingMode() const;
	virtual void getScreenName() const;
	virtual void getScreenNameW() const;
	virtual void getSendEvents();
	virtual void getEventProperties() const;
	virtual void processBufferedTextCharEvents(std::vector<TextCharEventData, std::allocator<TextCharEventData> > const&);
	virtual void toGUICoordinate(int, int, int&, int&);
};