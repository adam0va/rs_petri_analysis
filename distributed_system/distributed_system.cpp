#include "../rapidjson/document.h"     // rapidjson's DOM-style API
#include "../rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include "distributed_system.hpp"

using namespace rapidjson;

Server::Server(std::string name) {
	this->name = name;
}

std::string Server::getName() {
	return this->name;
}

std::vector<Server*> Server::getServerConnections() {
	return this->serverConnections;
}

std::vector<DataBase*> Server::getDbConnections() {
	return this->dbConnections;
}

PetriNet *Server::getPnRepresentation() {
    return this->pnRepresentation;
}

void Server::addConnection(Server *server) {
	this->serverConnections.push_back(server);
}

void Server::addConnection(DataBase *db) {
	this->dbConnections.push_back(db);
}

void Server::printServer() {
	printf("Server %s\n", this->name.c_str());
	printf("\tConnected to servers: ");
	for (int i = 0; i < this->serverConnections.size(); i++) {
		printf("%s, ", this->serverConnections[i]->name.c_str());
	}
	printf("\n\tConnected to databases: ");
	for (int i = 0; i < this->dbConnections.size(); i++) {
		printf("%s, ", this->dbConnections[i]->getName().c_str());
	}
	printf("\n");
}

DataBase::DataBase(std::string name) {
	this->name = name;
}

void DataBase::printDb() {
	printf("Database %s\n", this->name.c_str());
}

std::string DataBase::getName() {
	return this->name;
}

void DistributedSystem::addServer(std::string name) {
	Server *newServer = new Server(name);
	this->servers.push_back(newServer);
	return;
}

void DistributedSystem::addDataBase(std::string name) {
	DataBase *newDB = new DataBase(name);
	this->dataBases.push_back(newDB);
	return;
}

PetriNet* DistributedSystem::getPnRepresentation() {
    return this->petriNetRepresentation;
}

std::string DistributedSystem::getDotFileName() {
    return this->dotFileName;
}

Server* DistributedSystem::findServerByName(std::string name) {
	for (int i = 0; i < this->servers.size(); i++) {
		if (strcmp(name.c_str(), this->servers[i]->getName().c_str()) == 0)
			return this->servers[i];
	}
	return NULL;
}   

DataBase* DistributedSystem::findDbByName(std::string name) {
	for (int i = 0; i < this->dataBases.size(); i++) {
		if (strcmp(name.c_str(),this->dataBases[i]->getName().c_str()) == 0)
			return this->dataBases[i];
	}
	return NULL;
}

void DistributedSystem::parseName(std::string name) {
	for (int i = 0; i < name.length(); i++) {
		if (name[i] != '.') 
			this->name.push_back(name[i]);
		else 
			return;
	}
	return;
}

void DistributedSystem::getDescritpionFromFile(const char *filename) {
	std::string currentLine, buffer;
	std::ifstream fin(filename, std::ios_base::in); 
	if (!fin.is_open()) {
		std::cout << "File error" << std::endl;
	}

	while (std::getline(fin, currentLine)) {
		buffer += currentLine;

	}
	std::cout << buffer << std::endl;
	fin.close(); 

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

    assert(document.HasMember("Servers"));
    assert(document["Servers"].IsArray()); 
    for (int i = 0; i < document["Servers"].Size(); i++) {
		assert(document["Servers"][i].IsString());
		this->addServer(document["Servers"][i].GetString());
	}

	assert(document.HasMember("Databases")); 
    assert(document["Databases"].IsArray()); 
    for (int i = 0; i < document["Databases"].Size(); i++) {
		assert(document["Databases"][i].IsString());
		char buf[100];
		sprintf(buf, "%s", document["Databases"][i].GetString());
		this->addDataBase(buf);
	}

    assert(document.HasMember("Сonnections")); 
    assert(document["Сonnections"].IsArray());
	for (int i = 0; i < document["Сonnections"].Size(); i++) {
		char from[100], to[100];
		assert(document["Сonnections"][i].IsObject());
		
		assert(document["Сonnections"][i]["From"].IsString());
		sprintf(from, "%s", document["Сonnections"][i]["From"].GetString());

		Server *server = this->findServerByName(std::string(from));
		
		assert(document["Сonnections"][i]["To"].IsString());
		sprintf(to, "%s", document["Сonnections"][i]["To"].GetString());
		Server *sTo = this->findServerByName(std::string(to));
		if (sTo) {
			server->addConnection(sTo);
		} else {
			DataBase *dbTo = this->findDbByName(std::string(to));
			server->addConnection(dbTo);
		}
		
	}

    this->parseName(std::string(filename));
    this->makeDotFile();

}

