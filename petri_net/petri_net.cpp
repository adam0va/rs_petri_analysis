#include "petri_net.hpp"
#include <iostream>
#include <regex>
#include "../rapidjson/document.h"     // rapidjson's DOM-style API
#include "../rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <fstream>
#include <graphviz/gvc.h>
#include "../distributed_system/distributed_system.hpp"

using namespace rapidjson;

std::string Vertex::getName() {
	return name;
}

void Vertex::setName(std::string name_) {
	name = name_;
	return;
}

Place::Place() {
    tokens = 0;
}

Place::~Place() {
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

int Place::getTokens() {
    return tokens;
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

Transition *PetriNet::getOutToOtherServer() {
    return this->outToOtherServer;
}

Place *PetriNet::getInFromOtherServer() {
    return this->inFromOtherServer;
}

Transition *PetriNet::getSyncFromOtherServerOut() {
    return this->syncFromOtherServerOut;
}

Transition *PetriNet::getSyncFromOtherServerIn() {
    return this->syncFromOtherServerIn;
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

Place* PetriNet::addPlace(std::string name, PetriNet *net) {
    Place *newPlace = new Place();
    if (newPlace->checkName(name))
        newPlace->name = name;
    else
        return NULL;
    this->netTokens.push_back(net);
    newPlace->netTokensNumber.push_back(netTokens.size() - 1);
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

Transition* PetriNet::addTransition(std::string name, std::string horLabel, std::string vertLabel) {
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

std::vector<Arc*> PetriNet::findArcs(Place* from) {
    std::vector<Arc*> ret;
    for (int i = 0; i < this->arcs.size(); i++) {
        if (arcs[i]->from->getName() == from->getName())
            ret.push_back(arcs[i]);
    }
    return ret;
}

std::vector<Arc*> PetriNet::findArcs(Transition* from) {
    std::vector<Arc*> ret;
    for (int i = 0; i < this->arcs.size(); i++) {
        if (arcs[i]->from->getName() == from->getName())
            ret.push_back(arcs[i]);
    }
    return ret;
}

std::vector<Arc*> PetriNet::findArcsBack(Place* to) {
    std::vector<Arc*> ret;
    for (int i = 0; i < this->arcs.size(); i++) {
        if (arcs[i]->to->getName() == to->getName())
            ret.push_back(arcs[i]);
    }
    return ret;
}

std::vector<Arc*> PetriNet::findArcsBack(Transition* to) {
    std::vector<Arc*> ret;
    for (int i = 0; i < this->arcs.size(); i++) {
        if (arcs[i]->to->getName() == to->getName())
            ret.push_back(arcs[i]);
    }
    return ret;
}

Transition* PetriNet::findPairHorTransition(Transition* t) {
    for (int i = 0; i < transitions.size(); i++) {
        if (transitions[i]->name != t->name &&
            transitions[i]->horSyncLabel == t->horSyncLabel) {
            return transitions[i];
        }
    }
    return NULL;
}

std::pair<Transition*, PetriNet*> PetriNet::findPairVertTransition(Transition* t) {
    for (int i = 0; i < netTokens.size(); i++) {
        for (int j = 0; j < netTokens[i]->transitions.size(); j++) {
            if (netTokens[i]->transitions[j]->name == t->name)
                return std::pair<Transition*, PetriNet*>(netTokens[i]->transitions[j], netTokens[i]);
        }
    }
    return std::pair<Transition*, PetriNet*>(NULL, NULL);
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

std::string PetriNet::makeDotFile(std::string fn, bool isSubgraph) {
    this->dotFileName = fn == "" ? this->name + ".dot" : fn;
    printf("name of PN: %s\n", this->name.c_str());
    std::ofstream out(this->dotFileName.c_str());
    std::vector<std::string> filenames;
    printf("FILENAME: 1%s1\n", dotFileName.c_str());
    if (isSubgraph) {
        out << "subgraph " << "sub" << " {\nstyle=filled;\ncolor=lightgrey;\n";
    } else
	    out << "digraph distributed_system {\nlayout=neato\nnode [shape=circle];  ";
	for (int i = 0; i < places.size(); i++) {
		out << places[i]->name;
		if (places[i]->tokens) {
		    out << " [xlabel =";
            out << places[i]->tokens << " ]";
		}
        std::string file;
		if (!places[i]->netTokensNumber.empty()) {
		    for (int j = 0; j < places[i]->netTokensNumber.size(); j++) {
		        file = this->netTokens[places[i]->netTokensNumber[j]]->makeDotFile("", true);
		        filenames.push_back(file);
		    }
		}
		out << "; ";
		/*if (!places[i]->netTokensNumber.empty()) {
            std::ofstream subgraph_file(file.c_str());
            out << subgraph_file;
            subgraph_file.close();
		}*/
	}

	out << "\nnode [shape=box,fixedsize=true,width=0.9];  ";
	for (int i = 0; i < transitions.size(); i++) {
		out << transitions[i]->name;
		out << "; ";
	}
    out << "\n";
	for (int i = 0; i < filenames.size(); i++) {
        std::ifstream subgraph_file(filenames[i].c_str());
        std::string line;
        //out << subgraph_file;
        while(getline(subgraph_file, line)){
            out << line << "\n";
        }

        subgraph_file.close();
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
    return this->dotFileName;
}

std::string PetriNet::getDotFileName() {
    return this->dotFileName;
}

bool PetriNet::visualize() {
    std::string command = "dot -Tpdf " + this->dotFileName + " -o " + "outputPath.pdf";
    system(command.c_str());
    return false;
}

bool PetriNet::visualize(std::string dotfilePath, std::string outputPath) {
    std::string command = "dot -Tpdf " + dotfilePath + " -o " + outputPath;
    system(command.c_str());
}

void PetriNet::parseName(std::string name) {
	for (int i = 0; i < name.length(); i++) {
		if (name[i] != '.') 
			this->name.push_back(name[i]);
		else 
			return;
	}
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
    int hasElementaryCount = 0;

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
    		if (itr->value.IsInt()) {
                this->addPlace(std::string(itr->name.GetString()), itr->value.GetInt());
    		} else if (itr->value.IsString()) {
    		    PetriNet *en = new PetriNet();
    		    std::string name = std::string(itr->value.GetString());
    		    assert(document.HasMember("Files"));
    		    assert(document["Files"].HasMember(name.c_str()));
                Value::ConstMemberIterator it = document["Files"].FindMember(name.c_str());
                std::string filename = std::string(it->value.GetString());
                en->getDescritpionFromFile(filename.c_str());
                //this->netTokens.push_back(en);
                this->addPlace(std::string(itr->name.GetString()), en);
                printf("number of EN %lu\n", netTokens.size());
    		}
    	}
    } else 
    if (document["Places"].IsArray()) {
    	for (int i = 0; i < document["Places"].Size(); i++) {
    		assert(document["Places"][i].IsString());
    		this->addPlace(std::string(document["Places"][i].GetString()), 0);
    	}
    }

	assert(document.HasMember("Transitions")); // проверяем, есть ли вершины-переходы в JSON'e (если нет, ошибка)
    if (document["Transitions"].IsArray()) {
        for (int i = 0; i < document["Transitions"].Size(); i++) {
            assert(document["Transitions"][i].IsString());
            char buf[100];
            sprintf(buf, "%s", document["Transitions"][i].GetString());
            this->addTransition(std::string(buf), "", "");
        }
    } else {
        for (Value::ConstMemberIterator itr = document["Transitions"].MemberBegin();
        itr != document["Transitions"].MemberEnd(); ++itr) {
            assert(itr->name.IsString());
            assert(itr->value.IsString());
            if (std::string(itr->value.GetString())[0] == 'h') {
                this->addTransition(itr->name.GetString(), itr->value.GetString(), "");
            } else {
                this->addTransition(itr->name.GetString(), "", itr->value.GetString());
            }


        }
    }

    assert(document.HasMember("Arcs")); // проверяем, есть ли дуги в JSON'e (если нет, ошибка)
    assert(document["Arcs"].IsArray());
	for (int i = 0; i < document["Arcs"].Size(); i++) {
		//char from[100], to[100];
		int mark = 1;
		assert(document["Arcs"][i].IsObject());
		
		assert(document["Arcs"][i]["From"].IsString());
		assert(document["Arcs"][i]["To"].IsString());

		Transition *fromT = findTransitionByName(std::string(document["Arcs"][i]["From"].GetString()));
		Transition *toT = NULL;
		Place *fromV = NULL, *toV = NULL;
		if (fromT) {
			toV = findPlaceByName(std::string(document["Arcs"][i]["To"].GetString()));
			if (!toV) {
				printf("No such place or position: %s, %s\n", document["Arcs"][i]["From"].GetString(),
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
		}
        if (fromT)
            this->addArc(fromT, toV, mark);
        else
            this->addArc(fromV, toT, mark);
	}

	if (document.HasMember("OutToOtherServer")) {
	    Transition* t = this->findTransitionByName(
	            std::string(document["OutToOtherServer"].GetString()));
	    this->outToOtherServer = t;
	}
    if (document.HasMember("InFromOtherServer")) {
        Place* p = this->findPlaceByName(
                std::string(document["InFromOtherServer"].GetString()));
        this->inFromOtherServer = p;
    }
    if (document.HasMember("SyncWithOtherServerOut")) {
        Transition* t = this->findTransitionByName(
                std::string(document["SyncWithOtherServerOut"].GetString()));
        this->syncFromOtherServerOut = t;
    }
    if (document.HasMember("SyncWithOtherServerIn")) {
        Transition* t = this->findTransitionByName(
                std::string(document["SyncWithOtherServerIn"].GetString()));
        this->syncFromOtherServerIn = t;
    }

    
    this->parseName(std::string(filename));
    this->printPetriNet();

    findEntrances();
    findExits();

    for (int i = 0; i < this->netTokens.size(); i++) {
        netTokens[i]->rename(lastPlaceNumber, lastTransitionNumber);
        lastTransitionNumber = netTokens[i]->lastTransitionNumber;
        lastPlaceNumber = netTokens[i]->lastPlaceNumber;
    }

    printf("entrances: ");
    for (int i = 0; i < entrances.size(); i++) {
        printf("%s ", entrances[i]->getName().c_str());
    }
    printf("\nexits: ");
    for (int i = 0; i < exits.size(); i++) {
        printf("%s ", exits[i]->getName().c_str());
    }
}

void PetriNet::seriesJoin(PetriNet* &pn) {
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

void PetriNet::parallelJoin(PetriNet* &pn) {
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

void PetriNet::parallelJoin(std::vector<PetriNet*> &pns) {
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
    printf("rename: \n");
    if (this->inFromOtherServer)
        this->inFromOtherServer->printPlace();
    if (this->outToOtherServer)
        this->outToOtherServer->printTransition();
}

bool PetriNet::checkTransitionWithSynchronization(Transition* t) {
    // найти парную
    if (t->horSyncLabel != "") {
        Transition *pairTransition = findPairHorTransition(t);
        return canMakeStep(pairTransition,false);
    } else {
        // найти сеть Петри в которой переход
        std::pair<Transition*, PetriNet*> p = findPairVertTransition(t);
        return p.second->canMakeStep(p.first, false);
    }
}

Transition* PetriNet::canMakeStep(bool checkSync) {
    std::vector<Transition*> possibleTransitions;
    // для каждого перехода найти все дуги, которые в него входят
    for (int i = 0; i < transitions.size(); i++) {
        std::vector<Arc*> arcsIn = findArcsBack(transitions[i]);
        int suit = true;
        for (int j = 0; j < arcsIn.size(); j++) {
            if (checkSync && (transitions[i]->vertSyncLabel != "" || transitions[i]->horSyncLabel != "")) {
                // найти парный переход
                suit = checkTransitionWithSynchronization(transitions[i]);
                // проверить может ли он сработать
            }
            // проверить что для всех входных позиций можно совершить переход
            if (arcsIn[j]->mark > ((Place*)arcsIn[j]->from)->getTokens())
                suit = false;
        }
        if (suit)
            possibleTransitions.push_back(transitions[i]);
    }

    if (possibleTransitions.empty()) {
        return NULL;
    } else {
        printf("size: %lu\n", possibleTransitions.size());
        srand( time( 0 ) );
        int idx = rand() % possibleTransitions.size(); // возвращаем случайную дугу
        printf("%d\n", idx);
        return possibleTransitions[idx];
    }
}

bool PetriNet::canMakeStep(Transition* t, bool checkSync) {
    std::vector<Arc*> arcsIn = findArcsBack(t);
    int suit = true;
    for (int j = 0; j < arcsIn.size(); j++) {
        if (checkSync && (t->vertSyncLabel != "" || t->horSyncLabel != "")) {
            // найти парный переход
            suit = checkTransitionWithSynchronization(t);
            // проверить может ли он сработать
        }
        // проверить что для всех входных позиций можно совершить переход
        if (arcsIn[j]->mark > ((Place*)arcsIn[j]->from)->getTokens())
            suit = false;
    }
    return suit;
}

void PetriNet::makeStep(Transition* t, bool sync) {
    std::vector<Arc*> arcsIn = findArcsBack(t), arcsOut = findArcs(t);
    for (int i = 0; i < arcsIn.size(); i++) {
        ((Place*)arcsIn[i]->from)->tokens -= arcsIn[i]->mark;
    }
    if (sync) {
        if (t->horSyncLabel != "") {
            Transition *pairTransition = findPairHorTransition(t);
            makeStep(pairTransition, false);
        }
        if (t->vertSyncLabel != "") {
            std::pair<Transition*, PetriNet*> p = findPairVertTransition(t);
            p.second->makeStep(p.first, false);
        }
    }
    for (int i = 0; i < arcsOut.size(); i++) {
        ((Place*)arcsOut[i]->to)->tokens += arcsOut[i]->mark;
    }
}








