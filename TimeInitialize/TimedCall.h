#pragma once

#include <functional>

/// <summary>
///  時限発動
/// </summary>
class TimedCall {
public:
	// コンストラクタ
	TimedCall();

	// 更新
	void Update();
	// 完了ならtrueを返す
	bool IsFinished() { return timerFinish_; }

private:
	// コールバック

	// 残り時間
	uint32_t time_;
	// 完了フラグ
	bool timerFinish_ = false;
};
