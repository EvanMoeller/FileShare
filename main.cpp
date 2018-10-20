#include "Predict.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
	Predict p = Predict(argv[1], argv[2]);
	p.alwaysTaken();
	p.alwaysNotTaken();
	p.bitBimodal();
	p.twoBitBimodal();
	p.Gshare();
	p.tournament();
	p.popOutput();
	return 0;
}
