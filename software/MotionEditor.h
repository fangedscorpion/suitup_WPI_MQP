#pragma once

class MotionEditor {
public:
	MotionEditor();
	Motion recordMotion(float frameRate, Suit s);
	PositionSnapshot recordSnapshot(Suit s);
	Motion loadMotionFrom(string);
	void saveMotionTo(string);
};
