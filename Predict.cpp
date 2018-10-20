#include "Predict.h"
#include <string>
#include <fstream>
#include <math.h>
#include <vector>
#include <sstream>
#include "Branch.h"
#include <iostream>

using namespace std;


Predict::Predict(string in, string o){
	this->output = o;
	this->b = 0;

	ifstream input;
	input.open(in.c_str());

	string ad, val;
	unsigned int a;

	while(input >> ad && input >> val){
		stringstream s;
	
		this->b++;
		ad = ad.substr(2);
		s << ad;
    	s >> hex >> a;
		Branch b = Branch(a, val);
		branches.push_back(b);
	}
}

void Predict::alwaysTaken(){
	int count = 0;
	for(int i=0; i<branches.size(); i++){
		if(branches[i].getVal() == "T"){
			count++;
		}
	}
	this->at.push_back(count);
}

void Predict::alwaysNotTaken(){
	int count = 0;
	for(int i=0; i<branches.size(); i++){
		if(branches[i].getVal() != "T"){
			count++;
		}
	}
	this->ant.push_back(count);
}

void Predict::bitBimodal(){
	for(int i =0; i<7; i++){
		int count = 0;
		vector<unsigned int> table(sizes[i], 1);

		for(int j=0; j<branches.size(); j++){
			if(table[(branches[j].getAd()%sizes[i])]==1 && branches[j].getVal()=="T"){
				count++;
			}
			else if(table[(branches[j].getAd()%sizes[i])]==0 && branches[j].getVal()!="T"){
				count++;
			}
			else{
				if(table[(branches[j].getAd()%sizes[i])]==1){table[(branches[j].getAd()%sizes[i])]=0;}
				else{table[(branches[j].getAd()%sizes[i])]=1;};
			}
		}
		b1.push_back(count);
	}
}

void Predict::twoBitBimodal(){
	for(int i =0; i<7; i++){
		int count = 0;
		vector<unsigned int> table(sizes[i], 3);

		for(int j=0; j<branches.size(); j++){
			if(table[(branches[j].getAd()%sizes[i])]>1 && branches[j].getVal()=="T"){
				count++;
				if(table[(branches[j].getAd()%sizes[i])]<3){table[(branches[j].getAd()%sizes[i])]++;};
			}
			else if(table[(branches[j].getAd()%sizes[i])]<2 && branches[j].getVal()!="T"){
				count++;
				if(table[(branches[j].getAd()%sizes[i])]>0){table[(branches[j].getAd()%sizes[i])]--;};
			}
			else{
				if(table[(branches[j].getAd()%sizes[i])]>1){table[(branches[j].getAd()%sizes[i])]--;}
				else{table[(branches[j].getAd()%sizes[i])]++;};
			}
		}
		b2.push_back(count);
	}
}

void Predict::Gshare(){
	for(int i = 3; i<12; i++){
		vector<unsigned int> table(2048, 3);
		int count = 0;
		string ghr = "";
		for(int j=0; j<i; j++){
			ghr.append("0");
		}
		for(int j=0; j<branches.size(); j++){
			int index = (branches[j].getAd() ^ (stoi(ghr, nullptr, 2))) % 2048;

			//calculations
			if(table[index] > 1 && branches[j].getVal() == "T"){
				count++;
			} else if(table[index]<2 && branches[j].getVal() != "T") {
				count++;
			}

			//update prediction values
			if(branches[j].getVal()=="T"){
				if(table[index]!=3){table[index]++;}
			} else {
				if(table[index]!=0){table[index]--;}
			}

			//update ghr
			if(branches[j].getVal()=="T"){
				ghr.append("1");
				ghr = ghr.substr(1, ghr.size());
			} else {
				ghr.append("0");
				ghr = ghr.substr(1, ghr.size());
			}
		}
		g.push_back(count);
	}
}

void Predict::tournament(){
	int count = 0;

	//Configure Gshare
	string ghr = "00000000000";
	vector<unsigned int> gtable(2048, 3);

	//Configure Bimodal
	vector<unsigned int> btable(2048, 3);

	//Configure selector
	vector<unsigned int> tour(2048, 0); // 00= p Gshare, 1= wp Gshare, 2= wp Bimodal, 3= p Bimodal

	//Calculations
	for(int i=0; i<branches.size(); i++){
		bool gPredict, bPredict;
		//Indexes
		int index = (branches[i].getAd() ^ stoi(ghr, nullptr, 2)) % 2048;

		int Index = branches[i].getAd()%2048;

		//Bimodal
		if(btable[Index]>1 && branches[i].getVal()=="T"){
				bPredict = true;
				if(btable[Index]<3){btable[Index]++;};
		}
		else if(btable[Index]<2 && branches[i].getVal()!="T"){
			bPredict = true;;
			if(btable[Index]>0){btable[Index]--;};
		}
		else{
			bPredict = false;
			if(btable[Index]>1){btable[Index]--;}
			else{btable[Index]++;};
		}

		//Gshare
		if(gtable[index] > 1 && branches[i].getVal() == "T"){
			gPredict = true;
		} else if(gtable[index]<2 && branches[i].getVal() != "T") {
			gPredict = true;
		} else {
			gPredict = false;
		}

		//update prediction values
		if(branches[i].getVal()=="T"){
			if(gtable[index]!=3){gtable[index]++;}
		} else {
			if(gtable[index]!=0){gtable[index]--;}
		}

		//update ghr
		if(branches[i].getVal()=="T"){
			ghr.append("1");
			ghr = ghr.substr(1, ghr.size());
		} else {
			ghr.append("0");
			ghr = ghr.substr(1, ghr.size());
		}

		//tournament
		if(bPredict==true && gPredict==true){
			count++;
		} else if (bPredict){
			if(tour[Index]>1){count++;}
			if(tour[Index]<3){tour[Index]++;}
		} else if (gPredict){
			if(tour[Index]<2){count++;}
			if(tour[Index]>0){tour[Index]--;}
		} else {
			continue;
		}
	}
	t.push_back(count);
}

void Predict::popOutput(){
	ofstream output;
	output.open(this->output);

	output << at[0] << "," << this->b << ";\n";
	
	output << ant[0] << "," << this->b << ";\n";
	
	output << b1[0] << "," << this->b << "; " << b1[1] << "," << this->b << "; " <<
  b1[2] << "," << this->b << "; " << b1[3] << "," << this->b << "; " <<
  b1[4] << "," << this->b << "; " << b1[5] << "," << this->b << "; " <<
  b1[6] << "," << this->b << ";\n";
	
	output << b2[0] << "," << this->b << "; " << b2[1] << "," << this->b << "; " <<
  b2[2] << "," << this->b << "; " << b2[3] << "," << this->b << "; " <<
  b2[4] << "," << this->b << "; " << b2[5] << "," << this->b << "; " <<
  b2[6] << "," << this->b << ";\n";

	output << g[0] << "," << this->b << "; " << g[1] << "," << this->b << "; " <<
  g[2] << "," << this->b << "; " << g[3] << "," << this->b << "; " <<
  g[4] << "," << this->b << "; " << g[5] << "," << this->b << "; " <<
  g[6] << "," << this->b << "; " << g[7] << "," << this->b << "; " << 
  g[8] << "," << this->b << ";\n";

	output << t[0] << "," << this->b << ";\n";

	output.close();
}
