#pragma once

class PlaybackController {
public:
	PlaybackController();
	Motion loadMotionFrom(string);
	bool playMotion(Motion); // play from currentFrame to end

private:
	bool playing;
	bool stepThrough;
	float FrameRate;
	bool voiceControl;
	bool suitActive;
	int currentFrame;
	Time timeToHoldFinalFrame;
	Time stepThroughInterval;
	float stepThroughTolderance;
};
