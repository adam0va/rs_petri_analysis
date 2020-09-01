#include "petri_net.hpp"
#include <iostream>
#include <regex>

std::string convertToString(char* a, int size) { 
    int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
} 

std::string makeString(const char* a, int size) {
	int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
}

std::string Vertex::getName() {
	return name;
}

void Vertex::setName(std::string name_) {
	name = name_;
	return;
}

void Place::printPlace() {
	std::cout << "name: " << name << std::endl;
	std::cout << "tokens: " << tokens << std::endl;
	std::cout << "-----\n";
}

void Transition::printTransition() {
	std::cout << "name: " << name << std::endl;
	std::cout << "-----\n";
}

void Arc::printArc() {
	std::cout << "from: " << from.name << std::endl;
	std::cout << "to: " << to.name << std::endl;
	std::cout << "mark: " << mark << std::endl;
	std::cout << "-----\n";
}

PetriNet::PetriNet() {
	std::vector<Place> places;
	std::vector<Transition> transitions;
	std::vector<Arc> arcs;
}

std::vector<Place> PetriNet::getPlaces() {
	return places;
}

std::vector<Transition> PetriNet::getTransitions() {
	return transitions;
}

std::vector<Arc> PetriNet::getArcs() {
	return arcs;
}

int PetriNet::addPlace(char* name) { // добавить проверку правильности написания имени
	Place newPlace;
	newPlace.name = convertToString(name, sizeof(name) / sizeof(char));
	places.push_back(newPlace);
	return 1;
}

int PetriNet::addPlace(const char* name) { // добавить проверку правильности написания имени
	Place newPlace;
	newPlace.name = makeString(name, sizeof(name) / sizeof(char));
	places.push_back(newPlace);
	return 1;
}

int PetriNet::addPlace(char* name, int mark) { // добавить проверку правильности написания имени
	Place newPlace;
	newPlace.name = convertToString(name, sizeof(name) / sizeof(char));
	newPlace.tokens = mark;
	places.push_back(newPlace);
	return 1;
}

int PetriNet::addPlace(const char* name, int mark) { // добавить проверку правильности написания имени
	Place newPlace;
	newPlace.name = makeString(name, sizeof(name) / sizeof(char));
	newPlace.tokens = mark;
	places.push_back(newPlace);
	return 1;
}

int PetriNet::addTransition(char* name) {
	Transition newTransition;
	newTransition.name = convertToString(name, sizeof(name) / sizeof(char));
	transitions.push_back(newTransition);
	return 1;
}

int PetriNet::addArc(const char* from, const char* to, int mark) {
	Arc newArc;
	const std::regex pVertexRegex("p[0-9]+"); // регулярное выражение для записи вершины
	const std::regex tVertexRegex("t[0-9]+");
	std::string fromString = makeString(from, strlen(from));
	std::string toString = makeString(to, strlen(to));
	if (std::regex_match(toString, pVertexRegex) && std::regex_match(fromString, tVertexRegex)) {
		Transition from_;
		Place to_;
		from_.name = fromString;
		to_.name = toString;
		newArc.from = from_;
		newArc.to = to_;
		newArc.mark = mark;
		arcs.push_back(newArc);
	} else
	if (std::regex_match(toString, tVertexRegex) && std::regex_match(fromString, pVertexRegex)) {
		Place from_;
		Transition to_;
		from_.name = fromString;
		to_.name = toString;
		newArc.from = from_;
		newArc.to = to_;
		newArc.mark = mark;
		arcs.push_back(newArc);
	} else {
		return -1;
	}
}

int PetriNet::addArc(char* from, char* to, int mark) {
	Arc newArc;
	const std::regex pVertexRegex("p[0-9]+"); // регулярное выражение для записи вершины
	const std::regex tVertexRegex("t[0-9]+");
	std::string fromString = convertToString(from, strlen(from));
	std::string toString = convertToString(to, strlen(to));
	if (std::regex_match(toString, pVertexRegex) && std::regex_match(fromString, tVertexRegex)) {
		Transition from_;
		Place to_;
		from_.name = fromString;
		to_.name = toString;
		newArc.from = from_;
		newArc.to = to_;
		newArc.mark = mark;
		arcs.push_back(newArc);
	} else
	if (std::regex_match(toString, tVertexRegex) && std::regex_match(fromString, pVertexRegex)) {
		Place from_;
		Transition to_;
		from_.name = fromString;
		to_.name = toString;
		newArc.from = from_;
		newArc.to = to_;
		newArc.mark = mark;
		arcs.push_back(newArc);
	} else {
		return -1;
	}
}

void PetriNet::printPetriNet() {
	std::cout << "PETRI NET:\n";
	if (!places.empty()) {
		std::cout << "Places:\n";
		for (std::vector<Place>::iterator itr = places.begin(); itr < places.end(); ++itr) {
			itr->printPlace();
		}
	}
	if (!transitions.empty()) {
		std::cout << "Transitions:\n";
		for (std::vector<Transition>::iterator itr = transitions.begin(); 
			itr < transitions.end(); ++itr) {
			itr->printTransition();
		}
	}
	if (!arcs.empty()) {
		std::cout << "Arcs:\n";
		for (std::vector<Arc>::iterator itr = arcs.begin(); itr < arcs.end(); ++itr) {
			itr->printArc();
		}
	}
}









