#include "petri_net/petri_net.hpp"
#include "distributed_system/distributed_system.hpp"

int main() {
	DistributedSystem *ds = new DistributedSystem();
	printf("hello\n");
	ds->getDescritpionFromFile("templates/ds/ds1.json");
    printf("hello\n");
	ds->makePnRepresentation();
	ds->printDistributedSystem();

	/*PetriNet *pn = new PetriNet();
	pn->getDescritpionFromFile("templates/pn/simple_server.json");
	pn->makeDotFile();*/

	return 0;
}