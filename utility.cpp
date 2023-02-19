#include "utility.h"
#include <string>
using namespace std;

string attValPairtoString(const AttValPair& source)
{
	string concantenate = "";
	concantenate += source.attribute;
	concantenate += source.value;
	return concantenate;
}