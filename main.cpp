#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "petri_net.hpp"

using namespace rapidjson;


int main() {
	std::string currentLine, buffer;
	std::ifstream fin("net1.json", std::ios_base::in); // открываем файл с JSON'ом
	if (!fin.is_open()) {
		std::cout << "File error" << std::endl;
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
        return 1;
    }

    assert(document.IsObject());
    PetriNet *petriNet = new PetriNet();

    assert(document.HasMember("Places")); // проверяем, есть ли вершины-позиции в JSON'e (если нет, ошибка)
    // добавляем информацию об описанных в JSON'е позициях в нашу сеть Петри
    if (document["Places"].IsObject()) {
    	for (Value::ConstMemberIterator itr = document["Places"].MemberBegin(); itr != document["Places"].MemberEnd();
    		++itr) {
    		assert(itr->name.IsString());
    		assert(itr->value.IsInt());
    		petriNet->addPlace(itr->name.GetString(), itr->value.GetInt());
    	}
    } else 
    if (document["Places"].IsArray()) {
    	for (int i = 0; i < document["Places"].Size(); i++) {
    		assert(document["Places"][i].IsString());
    		petriNet->addPlace(document["Places"][i].GetString());
    	}
    }

	assert(document.HasMember("Transitions")); // проверяем, есть ли вершины-переходы в JSON'e (если нет, ошибка)
    assert(document["Transitions"].IsArray()); // они могут быть представлены только в виде списка
    for (int i = 0; i < document["Transitions"].Size(); i++) {
		assert(document["Transitions"][i].IsString());
		char buf[100];
		sprintf(buf, "%s", document["Transitions"][i].GetString());
		petriNet->addTransition(buf);
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

			petriNet->addArc(from, to, mark);
		} else {
			if (petriNet->addArc(from, to, 1) == -1) {
				std::cout << "something went wrong\n";
			}
		}
	}
    
    petriNet->printPetriNet();

}