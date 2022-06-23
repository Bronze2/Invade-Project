#pragma once
#include "pch.h"

constexpr int Match2by2Full = 4;
constexpr int Match3by3Full = 6;
constexpr int Match4by4Full = 8;

class CMatchMaking
{
	SINGLE(CMatchMaking)
private:
	int currentMatchIndex = 0;
	map<int, vector<int>> Match2by2;
	map<int, vector<int>> Match3by3;
	map<int, vector<int>> Match4by4;
public:
	void SetMatching(MATCH_TYPE matchType, int user_id);
};



