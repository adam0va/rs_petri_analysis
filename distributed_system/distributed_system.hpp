#ifndef DS_H
#define DS_H

#include <vector>
#include <string>
#include "../petri_net/petri_net.hpp"

class DataBase {
	std::string name;
public:
	std::string getName();
	DataBase(std::string name);
	void printDb();
};

enum ServerTypes {
    APP_SERVER,
    PROXY_SERVER,
    WEB_SERVER
};

class Server {
	std::string name;
	ServerTypes type;
	std::vector<DataBase*> dbConnections;
	std::vector<Server*> serverConnections;
	PetriNet *pnRepresentation;
public:
	Server(std::string &name, ServerTypes type = APP_SERVER);
	std::string getName();

	std::vector<Server*> getServerConnections();
	std::vector<DataBase*> getDbConnections();
	PetriNet *getPnRepresentation();

	void addConnection(Server *server);
	void addConnection(DataBase *db);

	void printServer();
	friend class DistributedSystem;
};

class DistributedSystem {
	std::string name;
	std::string dotFileName;
	std::string outPutFile;
	std::vector<Server*> servers;
	std::vector<DataBase*> dataBases;
    PetriNet* petriNetRepresentation;
public:
	void getDescritpionFromFile(const char *filename);
	void parseName(std::string name);
	PetriNet* getPnRepresentation();

	void addServer(std::string name, ServerTypes type = APP_SERVER);
	void addDataBase(std::string name);

	Server* findServerByName(std::string name);
	DataBase* findDbByName(std::string name);

	void printDistributedSystem();
	void makeDotFile();
    std::string getDotFileName();
	bool visualize(std::string dotfilePath, std::string outputPath);
	void visualizePn();
	PetriNet *getPnDescription(Server*);
    void makePnRepresentation();

};

#endif