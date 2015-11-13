CXX := g++
SRCS := $(wildcard *.cpp) $(wildcard bands/*.cpp)

OBJS := $(addprefix obj/,$(notdir $(SRCS:.cpp=.o)))
PGRM := WAGBands.x
CC_FLAGS := -c

all: $(PGRM)
	echo $(SRCS)

WAGBands.x: $(OBJS)
	$(CXX) -o $(PGRM) $(OBJS)

obj/%.o: %.cpp
	g++ $(CC_FLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(PGRM)
