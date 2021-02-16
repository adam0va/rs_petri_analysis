CXX = g++
CXXFLAGS = -Wall -g
SOURCES = petri_net/petri_net.cpp distributed_system/distributed_system.cpp main.cpp
HEADERS = petri_net/petri_net.hpp distributed_system/distributed_system.hpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: petri_net/petri_net.o distributed_system/distributed_system.o main.o
	$(CXX) $^ -o $@

clean:
	rm -rf main1 main all *.o *.dSYM *.pdf *.dot templates/ds/*.pdf templates/pn/*.pdf \
templates/ds/*.dot templates/pn/*.dot *.mk */*.o


-include deps.mk
deps.mk: $(HEADERS) $(SOURCES)
	$(CXX) -MM $(SOURCES) > $@
