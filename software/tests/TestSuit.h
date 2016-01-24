#include <cxxtest/TestSuite.h>
#include <cstddef>
#include "../bands/Band.h"
#include "../Suit.h"


class TestSuit : public CxxTest::TestSuite
{
public:
  void testCheckBands( void )
  {
    Suit s = Suit();

    Band *b1 = s.getBand(LEFT_HAND);
    TS_ASSERT_EQUALS(b1->getType(), LEFT_HAND);
    TS_ASSERT_EQUALS(b1->numDependentBands(), 0);

    Band *b2 = s.getBand(LEFT_LOWER_ARM);
    TS_ASSERT_EQUALS(b2->getType(), LEFT_LOWER_ARM);
    TS_ASSERT_EQUALS(b2->getDependentBand(0), s.getBand(LEFT_HAND));

    Band *b3 = s.getBand(LEFT_UPPER_ARM);
    TS_ASSERT_EQUALS(b3->getType(), LEFT_UPPER_ARM);
    TS_ASSERT_EQUALS(b3->getDependentBand(0), s.getBand(LEFT_LOWER_ARM));

    Band *b4 = s.getBand(LEFT_SHOULDER);
    TS_ASSERT_EQUALS(b4->getType(), LEFT_SHOULDER);
    TS_ASSERT_EQUALS(b4->getDependentBand(0), s.getBand(LEFT_UPPER_ARM));

    Band *b5 = s.getBand(RIGHT_HAND);
    TS_ASSERT_EQUALS(b5->getType(), RIGHT_HAND);
    TS_ASSERT_EQUALS(b5->numDependentBands(), 0);

    Band *b6 = s.getBand(RIGHT_LOWER_ARM);
    TS_ASSERT_EQUALS(b6->getType(), RIGHT_LOWER_ARM);
    TS_ASSERT_EQUALS(b6->getDependentBand(0), s.getBand(RIGHT_HAND));

    Band *b7 = s.getBand(RIGHT_UPPER_ARM);
    TS_ASSERT_EQUALS(b7->getType(), RIGHT_UPPER_ARM);
    TS_ASSERT_EQUALS(b7->getDependentBand(0), s.getBand(RIGHT_LOWER_ARM));

    Band *b8 = s.getBand(RIGHT_SHOULDER);
    TS_ASSERT_EQUALS(b8->getType(), RIGHT_SHOULDER);
    TS_ASSERT_EQUALS(b8->getDependentBand(0), s.getBand(CHEST));

    Band *b9 = s.getBand(CHEST);
    TS_ASSERT_EQUALS(b9->getType(), CHEST);
    TS_ASSERT_EQUALS(b9,s.getRefBand());
    TS_ASSERT_EQUALS(b9->getDependentBand(0), s.getBand(RIGHT_SHOULDER));
    TS_ASSERT_EQUALS(b9->getDependentBand(1), s.getBand(LEFT_SHOULDER));
  }
};
