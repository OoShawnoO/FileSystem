src = $(wildcard ./sources/*.cpp)
objs = $(patsubst %.cpp,%.o,$(src))
INCLUDE = -I ./headers
NOWARNING = -w
STD = -std=c++17
target = filesystem


$(target):$(objs)
	$(CXX) $(STD) $(objs) -o $(target) $(NOWARNING)
	

%.o:%.cpp
	$(CXX) $(STD) -c $< -o $(@) $(INCLUDE) $(NOWARNING)

clean:
	rm $(objs) -f
	clear

clear:
	clear

main:$(target) clear