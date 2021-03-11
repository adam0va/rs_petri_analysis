#include "petri_net/petri_net.hpp"
#include "distributed_system/distributed_system.hpp"

int main() {
	DistributedSystem *ds = new DistributedSystem();
	printf("hello\n");
	ds->getDescritpionFromFile("templates/ds/ds1.json");
    printf("hello\n");
    ds->makeDotFile();
	ds->makePnRepresentation();
	ds->printDistributedSystem();
	ds->getPnRepresentation()->visualize(ds->getDotFileName(), "sss.pdf");

	/*PetriNet *pn = new PetriNet();
	pn->getDescritpionFromFile("templates/pn/simple_server.json");
	pn->makeDotFile();*/

	return 0;
}