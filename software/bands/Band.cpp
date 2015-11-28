#include "Band.h"
#include <cstddef>

Band::Band(BandType bt) {
	type = bt;
	active = true;
	dependentBand = NULL;
}
