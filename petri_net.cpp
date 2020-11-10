#include "petri_net.hpp"
#include <iostream>
#include <regex>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <fstream>

using namespace rapidjson;

std::string convertToString(char* a, int size) { 
    int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s.push_back(a[i]); 
    } 
    return s; 
} 

std::string makeString(const char* a, int size) {
	int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s.push_back(a[i]); 
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
	printf("Name: %s, tokens: %d\n", name.c_str(), tokens);
}

void Transition::printTransition() {
	printf("Name: %s\n", name.c_str());
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

int PetriNet::addPlace(std::string name, int mark) { // добавить проверку правильности написания имени
	Place *newPlace = new Place();
	newPlace->name = name;
	newPlace->tokens = mark;
	places.push_back(newPlace);
	return 1;
}

int PetriNet::addTransition(std::string name) {
	Transition *newTransition = new Transition();
	newTransition->name = name;
	transitions.push_back(newTransition);
	return 1;
}

int PetriNet::addArc(Vertex *from, Vertex *to, int mark) {
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

void PetriNet::getDescritpionFromFile(char *filename) {
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
		int mark;
		assert(document["Arcs"][i].IsObject());
		
		assert(document["Arcs"][i]["From"].IsString());
		assert(document["Arcs"][i]["To"].IsString());

		Vertex *from = findTransitionByName(std::string(document["Arcs"][i]["From"].GetString()));
		Vertex *to = NULL;
		if (from) {
			to = findPlaceByName(std::string(document["Arcs"][i]["To"].GetString()));
			if (!to) {
				printf("No such place or position: %s, %s", document["Arcs"][i]["From"].GetString(),
				 document["Arcs"][i]["To"].GetString());
				return;
			}
		} else {
			from = findPlaceByName(std::string(document["Arcs"][i]["From"].GetString()));
			if (!from) {
				printf("Troubles with arc: %s, %s\n", document["Arcs"][i]["From"].GetString(), 
					document["Arcs"][i]["To"].GetString());
				return;
			}

			to = findTransitionByName(std::string(document["Arcs"][i]["To"].GetString()));
			if (!to) {
				printf("Troubles with arc: %s, %s\n", document["Arcs"][i]["From"].GetString(), 
					document["Arcs"][i]["To"].GetString());
				return;
			}
		}
		
		if (document["Arcs"][i].HasMember("Mark")) {
			assert(document["Arcs"][i]["Mark"].IsInt());
			mark = document["Arcs"][i]["Mark"].GetInt();

			this->addArc(from, to, mark);
		} else {
			if (this->addArc(from, to, 1) == -1) {
				std::cout << "something went wrong\n";
			}
		}
	}
    
    this->parseName(std::string(filename));
    this->printPetriNet();
}








