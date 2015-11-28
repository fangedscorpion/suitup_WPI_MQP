# Run make
# Run make tests
# Run make tests2

CC := g++
SRCS := $(wildcard *.cpp) $(wildcard bands/*.cpp)
OBJS := $(addprefix obj/,$(notdir $(subst .cpp,.o,$(SRCS))))
TESTS := $(wildcard Test*.h)
PGRM := WAGBands.x
CC_FLAGS := -c

all: $(PGRM)

WAGBands.x: $(OBJS)
	$(CC) -o $(PGRM) $(OBJS)

obj/%.o: %.cpp
	$(CC) $(CC_FLAGS) -o $@ $<

obj/%.o: bands/%.cpp
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(PGRM)

CXXC = g++
TESTGEN = cxxtestgen
TEST_HEADERS := $(wildcard tests/*.h)
TEST_RUNNERS := $(subst .h,.cpp,$(TEST_HEADERS))
TEST_OBJS := $(addprefix test_obj/,$(notdir $(subst .cpp,.o,$(TEST_RUNNERS))))
CC_FLAGS := -c

# tests: $(TEST_OBJS)
tests: tests/TestSuit.cpp
	echo $(TEST_HEADERS)

tests2: tests.x
	echo Done!

tests.x: test_obj/TestSuit.o obj/Suit.o obj/Band.o obj/Arms.o
	$(CXXC) -o tests.x test_obj/TestSuit.o obj/Suit.o obj/Band.o obj/Arms.o

test_obj/%.o: tests/%.cpp Suit.h bands/Band.h
	$(CXXC) $(CC_FLAGS) -o $@ $<

tests/%.cpp: tests/%.h
	$(TESTGEN) --error-printer -o $@ $< Suit.h bands/Bands.h
