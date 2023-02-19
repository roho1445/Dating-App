#ifndef PERSON_PROFILE
#define PERSON_PROFILE

#include <string>
#include <vector>
#include <unordered_set>
#include "provided.h"
#include "RadixTree.h"

class PersonProfile
{
public:
	PersonProfile(std::string name, std::string email);
	~PersonProfile();
	std::string GetName() const;
	std::string GetEmail() const;
	void AddAttValPair(const AttValPair& attval);
	int GetNumAttValPairs() const;
	bool GetAttVal(int attribute_num, AttValPair& attval) const;
private:
	std::string pName;
	std::string pEmail;
	RadixTree<std::vector<std::string>> attributes;
	//Struct to store AttValPair as a pointer
	struct AttValPtr {
		std::string* keyptr;
		std::vector<std::string>* values;
		int index;
	};
	std::vector<AttValPtr> attvalptr_vector;
	RadixTree<std::string> keyTree;
	//Method to find a string in a vector and return a pointer to it
	std::string* find(std::vector<std::string> *vec, const std::string& val);
};

#endif // !PERSON_PROFILE