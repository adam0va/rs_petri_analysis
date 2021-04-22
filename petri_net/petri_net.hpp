#ifndef PETRI_NET_H
#define PETRI_NET_H

#include <vector>
#include <string>
//#include "../distributed_system/distributed_system.hpp"

class Vertex {
	std::string name;
public:
    virtual ~Vertex() {};
	std::string getName();
	void setName(std::string name_);
	friend class PetriNet;
	friend class Arc;
	friend class Place;
	friend class Transition;

    virtual void changeName(std::string s) {}
};

class Place : public Vertex {
	int tokens;
	std::vector<int> netTokensNumber;
public:
    Place();
    ~Place();
	void printPlace();
    bool checkName(std::string);
    void changeName(std::string s);
    int getTokens();

	friend class PetriNet;
    friend class Arc;
};

class Transition : public Vertex {
    std::string horSyncLabel, vertSyncLabel;
public:
	void printTransition();
    bool checkName(std::string);
    void changeName(std::string s) ;

	friend class PetriNet;
    friend class Arc;
};

class Arc {
	Vertex *from, *to;
	int mark;

public:
	void printArc();
	friend class PetriNet;
};

class Markup {
    std::vector<Place*> places;
    //std::vector<PetriNet*> netTokens;
};

class PetriNet {
	std::vector<Place*> places;
	std::vector<Transition*> transitions;
	std::vector<Arc*> arcs;
	std::vector<PetriNet*> netTokens;

	std::vector<Markup*> markups;

	std::string dotFileName;
	std::string name;

	int lastTransitionNumber;
	int lastPlaceNumber;

    std::vector<Transition*> entrances;
    std::vector<Transition*> exits;

    Transition *outToOtherServer;
    Place *inFromOtherServer;
    Transition *syncFromOtherServerOut;
    Transition *syncFromOtherServerIn;
public:
	PetriNet();
	std::vector<Place*> getPlaces();
	std::vector<Transition*> getTransitions();
	std::vector<Arc*> getArcs();

    Transition *getOutToOtherServer();
    Place *getInFromOtherServer();
    Transition *getSyncFromOtherServerOut();
    Transition *getSyncFromOtherServerIn();

	Place* addPlace(std::string, int);
    Place* addPlace(std::string, PetriNet*);
    Place* addNextPlace();
	Transition* addTransition(std::string, std::string horLabel, std::string vertLabel);
    Transition* addNextTransition();
	int addArc(Place *&from, Transition *&to, int mark);
    int addArc(Transition *&from, Place *&to, int mark);

	Place *findPlaceByName(std::string name);
	Transition *findTransitionByName(std::string name);

    bool hasPlace(std::string);
    bool hasTransition(std::string);
    std::vector<Arc*> findArcs(Place* from);
    std::vector<Arc*> findArcs(Transition* from);
    std::vector<Arc*> findArcsBack(Place* to);
    std::vector<Arc*> findArcsBack(Transition* to);
    Transition *findPairHorTransition(Transition* t);
    std::pair<Transition*, PetriNet*> findPairVertTransition(Transition* t);

    void getDescritpionFromFile(const char *filename);
	void parseName(std::string name);
	void printPetriNet();
	std::string makeDotFile(std::string fn = "", bool isSubgraph = false);
	std::string getDotFileName();
    bool visualize();
	bool visualize(std::string dotfilePath, std::string outputPath);

	void seriesJoin(PetriNet*&);
    void parallelJoin(PetriNet*&);
    void parallelJoin(std::vector<PetriNet*> &pns);

    void rename(std::string);
    void rename(int places, int transitions);

    void findExits();
    void findEntrances();

    bool checkTransitionWithSynchronization(Transition*);
    Transition* canMakeStep(bool checkSync = false);
    bool canMakeStep(Transition* t, bool checkSync = false);
    void makeStep(Transition*, bool sync = false);
};

class NestedPetriNet : public PetriNet {

};

#endif