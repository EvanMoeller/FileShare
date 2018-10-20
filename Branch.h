#ifndef BRANCH_H
#define BRANCH_H

#include <string>

class Branch{
private:
	unsigned int ad;
	std::string val;
public:
	Branch(unsigned int a, std::string v){
		this->ad = a;
		this->val = v;
	}
	unsigned int getAd(){return ad;};
	std::string getVal(){return val;};
};
#endif
