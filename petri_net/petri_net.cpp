#include "petri_net.hpp"
#include <iostream>
#include <regex>
#include "../rapidjson/document.h"     // rapidjson's DOM-style API
#include "../rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <fstream>

using namespace rapidjson;

std::string Vertex::getName() {
	return name;
}

void Vertex::setName(std::string name_) {
	name = name_;
	return;
}

void Place::printPlace() {
	printf("Name: %s, tokens: %d\n", name.c_str(), tokens);
}

bool Place::checkName(std::string name) {
    bool correctName = name[0] == 'p';
    for (int i = 1; i < name.length(); i++)
        correctName &= (name[i] >= '0' && name[i] <= '9');
    return correctName;
}

void Place::changeName(std::string s) {
    this->name.insert(1, s);
}

void Transition::printTransition() {
	printf("Name: %s\n", name.c_str());
}

bool Transition::checkName(std::string name) {
    bool correctName = name[0] == 't';
    for (int i = 1; i < name.length(); i++)
        correctName &= (name[i] >= '0' && name[i] <= '9');
    return correctName;
}

void Transition::changeName(std::string s) {
    this->name.insert(1, s);
}

void Arc::printArc() {
	printf("%s --%d--> %s\n", from->name.c_str(), mark, to->name.c_str());
}

PetriNet::PetriNet() {
	std::vector<Place> places;
	std::vector<Transition> transitions;
	std::vector<Arc> arcs;
	std::string dotFileName = "";
	std::string name = "";
    lastTransitionNumber = 0;
    lastPlaceNumber = 0;
}

std::vector<Place*> PetriNet::getPlaces() {
	return places;
}

std::vector<Transition*> PetriNet::getTransitions() {
	return transitions;
}

std::vector<Arc*> PetriNet::getArcs() {
	return arcs;
}

Place* PetriNet::addPlace(std::string name, int mark) { // добавить проверку правильности написания имени
	Place *newPlace = new Place();
	if (newPlace->checkName(name))
	    newPlace->name = name;
	else
        return NULL;
	newPlace->tokens = mark;
	places.push_back(newPlace);
	lastPlaceNumber++;
	return newPlace;
}

Place* PetriNet::addNextPlace() {
    Place *newPlace = new Place();
    newPlace->name = "s" + std::to_string(++lastPlaceNumber);
    newPlace->tokens = 0;
    places.push_back(newPlace);
    return newPlace;
}

Transition* PetriNet::addTransition(std::string name) {
	Transition *newTransition = new Transition();
	if (newTransition->checkName(name))
        newTransition->name = name;
	else
        return NULL;
	transitions.push_back(newTransition);
	lastTransitionNumber++;
	return newTransition;
}

Transition* PetriNet::addNextTransition() {
    Transition *newTransition = new Transition();
    newTransition->name = "s" + std::to_string(++lastTransitionNumber);
    transitions.push_back(newTransition);
    lastTransitionNumber++;
    return newTransition;
}

bool PetriNet::hasPlace(std::string name) {
    for (int i = 0; i < this->places.size(); i++)
        if (!strcmp(this->places[i]->getName().c_str(), name.c_str()))
            return true;
    return false;
}

bool PetriNet::hasTransition(std::string name) {
    for (int i = 0; i < this->transitions.size(); i++)
        if (!strcmp(this->transitions[i]->getName().c_str(), name.c_str()))
            return true;
    return false;
}

int PetriNet::addArc(Place *&from, Transition *&to, int mark) {
	Arc *newArc = new Arc;
	newArc->from = from;
	newArc->to = to;
	newArc->mark = mark;
	arcs.push_back(newArc);
}

int PetriNet::addArc(Transition *&from, Place *&to, int mark) {
    Arc *newArc = new Arc;
    newArc->from = from;
    newArc->to = to;
    newArc->mark = mark;
    arcs.push_back(newArc);
}

void PetriNet::printPetriNet() {
	printf("Printing Petri net %s...\n", name.c_str());
	if (!places.empty()) {
		printf("Places:\n");
		for (int i = 0; i < places.size(); i++) {
			places[i]->printPlace();
		}
	}
	if (!transitions.empty()) {
		printf("Transitions:\n");
		for (int i = 0; i < transitions.size(); i++) {
			transitions[i]->printTransition();
		}
	}
	if (!arcs.empty()) {
		printf("Arcs\n");
		for (int i = 0; i < arcs.size(); i++) {
			arcs[i]->printArc();
		}
	}
}

Place* PetriNet::findPlaceByName(std::string name) {
	for (int i = 0; i < places.size(); i++) 
		if (strcmp(name.c_str(), places[i]->name.c_str()) == 0) 
			return places[i];	
	return NULL;
}

Transition* PetriNet::findTransitionByName(std::string name) {
	for (int i = 0; i < transitions.size(); i++) 
		if (strcmp(name.c_str(), transitions[i]->name.c_str()) == 0) 
			return transitions[i];	
	return NULL;
}

