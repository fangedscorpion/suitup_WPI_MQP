#include <cxxtest/TestSuite.h>
#include <cstddef>
#include "../bands/Band.h"
#include "../Suit.h"


class TestSuit : public CxxTest::TestSuite
{
public:
  void testCheckBands( void )
  {
    Quaternion *p = new Quaternion(2.0, 3.0, 4.0, 5.0);
    ShoulderBand *sb = new ShoulderBand(LEFT_SHOULDER);
    TS_ASSERT_EQUALS(sb->moveTo(p), true);
  }
};
