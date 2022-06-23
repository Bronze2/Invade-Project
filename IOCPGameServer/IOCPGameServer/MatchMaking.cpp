#include "pch.h"
#include "MatchMaking.h"

CMatchMaking::CMatchMaking() {};
CMatchMaking::~CMatchMaking() {};

void CMatchMaking::SetMatching(MATCH_TYPE matchType, int user_id)
{
	switch (matchType)
	{
	case MATCH_TYPE::TWO:
		if (Match2by2.size() == 0) {
			Match2by2[currentMatchIndex++].push_back(user_id);

			//user_id 에게 방장을 넘김
		}
		else {
			for (auto match : Match2by2) {
				if (match.second.size() < Match2by2Full)
				{
					match.second.push_back(user_id);
					break;
				}
				else {
					Match2by2[currentMatchIndex++].push_back(user_id);
					break;
				}
			}
		}

		break;
	case MATCH_TYPE::THREE:
		break;
	case MATCH_TYPE::FOUR:
		break;
	}
}