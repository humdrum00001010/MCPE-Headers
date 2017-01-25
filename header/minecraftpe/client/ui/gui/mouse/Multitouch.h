#pragma once

#include "MouseDevice.h"
#include "MouseAction.h"

#include <vector>

class Multitouch {
public:
	static MouseDeivce _pointers[12];
	static std::vector<MouseAction> _inputs;
};