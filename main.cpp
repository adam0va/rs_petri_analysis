#include "petri_net/petri_net.hpp"
#include "distributed_system/distributed_system.hpp"

int main() {
	DistributedSystem *ds = new DistributedSystem();
	ds->getDescritpionFromFile("templates/ds/ds1.json");
	ds->printDistributedSystem();

	PetriNet *pn = new PetriNet();
	pn->getDescritpionFromFile("templates/pn/net1.json");
	pn->makeDotFile();

	return 0;
}