#include "pch.h"
#include "MatchMaking.h"

CMatchMaking::CMatchMaking() {};
CMatchMaking::~CMatchMaking() {};




void CMatchMaking::enterRoom(int room_id , int client_id)
{

	Match2by2[room_id].push_back(client_id);
	for (auto room : Match2by2) {
		cout << "--------Room Index[" << room.first << "]----------" << endl;
		for (int i = 0; i < room.second.size(); ++i) {
			cout << "유저 아이디" << room.second[i] << endl;
		}
		cout << "-------------------------------------------" << endl;
	}
	if (LobbyBlue[room_id].size() <= LobbyRed[room_id].size()) {
		LobbyBlue[room_id].push_back(client_id);
		SHARED_DATA::g_clients[client_id].m_camp = CAMP_STATE::BLUE;
	}
	else {
		LobbyRed[room_id].push_back(client_id);
		SHARED_DATA::g_clients[client_id].m_camp = CAMP_STATE::RED;

	}

	for (auto blue : LobbyBlue) {
		for (int i = 0; i < blue.second.size(); ++i) {
			cout << "블루팀 - " << blue.second[i] << endl;
		}
	}

	for (auto red : LobbyRed) {
		for (int i = 0; i < red.second.size(); ++i) {
			cout << "레드팀 - " << red.second[i] << endl;
		}
	}
}



void CMatchMaking::makeRoom(int room_id, MATCH_TYPE matchType ,char _RoomName[100])
{
	if (matchType == MATCH_TYPE::TWO) {
		string temp(_RoomName);
		Match2by2[room_id].push_back(room_id);
		RoomName[room_id] = temp;
		cout << RoomName[room_id] << endl;
	}

	for (auto room : Match2by2) {
		cout<<"--------Room Index[" << room.first<<"]----------"<< endl;
		for (int i = 0; i < room.second.size(); ++i) {
			cout << "유저 아이디" << room.second[i] <<endl;
			LobbyBlue[room_id].push_back(room.second[i]);
		}
		cout << "-------------------------------------------" <<endl;
	}

	// 확인 
	for (auto blue : LobbyBlue) {
		for (int i = 0; i < blue.second.size(); ++i) {
			cout << "블루팀 - " << blue.second[i] << endl;
		}
	}

	for (auto red : LobbyRed) {
		for (int i = 0; i < red.second.size(); ++i) {
			cout << "레드팀 - " << red.second[i] << endl;
		}
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