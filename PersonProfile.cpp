#include "PersonProfile.h"
using namespace std;

PersonProfile::PersonProfile(string name, string email) : pName(name), pEmail(email) {}
PersonProfile::~PersonProfile() {}

string PersonProfile::GetName() const
{
	return pName;
}

string PersonProfile::GetEmail() const
{
	return pEmail;
}

void PersonProfile::AddAttValPair(const AttValPair& attval)
{
	std::string* valueptr = nullptr;

	std::vector<std::string>* values = attributes.search(attval.attribute);
	if (values == nullptr)
	{
		std::vector<std::string> val;
		val.push_back(attval.value);
		attributes.insert(attval.attribute, val);
		values = attributes.search(attval.attribute);
	}
	else
	{
		valueptr = find(values, attval.value);
		if (valueptr != nullptr) {  
			return;
		}
		values->push_back(attval.value);
	}

	std::string* keyptr = keyTree.search(attval.attribute);
	if (keyptr == nullptr) {
		keyTree.insert(attval.attribute, attval.attribute);
		keyptr = keyTree.search(attval.attribute);
	}
	AttValPtr avp({ keyptr, values, static_cast<int>(values->size())-1});
	attvalptr_vector.push_back(avp);
}

int PersonProfile::GetNumAttValPairs() const
{
	return static_cast<int>(attvalptr_vector.size());
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const
{
	if (attribute_num >= GetNumAttValPairs() || attribute_num < 0)
		return false;

	attval.attribute = *attvalptr_vector[attribute_num].keyptr;
	std::vector<std::string> values = *attvalptr_vector[attribute_num].values;
	int index = attvalptr_vector[attribute_num].index;
	attval.value = values[index];
	return true;
}

std::string* PersonProfile::find(std::vector<std::string>* vec, const std::string& str) {
	if (vec != nullptr) {
		for (int i = 0; i < vec->size(); i++) {
			if ((*vec)[i].compare(str) == 0) {
				return &(*vec)[i];
			}
		}
	}
	return nullptr;
}