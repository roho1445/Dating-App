#ifndef MATCH_MAKER
#define MATCH_MAKER

#include <string>
#include <vector>
#include "provided.h"
#include "MemberDatabase.h"
#include "AttributeTranslator.h"

class MatchMaker
{
public:
	MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
	~MatchMaker();
	std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
private:
	//Store MemberDatabase and AttributeTranslator without duplicates
	const MemberDatabase& members;
	const AttributeTranslator& attTran;
};


#endif // !MATCH_MAKER