void PetriNet::makeDotFile() {
	this->dotFileName = this->name + ".dot";
    std::ofstream out(this->dotFileName.c_str());
	out << "digraph distributed_system {\nlayout=neato\nnode [shape=circle];  ";
	for (int i = 0; i < places.size(); i++) {
		out << places[i]->name;
		out << "; ";
	}
	out << "\nnode [shape=box,fixedsize=true,width=0.9];  ";
	for (int i = 0; i < transitions.size(); i++) {
		out << transitions[i]->name;
		out << "; ";
	}
	out << "\n";
	for (int i = 0; i < arcs.size(); i++) {
		out << arcs[i]->from->name;
        out << "->";
        out << arcs[i]->to->name;
        if (arcs[i]->mark != 1) {
            out << " [label = ";
            out << arcs[i]->mark;
            out << " ]";
        }
        out << ";\n";
	}
    out << "overlap=false\nfontsize=12;\n}";
    out.close();
}

bool PetriNet::visualize() {

}

void PetriNet::parseName(std::string name) {
	for (int i = 0; i < name.length(); i++) {
		if (name[i] != '.') 
			this->name.push_back(name[i]);
		else 
			return;
	}
	return;
}

void PetriNet::findExits() {
    for (int i = 0; i < transitions.size(); i++) {
        bool flag = false;
        for (int j = 0; j < arcs.size(); j++) {
            if (transitions[i]->getName() == arcs[j]->from->getName()) {
                flag = true;
            }
        }
        if (!flag)
            this->exits.push_back(transitions[i]);
    }
}

void PetriNet::findEntrances() {
    for (int i = 0; i < transitions.size(); i++) {
        bool flag = false;
        for (int j = 0; j < arcs.size(); j++) {
            if (transitions[i]->getName() == arcs[j]->to->getName()) {
                flag = true;
            }
        }
        if (!flag) {
            this->entrances.push_back(transitions[i]);
            break;
        }
    }
}

void PetriNet::getDescritpionFromFile(const char *filename) {
	std::string currentLine, buffer;
	std::ifstream fin(filename, std::ios_base::in); // открываем файл с JSON'ом
	if (!fin.is_open()) {
		std::cout << "File error" << std::endl;
		return;
	}

	while (std::getline(fin, currentLine)) { // записываем в строку JSON
		buffer += currentLine;
	}
	fin.close(); // закрываем файл с JSON'ом

	int jsonStringLen = buffer.length(); 
	char jsonCharArray[jsonStringLen+1];
	std::strcpy(jsonCharArray, buffer.c_str());
	for (int i = 0; i < jsonStringLen; i++) 
        std::cout << jsonCharArray[i];
    std::cout << std::endl;

    Document document;

    if (document.ParseInsitu(jsonCharArray).HasParseError()) {
    	std::cout << "Parse error" << std::endl;
        return;
    }

    assert(document.IsObject());

    assert(document.HasMember("Places")); // проверяем, есть ли вершины-позиции в JSON'e (если нет, ошибка)
    // добавляем информацию об описанных в JSON'е позициях в нашу сеть Петри
    if (document["Places"].IsObject()) {
    	for (Value::ConstMemberIterator itr = document["Places"].MemberBegin(); itr != document["Places"].MemberEnd();
    		++itr) {
    		assert(itr->name.IsString());
    		assert(itr->value.IsInt());
    		this->addPlace(std::string(itr->name.GetString()), itr->value.GetInt());
    	}
    } else 
    if (document["Places"].IsArray()) {
    	for (int i = 0; i < document["Places"].Size(); i++) {
    		assert(document["Places"][i].IsString());
    		this->addPlace(std::string(document["Places"][i].GetString()), 0);
    	}
    }

	assert(document.HasMember("Transitions")); // проверяем, есть ли вершины-переходы в JSON'e (если нет, ошибка)
    assert(document["Transitions"].IsArray()); // они могут быть представлены только в виде списка
    for (int i = 0; i < document["Transitions"].Size(); i++) {
		assert(document["Transitions"][i].IsString());
		char buf[100];
		sprintf(buf, "%s", document["Transitions"][i].GetString());
		this->addTransition(std::string(buf));
	}

    assert(document.HasMember("Arcs")); // проверяем, есть ли дуги в JSON'e (если нет, ошибка)
    assert(document["Arcs"].IsArray());
	for (int i = 0; i < document["Arcs"].Size(); i++) {
		//char from[100], to[100];
		int mark = 0;
		assert(document["Arcs"][i].IsObject());
		
		assert(document["Arcs"][i]["From"].IsString());
		assert(document["Arcs"][i]["To"].IsString());

		Transition *fromT = findTransitionByName(std::string(document["Arcs"][i]["From"].GetString()));
		Transition *toT = NULL;
		Place *fromV = NULL, *toV = NULL;
		if (fromT) {
			toV = findPlaceByName(std::string(document["Arcs"][i]["To"].GetString()));
			if (!toV) {
				printf("No such place or position: %s, %s", document["Arcs"][i]["From"].GetString(),
				 document["Arcs"][i]["To"].GetString());
				return;
			}
		} else {
			fromV = findPlaceByName(std::string(document["Arcs"][i]["From"].GetString()));
			if (!fromV) {
				printf("Troubles with arc: %s, %s\n", document["Arcs"][i]["From"].GetString(), 
					document["Arcs"][i]["To"].GetString());
				return;
			}

			toT = findTransitionByName(std::string(document["Arcs"][i]["To"].GetString()));
			if (!toT) {
				printf("Troubles with arc: %s, %s\n", document["Arcs"][i]["From"].GetString(), 
					document["Arcs"][i]["To"].GetString());
				return;
			}
		}
		
		if (document["Arcs"][i].HasMember("Mark")) {
			assert(document["Arcs"][i]["Mark"].IsInt());
			mark = document["Arcs"][i]["Mark"].GetInt();

			if (fromT)
			    this->addArc(fromT, toV, mark);
			else
                this->addArc(fromV, toT, mark);
		} else {
            if (fromT) {
                if (this->addArc(fromT, toV, 1) == -1)
                    std::cout << "something went wrong\n";

            } else if (this->addArc(fromV, toT, mark) == -1)
                    std::cout << "something went wrong\n";
		}
	}
    
    this->parseName(std::string(filename));
    this->printPetriNet();

    findEntrances();
    findExits();

    printf("entrances: ");
    for (int i = 0; i < entrances.size(); i++) {
        printf("%s ", entrances[i]->getName().c_str());
    }
    printf("\nexits: ");
    for (int i = 0; i < exits.size(); i++) {
        printf("%s ", exits[i]->getName().c_str());
    }
}

