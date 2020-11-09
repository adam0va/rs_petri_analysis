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
	std::cout << buffer << std::endl;
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
    //PetriNet *petriNet = new PetriNet();

    assert(document.HasMember("Places")); // проверяем, есть ли вершины-позиции в JSON'e (если нет, ошибка)
    // добавляем информацию об описанных в JSON'е позициях в нашу сеть Петри
    if (document["Places"].IsObject()) {
    	for (Value::ConstMemberIterator itr = document["Places"].MemberBegin(); itr != document["Places"].MemberEnd();
    		++itr) {
    		assert(itr->name.IsString());
    		assert(itr->value.IsInt());
    		this->addPlace(itr->name.GetString(), itr->value.GetInt());
    	}
    } else 
    if (document["Places"].IsArray()) {
    	for (int i = 0; i < document["Places"].Size(); i++) {
    		assert(document["Places"][i].IsString());
    		this->addPlace(document["Places"][i].GetString());
    	}
    }

	assert(document.HasMember("Transitions")); // проверяем, есть ли вершины-переходы в JSON'e (если нет, ошибка)
    assert(document["Transitions"].IsArray()); // они могут быть представлены только в виде списка
    for (int i = 0; i < document["Transitions"].Size(); i++) {
		assert(document["Transitions"][i].IsString());
		char buf[100];
		sprintf(buf, "%s", document["Transitions"][i].GetString());
		this->addTransition(buf);
	}

    assert(document.HasMember("Arcs")); // проверяем, есть ли дуги в JSON'e (если нет, ошибка)
    assert(document["Arcs"].IsArray());
	for (int i = 0; i < document["Arcs"].Size(); i++) {
		char from[100], to[100];
		int mark;
		assert(document["Arcs"][i].IsObject());
		
		assert(document["Arcs"][i]["From"].IsString());
		sprintf(from, "%s", document["Arcs"][i]["From"].GetString());
		
		assert(document["Arcs"][i]["To"].IsString());
		sprintf(to, "%s", document["Arcs"][i]["To"].GetString());
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
    
    this->printPetriNet();
}








