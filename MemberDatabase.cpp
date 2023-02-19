#include "MemberDatabase.h"
#include "utility.h"
#include <iostream>
#include <fstream>
using namespace std;

MemberDatabase::MemberDatabase() {}

MemberDatabase::~MemberDatabase() {}

bool MemberDatabase::LoadDatabase(std::string filename)
{
	string myText;
	ifstream MyReadFile(filename);
	bool nameAdded = false;
	bool emailAdded = false;
	string name;
	string email;
	PersonProfile* add = nullptr;
	int numRecords = 0;
	int maxatt = 0;
	int attcount = 0;
	while (getline(MyReadFile, myText))
	{
		if (myText.empty())
		{
			continue;
		}
		else if (!nameAdded)
		{
			name = myText;
			nameAdded = true;
		}
		else if (!emailAdded)
		{
			email = myText;

			if (emailTree.search(email) != nullptr)
				return false;

			emailAdded = true;
		}
		else if (isdigit(myText[0]))
		{
			maxatt = std::stoi(myText);
		}
		else if (!isdigit(myText[0]))
		{
			if (add == nullptr) {
				add = new PersonProfile(name, email);
			}
			size_t pos = myText.find(",");
			string attribute = myText.substr(0, pos);
			string value = myText.substr(pos + 1);
			AttValPair insert(attribute, value);
			add->AddAttValPair(insert);

			string attVal = attValPairtoString(insert);
			std::vector<std::string> *attEmails = attValTree.search(attVal);
			if (attEmails == nullptr)
			{
				vector<string> vecEm;
				vecEm.push_back(email);
				attValTree.insert(attVal, vecEm);
			}
			else
			{
				attEmails->push_back(email);
			}
			attcount++;

			if (attcount == maxatt) {
				if (add != nullptr) {
					emailTree.insert(email, (*add));
				}
				nameAdded = false;
				emailAdded = false;
				add = nullptr;
				attcount = 0;
				maxatt = 0;
				numRecords++;
				continue;
			}
		}
	}
	MyReadFile.close();
	return true;
}

vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
	vector<string> emails;

	vector<string>* val = attValTree.search(attValPairtoString(input));

	if (val != nullptr)
		emails = *val;

	return emails;
}


const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const
{
	return emailTree.search(email);
}