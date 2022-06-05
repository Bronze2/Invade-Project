#pragma once

struct SUserInformation {

public:
	char m_cUserName[255] = "Enter Your ID";
	char m_cUserPassWord[255] = "Enter Your Password";
	int button_opacity = 255;
	bool bLogin = false;

}UserInformations;



struct SInitInformation {

public:
	char m_cUserName[255] = "Enter Your ID";
	char m_cUserPassWord[255] = "Enter Your Password";


}InitInformations;