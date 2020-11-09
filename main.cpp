#include "petri_net.hpp"
#include "distributed_system.hpp"

int main() {
	DistributedSystem *ds = new DistributedSystem();
	ds->getDescritpionFromFile("ds1.json");
	ds->printDistributedSystem();

	return 0;
}