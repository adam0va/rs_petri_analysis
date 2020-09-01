CXX = g++
CXXFLAGS = -Wall -g
SOURCES = petri_net.cpp main.cpp 
HEADERS = petri_net.hpp 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main1: petri_net.o main.o 
	$(CXX) $^ -o $@

clean:
	rm -rf main1 *.o


-include deps.mk
deps.mk: $(HEADERS) $(SOURCES)
	$(CXX) -MM $(SOURCES) > $@
