#include "petri_net/petri_net.hpp"
#include "distributed_system/distributed_system.hpp"
#include <iostream>

int main() {
	DistributedSystem *ds = new DistributedSystem();
	ds->getDescritpionFromFile("templates/ds/ds1.json");
    ds->makeDotFile();
    ds->visualize(ds->getDotFileName(), "");
	ds->makePnRepresentation();
	//ds->printDistributedSystem();
	ds->visualizePn();
    // do you want to run net?
    std::string input;
    bool wantToRun = false, printTrans = false;
    std::cout << "Do you want to run net? y/anything else\n";
    std::cin >> input;
    wantToRun = input == "y";
    std::cout << "Do you want to see transitions that fire? y/anything else\n";
    std::cin >> input;
    printTrans = input == "y";
    // do you want to see transitions that fire?
    if (wantToRun)
	    ds->getPnRepresentation()->runNet(printTrans);

	/*PetriNet *pn = new PetriNet();
	pn->getDescritpionFromFile("templates/pn/server_with_connections.json");
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
    }*/

	return 0;
}