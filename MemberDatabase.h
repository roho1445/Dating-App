#ifndef MEMBER_DATABASE
#define MEMBER_DATABASE

#include <vector>
#include <string>
#include "provided.h"
#include "RadixTree.h"
#include "PersonProfile.h"

class MemberDatabase
{
public:
	MemberDatabase();
	~MemberDatabase();
	bool LoadDatabase(std::string filename);
	std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
	const PersonProfile* GetMemberByEmail(std::string email) const;
private:
	//Maps email to PersonProfile object
	RadixTree <PersonProfile> emailTree;

	//Maps AttValPair to vector of emails
	RadixTree <std::vector<std::string>> attValTree;
};

#endif // !MEMBER_DATABASE

