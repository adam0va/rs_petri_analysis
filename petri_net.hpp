#ifndef PETRI_NET_H
#define PETRI_NET_H

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
	Vertex *from, *to;
	int mark;
public:
	void printArc();
	friend class PetriNet;
};

class PetriNet {
	std::vector<Place*> places;
	std::vector<Transition*> transitions;
	std::vector<Arc*> arcs;
	std::string dotFileName;
	std::string name;
public:
	PetriNet();
	std::vector<Place*> getPlaces();
	std::vector<Transition*> getTransitions();
	std::vector<Arc*> getArcs();

	int addPlace(std::string, int);
	int addTransition(std::string);
	int addArc(Vertex *from, Vertex *to, int mark);
	int addVertex(char*);

	void addPlacesFromJSONArray();

	Place *findPlaceByName(std::string name);
	Transition *findTransitionByName(std::string name);

	void getDescritpionFromFile(char *filename);
	void parseName(std::string name);
	void printPetriNet();
	void makeDotFile();
	bool visualize();
};

std::string convertToString(char* a, int size);

std::string makeString(const char*);

#endif