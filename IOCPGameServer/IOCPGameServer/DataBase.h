#pragma once
#include <sqlext.h>  
#define NAME_LEN 20  
#define PHONE_LEN 60


struct LoginData
{
	int id;
	string login_id;
	int login_password;
};

class CDataBase
{
	SINGLE(CDataBase)
public:
	void Init();
    
    //계정 생성
    void CreateAdmin(wstring admin_id, wstring admin_pw);
    //계정 생성 전 중복 확인
    bool DoubleCheckAdmin(wstring admin_id);
    
    //계정 로그인
    bool CheckAdminLogin(wstring admin_id, wstring admin_pw);
 
    void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);

private:
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLHSTMT hstmt = 0;
    SQLRETURN retcode;
    SQLWCHAR szUser_name[NAME_LEN];
    SQLINTEGER dUser_id, dUser_level;
    SQLLEN cbName = 0, cbID = 0, cblevel = 0;
    //LoginData
    SQLWCHAR szUser_LoginID[NAME_LEN];
    SQLWCHAR szUser_LoginPW[NAME_LEN];
    SQLLEN cbLoginID = 0, cbLoginPW = 0;

	
};

