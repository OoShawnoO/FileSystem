src = $(wildcard ./sources/*.cpp)
objs = $(patsubst %.cpp,%.o,$(src))
INCLUDE = -I ./headers
NOWARNING = -w
target = filesystem


$(target):$(objs)
	$(CXX) $(objs) -o $(target) $(NOWARNING)
	

%.o:%.cpp
	$(CXX) -c $< -o $(@) $(INCLUDE) $(NOWARNING)

clean:
	rm $(objs) -f
	clear

main:$(target) clean