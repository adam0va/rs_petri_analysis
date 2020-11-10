#ifndef DS_H
#define DS_H

#include <vector>
#include <string>

class DataBase {
	std::string name;
public:
	std::string getName();
	DataBase(std::string name);
	void printDb();
};

class Server {
	std::string name;
	std::vector<DataBase*> dbConnections;
	std::vector<Server*> serverConnections;
public:
	Server(std::string name);
	std::string getName();
	std::vector<Server*> getServerConnections();
	std::vector<DataBase*> getDbConnections();
	void addConnection(Server *server);
	void addConnection(DataBase *db);
	void printServer();
};

class DistributedSystem {
	std::string name;
	std::string dotFileName;
	std::vector<Server*> servers;
	std::vector<DataBase*> dataBases;
public:
	void getDescritpionFromFile(char *filename);
	void parseName(std::string name);

	void addServer(std::string name);
	void addDataBase(std::string name);

	Server* findServerByName(std::string name);
	DataBase* findDbByName(std::string name);

	void printDistributedSystem();
	void makeDotFile();
	bool visualize();
};








#endif