void DistributedSystem::printDistributedSystem() {
	printf("Printing distributed system %s...\n", this->name.c_str());
	printf("Databases:\n");
	for (int i = 0; i < this->dataBases.size(); i++) {
		this->dataBases[i]->printDb();
	}
	printf("\nServers:\n");
	for (int i = 0; i < this->servers.size(); i++) {
		this->servers[i]->printServer();
	}
}

void DistributedSystem::makeDotFile() {
	this->dotFileName = this->name + ".dot";
	std::string content = "digraph distributed_system {\nnode [shape=box];\n";

	for (int i = 0; i < this->servers.size(); i++) {
		content += "\"" + this->servers[i]->getName() +"\"\n";
		for (int j = 0; j < this->servers[i]->getServerConnections().size(); j++) {
			content += "\"";
			content += this->servers[i]->getName();
			content += "\" -> \"";
			content += this->servers[i]->getServerConnections()[j]->getName();
			content += "\"\n";
		}
		for (int j = 0; j < this->servers[i]->getDbConnections().size(); j++) {
			content += "\"";
			content += this->servers[i]->getName();
			content += "\" -> \"";
			content += this->servers[i]->getDbConnections()[j]->getName();
			content += "\"\n";
		}
	}
	for (int i = 0; i < this->dataBases.size(); i++) {
		content += "\"";
		content += this->dataBases[i]->getName() +"\"\n";
	}
	content.push_back('}');

	std::ofstream out(this->dotFileName.c_str());
    out << content;
    out.close();
}

bool DistributedSystem::visualize(std::string dotfilePath, std::string outputPath){
    std::string command = "dot -Tpdf " + dotfilePath + " -o " + outputPath;
    system(command.c_str());
    /*
    GVC_t *gvc;
    Agraph_t *g;
    FILE *fp;
    gvc = gvContext();
    fp = fopen(this->dotFileName.c_str(), "r");
    g = agread(fp, 0);
    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "png", fopen((this->name+".png").c_str(), "w"));
    gvFreeLayout(gvc, g);
    agclose(g);
    return (gvFreeContext(gvc));*/
}

PetriNet* DistributedSystem::getPnDescription(Server *s) {
    PetriNet *pn = new PetriNet();
    if (s->getServerConnections().empty() && s->getDbConnections().empty()) {
        pn->getDescritpionFromFile("templates/pn/simple_server.json");
        return pn;
    }
    if (s->getServerConnections().empty() && !s->getDbConnections().empty()) {
        pn->getDescritpionFromFile("templates/pn/server_with_db.json");
        return pn;
    } else {
        pn->getDescritpionFromFile("templates/pn/server_with_connections.json");
        return pn;
    }
}

void DistributedSystem::makePnRepresentation() {
    std::vector<PetriNet*> components;
    PetriNet *firstPn = getPnDescription(this->servers[0]);
    for (int i = 1; i < this->servers.size(); i++) {
        components.push_back(getPnDescription(this->servers[i]));
    }
    firstPn->parallelJoin(components);
    this->servers[0]->pnRepresentation = firstPn;
    for (int i = 1; i < this->servers.size(); i++) {
        this->servers[i]->pnRepresentation = components[i-1];
    }
    for (int i = 0; i < this->servers.size(); i++) {
        for (int j = 0; j < this->servers[i]->serverConnections.size(); j++) {
            Transition *t = servers[i]->getPnRepresentation()->getOutToOtherServer();
            Place *p = servers[i]->serverConnections[j]->getPnRepresentation()->getInFromOtherServer();
            firstPn->addArc(t, p, 1);
            // добавить метки
        }
    }
    printf("22222\n");
    this->petriNetRepresentation = firstPn;
    this->petriNetRepresentation->makeDotFile("templates/pn/DS.dot", false);
}

void DistributedSystem::visualizePn() {
    this->petriNetRepresentation->visualize();
}









