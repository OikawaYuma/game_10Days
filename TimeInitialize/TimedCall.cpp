#include "TimedCall.h"
TimedCall::TimedCall(){
};

void TimedCall::Update() {
	if (IsFinished()) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		timerFinish_ = true;
		// コールバック関数呼び出し

	}

};