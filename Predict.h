#ifndef _PREDICT_H
#define _PREDICT_H

#include "Branch.h"
#include <vector>
#include <string>
#include <fstream>


class Predict{
private:
	std::vector<Branch> branches; //getVal(); getAd();

	int sizes[7] = {16, 32, 128, 256, 512, 1024, 2048};

	std::string output;

	int b; //total branches
	std::vector<int> at; //resutlt always taken
	std::vector<int> ant; //result always not taken
	std::vector<int> b1; //result 1 bit bimodal
	std::vector<int> b2; //result 2 bit bimodal
	std::vector<int> g; //result gshare
	std::vector<int> t; //result tournament

public:
	Predict(std::string, std::string);
	void alwaysTaken();
	void alwaysNotTaken();
	void bitBimodal();
	void twoBitBimodal();
	void Gshare();
	void tournament();
	void popOutput();
};

#endif
