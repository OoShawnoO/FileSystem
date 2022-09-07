src = $(wildcard ./sources/*.cpp)
objs = $(patsubst %.cpp,%.o,$(src))
INCLUDE = -I ./headers
target = main

$(target):$(objs)
	$(CXX) $(objs) -o $(target)

%.o:%.cpp
	$(CXX) -c $< -o $(@) $(INCLUDE)

clean:
	rm $(objs) -f
	clear