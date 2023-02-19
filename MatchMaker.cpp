#include "MatchMaker.h"
#include "utility.h"

#include <iostream>
using namespace std;


MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at)
	: members(mdb), attTran(at)
{
}

MatchMaker::~MatchMaker()
{
}

//Sorts vector using insertionSort logic
void insertionSort(vector<EmailCount*>& orderMe)
{
	for (int s = 2; s <= orderMe.size(); s++)
	{
		EmailCount* sortMe = orderMe[s - 1];

		int i = s - 2;
		while ((i >= 0) &&
			   ((sortMe->count > orderMe[i]->count) ||
				(sortMe->count == orderMe[i]->count && sortMe->email < orderMe[i]->email))) {
				orderMe[i + 1] = orderMe[i];
				--i;
		}
		orderMe[i + 1] = sortMe;
	}
}


vector<EmailCount> MatchMaker::IdentifyRankedMatches(string email, int threshold) const
{
	//Get PersonProfile in question
	RadixTree<std::string> compTree;
	std::vector<AttValPair> compList;

	const PersonProfile* currPerson = members.GetMemberByEmail(email);

	// Translate to unique attribute-value paris for the given person
	for (int i = 0; i < currPerson->GetNumAttValPairs(); i++)
	{
		AttValPair currAttValPair;
		currPerson->GetAttVal(i, currAttValPair);
		vector<AttValPair> currCompAttributes = attTran.FindCompatibleAttValPairs(currAttValPair);
		for (int i = 0; i < currCompAttributes.size(); i++) {
			std::string attvalstr = attValPairtoString(currCompAttributes[i]);
			if (!compTree.search(attvalstr)) {  // select unique key-value pairs only
				compTree.insert(attvalstr, "");
				compList.push_back(currCompAttributes[i]);
			}
		}
	}

	// Get the email addresses and match count for the translated attributes
	RadixTree<EmailCount> compEmailTree;
	std::vector<EmailCount*> compEmailList;

	for (int i = 0; i < compList.size(); i++)
	{
		vector<string> compPeople = members.FindMatchingMembers(compList[i]);
		for (int x = 0; x < compPeople.size(); x++)
		{
			if (email.compare(compPeople[x]) != 0) {
				EmailCount* emailcount = compEmailTree.search(compPeople[x]);
				if (emailcount == nullptr) {  // if email not present add it with count=1
					EmailCount insertEmail(compPeople[x], 1);
					compEmailTree.insert(compPeople[x], insertEmail);
					emailcount = compEmailTree.search(compPeople[x]);
					compEmailList.push_back(emailcount);
				}
				else {  // email present.  increment the number of matched attributes
					emailcount->count++;
				}
			}
		}
	}

	insertionSort(compEmailList);

	std::vector<EmailCount> result;
	for (int i = 0; i < compEmailList.size(); i++) {
		if (compEmailList[i]->count >= threshold) {
			result.push_back(*compEmailList[i]);
		}
	}
	return result;
}
