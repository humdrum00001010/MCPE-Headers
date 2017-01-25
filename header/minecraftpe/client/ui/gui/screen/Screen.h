#pragma once

#include "BaseScreen.h"

class MinecraftClient;
enum class MojangConnectionStatus;
class Button;
class GuiElement;
enum class StickDirection;

// Size : 60
class Screen : public BaseScreen {
public:
	enum class OFFSET {
		MINECRAFTCLIENT = 20
	};
	
	char filler[52]; // 8
	
	Screen(MinecraftClient& &);
	virtual ~Screen();
	virtual void _init(int, int);
	virtual void setSize(int, int);
	virtual void onSetKeyboardHeight(float);
	virtual void setAssociatedBlockPos(BlockPos const&);
	virtual void onInternetUpdate();
	virtual void onFocusGained();
	virtual void onFocusLost();
	virtual void terminate();
	virtual void onGameEventNotification(ui::GameEventNotification);
	virtual void _handleDirtyVisualTree(bool);
	virtual void leaveScreen();
	virtual void tick();
	virtual void tick(int, int);
	virtual void updateEvents();
	virtual void applyInput(float);
	virtual void render(ScreenContext&);
	virtual void setupAndRender(UIScreenContext&);
	virtual void handleInputModeChanged(InputMode);
	virtual void handleHoloInputModeChanged(HoloUIInputMode);
	virtual void handleButtonPress(short, FocusImpact);
	virtual void handleButtonRelease(short, FocusImpact);
	virtual void handleRawInputEvent(int, RawInputType, ButtonState, bool);
	virtual void handlePointerLocation(short, short, FocusImpact);
	virtual void handlePointerPressed(bool);
	virtual void handleDirection(DirectionId, float, float, FocusImpact);
	virtual void handleBackEvent(bool);
	virtual void handleTextChar(std::string const&, bool, FocusImpact);
	virtual void handleCaretLocation(int, FocusImpact);
	virtual void setTextboxText(std::string const&);
	virtual void handleLicenseChanged();
	virtual void handleIdentityGained();
	virtual void handleIdentityLost();
	virtual void handleGazeGestureInput(short, float, float, float, FocusImpact);
	virtual void handleDictationEvent();
	virtual void handleCommandEvent(VoiceCommand const&);
	virtual void renderGameBehind() const;
	virtual void absorbsInput() const;
	virtual void closeOnPlayerHurt() const;
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
	virtual void getWidth();
	virtual void getHeight();
	virtual void getRenderingAABB();
	virtual void getNumberOfRenderPasses() const;
	virtual void getEyeRenderingMode() const;
	virtual void getScreenName() const;
	virtual void getScreenNameW() const;
	virtual void getSendEvents();
	virtual void getEventProperties() const;
	virtual void processBufferedTextCharEvents(std::vector<TextCharEventData, std::allocator<TextCharEventData> > const&);
	virtual void toGUICoordinate(int, int, int&, int&);
	virtual void render(int, int, float);
	virtual void init();
	virtual void setupPositions();
	virtual void checkForPointerEvent();
	virtual void controllerEvent();
	virtual void renderBackground(int);
	virtual void renderDirtBackground();
	virtual void renderMenuBackground(float);
	virtual void confirmResult(bool, int);
	virtual void supppressedBySubWindow();
	virtual void onTextBoxUpdated(int);
	virtual void onMojangConnectorStatus(MojangConnectionStatus);
	virtual void handleButtonPress(short);
	virtual void handleButtonRelease(short);
	virtual void handleTextChar(std::string const&, bool);
	virtual void handleCaretLocation(int);
	virtual void handlePointerLocation(short, short);
	virtual void handleDirection(DirectionId, float, float);
	virtual void handleScrollWheel(float);
	virtual void handleGazeGestureInput(short, float, float, float);
	virtual void _handlePointerAction(int, int, bool);
	virtual void _updateTabButtonSelection();
	virtual void _buttonClicked(Button&);
	virtual void _guiElementClicked(GuiElement&);
	virtual void _pointerPressed(int, int);
	virtual void _pointerReleased(int, int);
	virtual void _controllerDirectionChanged(int, StickDirection);
	virtual void _controllerDirectionHeld(int, StickDirection);
};