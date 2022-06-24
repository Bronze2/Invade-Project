#include "pch.h"
#include "MatchMaking.h"

CMatchMaking::CMatchMaking() {};
CMatchMaking::~CMatchMaking() {};




void CMatchMaking::enterRoom(int room_id , int clinet_id)
{

	Match2by2[room_id].push_back(clinet_id);
	for (auto room : Match2by2) {
		cout << "--------Room Index[" << room.first << "]----------" << endl;
		for (int i = 0; i < room.second.size(); ++i) {
			cout << "유저 아이디" << room.second[i] << endl;
		}
		cout << "-------------------------------------------" << endl;
	}
}



void CMatchMaking::makeRoom(int room_id, MATCH_TYPE matchType)
{
	if (matchType == MATCH_TYPE::TWO) {
		Match2by2[room_id].push_back(room_id);
	}

	for (auto room : Match2by2) {
		cout<<"--------Room Index[" << room.first<<"]----------"<< endl;
		for (int i = 0; i < room.second.size(); ++i) {
			cout << "유저 아이디" << room.second[i] <<endl;
		}
		cout << "-------------------------------------------" <<endl;
	}
}

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