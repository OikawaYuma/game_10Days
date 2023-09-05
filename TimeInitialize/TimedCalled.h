#pragma once
#include<functional>

/// <summary>
/// 時限発動
/// </summary>
class TimedCalled {
public:
	// コンストラクタ
	TimedCalled(std::function<void(int)> hr,uint32_t time);
	// 更新
	void Update();
	// 完了ならtrueを返す
	bool IsFinished() { return TimeCall = true; };

	bool GetTimeCall() { return TimeCall; };

private:
	// コールバック
	std::function<void(int)> hr_;
	//残り時間
	uint32_t time_;
	// 完了フラグ
	bool TimeCall = false;

};
