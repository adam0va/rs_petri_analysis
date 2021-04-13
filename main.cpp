#include "petri_net/petri_net.hpp"
#include "distributed_system/distributed_system.hpp"

int main() {
	/*DistributedSystem *ds = new DistributedSystem();
	printf("hello\n");
	ds->getDescritpionFromFile("templates/ds/ds1.json");
    printf("hello\n");
    ds->makeDotFile();
	ds->makePnRepresentation();
	//ds->printDistributedSystem();
	ds->visualizePn();*/

	PetriNet *pn = new PetriNet();
	pn->getDescritpionFromFile("templates/pn/server_with_db.json");
	pn->makeDotFile("");
	pn->visualize();
    Transition *transition = pn->canMakeStep(true);
    if (transition) {
        transition->printTransition();
        pn->makeStep(transition);
        transition = pn->canMakeStep(true);
        transition->printTransition();
        pn->makeStep(transition);
        pn->makeDotFile("");
        pn->visualize();
    }

	return 0;
}