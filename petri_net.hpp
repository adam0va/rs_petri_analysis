#include <vector>
#include <string>

class Vertex {
	std::string name;
public:
	std::string getName();
	void setName(std::string name_);
	friend class PetriNet;
	friend class Arc;
	friend class Place;
	friend class Transition;
};

class Place : public Vertex {
	int tokens = 0;
public:
	void printPlace();
	friend class PetriNet;
};

class Transition : public Vertex {
public:
	void printTransition();
	friend class PetriNet;
};

class Arc {
	Vertex from, to;
	int mark;
public:
	void printArc();
	friend class PetriNet;
};

class PetriNet {
	std::vector<Place> places;
	std::vector<Transition> transitions;
	std::vector<Arc> arcs;
public:
	PetriNet();
	std::vector<Place> getPlaces();
	std::vector<Transition> getTransitions();
	std::vector<Arc> getArcs();
	int addPlace(char*);
	int addPlace(const char*);
	int addPlace(char*, int);
	int addPlace(const char*, int);
	int addTransition(char*);
	int addArc(char*, char*, int);
	int addArc(const char*, const char*, int);
	int addVertex(char*);
	void addPlacesFromJSONArray();
	void printPetriNet();
};

std::string convertToString(char* a, int size);

std::string makeString(const char*);