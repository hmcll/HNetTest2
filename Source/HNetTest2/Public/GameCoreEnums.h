#pragma once
#include "Core.h"

UENUM()
enum class ECardType :int32 {
	Delete,
	CoreObject,
	TrojanHorse,
	FakeTarget,
	Trap,
};

UENUM()
enum class ERoomState {
	Waiting,
	Gaming,
	Ended
};