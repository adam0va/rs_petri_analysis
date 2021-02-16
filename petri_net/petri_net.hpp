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
	void changeName(std::string s) {}
};

class Place : public Vertex {
	int tokens;
public:
	void printPlace();
    bool checkName(std::string);
    void changeName(std::string s);

	friend class PetriNet;
};

class Transition : public Vertex {
public:
	void printTransition();
    bool checkName(std::string);
    void changeName(std::string s);

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

	int lastTransitionNumber;
	int lastPlaceNumber;

    std::vector<Transition*> entrances;
    std::vector<Transition*> exits;
public:
	PetriNet();
	std::vector<Place*> getPlaces();
	std::vector<Transition*> getTransitions();
	std::vector<Arc*> getArcs();

	Place* addPlace(std::string, int);
    Place* addNextPlace();
	Transition* addTransition(std::string);
    Transition* addNextTransition();
    //int addArc(Vertex *&from, Vertex *&to, int mark);
	int addArc(Place *&from, Transition *&to, int mark);
    int addArc(Transition *&from, Place *&to, int mark);

	Place *findPlaceByName(std::string name);
	Transition *findTransitionByName(std::string name);

    bool hasPlace(std::string);
    bool hasTransition(std::string);

    void getDescritpionFromFile(const char *filename);
	void parseName(std::string name);
	void printPetriNet();
	void makeDotFile();
	bool visualize();

	void seriesJoin(PetriNet*);
    void parallelJoin(PetriNet*);
    void parallelJoin(std::vector<PetriNet*>);

    void rename(std::string);
    void rename(int places, int transitions);

    void findExits();
    void findEntrances();
};

#endif