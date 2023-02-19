#ifndef ATTRIBUTE_TRANSLATOR
#define ATTRIBUTE_TRANSLATOR

#include <vector>
#include <string>
#include "provided.h"
#include "RadixTree.h"


class AttributeTranslator
{
public:
	AttributeTranslator();
	~AttributeTranslator();
	bool Load(std::string filename);
	std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
	RadixTree<std::vector<AttValPair*>> compAttributes;
	//Check if vector contains attValPair
	bool contains(std::vector<AttValPair*>* vec, const AttValPair& attval);
	RadixTree<AttValPair> valueAttrs;  
};

#endif // !ATTRIBUTE_TRANSLATOR
