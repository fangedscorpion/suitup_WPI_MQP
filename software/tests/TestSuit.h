#include <cxxtest/TestSuite.h>
#include "../bands/Band.h"
#include "../Suit.h"


class TestSuit : public CxxTest::TestSuite
{
public:
  void testBands( void )
  {
    Suit s = Suit();
    Band *b = s.getBand(LEFT_HAND);
    TS_ASSERT_EQUALS(b->getType(), LEFT_HAND);
    // TS_ASSERT_EQUALS(b->getDependentBand(), s.getBand(LEFT_LOWER_ARM));
    TS_ASSERT_EQUALS(1+1, 2);
  }

  // void testBands2( void )
  // {
  //   Suit s = Suit();
  //   Band *b = s.getBand(LEFT_HAND);
  //   // TS_ASSERT_EQUALS(b->getType(), LEFT_HAND);
  //   TS_ASSERT_EQUALS(b->getDependentBand(), s.getBand(LEFT_LOWER_ARM));
  //   // TS_ASSERT_EQUALS(1+1, 2);
  // }
};

// from other test file
/*int main(int argc, char** argv) {
  Quaternion *p = new Quaternion(2.0, 3.0, 4.0, 5.0);
  Suit s = Suit();
  Band* b = s.getBand(LEFT_LOWER_ARM);
  b->setCalibrationPose(p);
  s.getBand(LEFT_HAND)->getDependentBand()->moveTo(p);
  // printf("Pose x: %f\n", static_cast<Quaternion*>(b->getCalibrationPose())->x);
  // printf("Pose x: %f\n", static_cast<Quaternion*>(s.getBand(LEFT_LOWER_ARM)->getCalibrationPose())->x);

  return 1;
}
*/
