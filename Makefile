CXX = g++
CXXFLAGS = -Wall -g
SOURCES = petri_net.cpp distributed_system.cpp main.cpp 
HEADERS = petri_net.hpp distributed_system.hpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main: petri_net.o distributed_system.o main.o 
	$(CXX) $^ -o $@

clean:
	rm -rf main1 main *.o *.dSYM


-include deps.mk
deps.mk: $(HEADERS) $(SOURCES)
	$(CXX) -MM $(SOURCES) > $@
