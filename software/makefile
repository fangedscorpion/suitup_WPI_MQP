# Confirm that the directories obj/ and test_obj/ exist or make them
# Run 'make'
# Run 'make tests'
CC := g++
HDRS := $(wildcard *.h) $(wildcard, bands/*.h)
SRCS := $(wildcard *.cpp) $(wildcard bands/*.cpp)
OBJS := $(addprefix obj/,$(notdir $(subst .cpp,.o,$(SRCS))))
PGRM := WAGBands.x
CC_FLAGS := -c

all: $(PGRM)

WAGBands.x: $(OBJS)
	$(CC) -o $(PGRM) $(OBJS)

obj/%.o: %.cpp
	$(CC) $(CC_FLAGS) -o $@ $<

obj/%.o: bands/%.cpp
	$(CC) $(CC_FLAGS) -o $@ $<

TEST_HEADERS := $(wildcard tests/*.h)
TEST_OBJS := $(addprefix test_obj/,$(notdir $(subst .h,.o,$(TEST_HEADERS))))
# Test.o has the main function
NON_MAIN_OBJS := $(filter-out obj/Test.o, $(OBJS))
TESTS := tests.x

tests: $(PGRM) $(TESTS)
	./$(TESTS)

tests.x: $(TEST_OBJS) $(NON_MAIN_OBJS)
	$(CC) -o $@ $< $(NON_MAIN_OBJS)

test_obj/%.o: tests/%.cpp
	$(CC) $(CC_FLAGS) -o $@ $<

tests/%.cpp: tests/%.h
	cxxtestgen --error-printer -o $@ $< $(HDRS)

clean:
	rm -f $(OBJS) $(PGRM) $(TESTS) $(TEST_OBJS)