void PetriNet::seriesJoin(PetriNet* pn) {
    Place *newPlace = addNextPlace();

    addArc(this->exits[0], newPlace, 1);

    pn->rename(lastPlaceNumber, lastTransitionNumber);
    lastPlaceNumber += pn->lastPlaceNumber;
    lastTransitionNumber += pn->lastTransitionNumber;

    addArc(newPlace, pn->entrances[0], 1);
    this->places.insert(this->places.end(), pn->places.begin(), pn->places.end());
    this->transitions.insert(this->transitions.end(), pn->transitions.begin(), pn->transitions.end());
    this->arcs.insert(this->arcs.end(), pn->arcs.begin(), pn->arcs.end());

    findExits();
    findEntrances();

    makeDotFile();
}

void PetriNet::parallelJoin(PetriNet* pn) {
    Place *newPlace1 = addNextPlace();
    Transition *newTransition1 = addNextTransition();

    pn->rename(lastPlaceNumber, lastTransitionNumber);
    lastPlaceNumber += pn->lastPlaceNumber;
    lastTransitionNumber += pn->lastTransitionNumber;

    addArc(newTransition1, newPlace1, 1);
    addArc(newPlace1, entrances[0], 1);
    addArc(newPlace1, pn->entrances[0], 1);

    this->places.insert(this->places.end(), pn->places.begin(), pn->places.end());
    this->transitions.insert(this->transitions.end(), pn->transitions.begin(), pn->transitions.end());
    this->arcs.insert(this->arcs.end(), pn->arcs.begin(), pn->arcs.end());

    findExits();
    findEntrances();

    makeDotFile();
}

void PetriNet::parallelJoin(std::vector<PetriNet*> pns) {
    Place *newPlace1 = addNextPlace();
    Transition *newTransition1 = addNextTransition();

    for (int i = 0; i < pns.size(); i++) {
        pns[i]->rename(lastPlaceNumber, lastTransitionNumber);
        lastPlaceNumber += pns[i]->lastPlaceNumber;
        lastTransitionNumber += pns[i]->lastTransitionNumber;
        addArc(newPlace1, pns[i]->entrances[0], 1);
        this->places.insert(this->places.end(), pns[i]->places.begin(), pns[i]->places.end());
        this->transitions.insert(this->transitions.end(), pns[i]->transitions.begin(), pns[i]->transitions.end());
        this->arcs.insert(this->arcs.end(), pns[i]->arcs.begin(), pns[i]->arcs.end());
    }

    addArc(newTransition1, newPlace1, 1);
    addArc(newPlace1, entrances[0], 1);

    findExits();
    findEntrances();

    makeDotFile();
}

void PetriNet::rename(std::string n) {
    for (int i = 0; i < this->places.size(); i++) {
        this->places[i]->changeName(n + "x");
    }
    for (int i = 0; i < this->transitions.size(); i++) {
        this->transitions[i]->changeName(n + "x");
    }
}

void PetriNet::rename(int placesN, int transitionsN) {
    for (int i = 0; i < this->places.size(); i++) {
        std::string number = this->places[i]->name.substr(1, this->places[i]->name.length() - 1);
        int n = stoi(number) + placesN;
        this->places[i]->setName("p" + std::to_string(n));
    }
    for (int i = 0; i < this->transitions.size(); i++) {
        std::string number = this->transitions[i]->name.substr(1, this->transitions[i]->name.length() - 1);
        int n = stoi(number) + transitionsN;
        this->transitions[i]->setName("t" + std::to_string(n));
    }
}








