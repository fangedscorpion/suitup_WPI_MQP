#pragma once
#include <map>

class PositionSnapshot {
public:
	PositionSnapshot();
	std::map<BandType, Pose> getSnapshot() { return snapshot; }
	Pose getPositionOf(BandType b) { return snapshot[b]; }
private:
	std::map<BandType, Pose> snapshot;
};
