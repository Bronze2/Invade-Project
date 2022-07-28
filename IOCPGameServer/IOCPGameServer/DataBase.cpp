//#include "pch.h"
//#include "DataBase.h"
//
//CDataBase::CDataBase()
//{
//
//}
//
//CDataBase::~CDataBase()
//{
//    SQLCancel(hstmt);
//    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
//    SQLDisconnect(hdbc);
//    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
//    SQLFreeHandle(SQL_HANDLE_ENV, henv);
//}
//
//
//
//void CDataBase::Init() 
//{
//
//    setlocale(LC_ALL, "korean");
//
//    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
//    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
//    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);	//명령어 저장할 핸들
//    SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
//    retcode = SQLConnect(hdbc, (SQLWCHAR*)L"game_db_odbc", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
//    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
//    {
//        std::cout << "DB connect!\n";
//    }
//    else
//    {
//        std::cout << "DB connect fail!\n";
//    }
//
//    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)L"SELECT login_id, login_pw FROM login_data ORDER BY 1", SQL_NTS);
//    std::cout << "----------------  Show Current Admin  -----------------"<< endl;
//    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//        retcode = SQLBindCol(hstmt, 1, SQL_C_WCHAR, szUser_LoginID, NAME_LEN, &cbLoginID);
//        retcode = SQLBindCol(hstmt, 2, SQL_C_WCHAR, szUser_LoginPW, NAME_LEN, &cbLoginPW);
//        for (int i = 0; ; i++) {
//            retcode = SQLFetch(hstmt);
//            if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
//                //   show_error();
//            }
//            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
//            {
//
//                printf("%d: %S %S\n", i + 1, szUser_LoginID, szUser_LoginPW);
//
//            }
//            else
//                break;
//        }
//    }
//    std::cout << "-------------------------------------------------------" << endl;
//}
//
//void CDataBase::CreateAdmin(wstring admin_id, wstring admin_pw)
//{
//    wstring order = L"EXECUTE insert_admin ";
//    order += admin_id;
//    order += L", ";
//    order += admin_pw;
//
//    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)order.c_str(), SQL_NTS);
//    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//        std::cout << "Create Admin\n";
//
//    }
//    else {
//        std::cout << "DB Order Error\n";
//    }
//}
//
//bool CDataBase::DoubleCheckAdmin(wstring admin_id)
//{
//    wstring order = L"EXECUTE double_check_admin ";
//    order += admin_id;
//
//    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)order.c_str(), SQL_NTS);
//    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//        retcode = SQLBindCol(hstmt, 1, SQL_C_WCHAR, szUser_LoginID, NAME_LEN, &cbLoginID);
//       
//        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//            retcode = SQLFetch(hstmt);
//            if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
//                HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
//            }
//            //검색 완료( 존재 )
//            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
//            {
//                printf("%S 가 이미 존재합니다. 다른 아이디를 사용해 주세요.\n", szUser_LoginID);
//                return false;
//            }
//            // 검색 실패 ( 없음 )
//            else {
//                printf("%S 는 사용 가능한 아이디 입니다.\n", admin_id);
//                return true;
//            }
//        }
//    }
//    else {
//        std::cout << "DB Order Error\n";
//        return false;
//
//    }
//}
//
//bool CDataBase::CheckAdminLogin(wstring admin_id, wstring admin_pw)
//{
//    wstring order = L"EXECUTE check_admin_login ";
//    order += admin_id;
//    order += L", ";
//    order += admin_pw;
//
//    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)order.c_str(), SQL_NTS);
//    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//        retcode = SQLBindCol(hstmt, 1, SQL_C_WCHAR, szUser_LoginID, NAME_LEN, &cbLoginID);
//        retcode = SQLBindCol(hstmt, 2, SQL_C_WCHAR, szUser_LoginPW, NAME_LEN, &cbLoginPW);
//
//
//        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//            retcode = SQLFetch(hstmt);
//            if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
//                HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
//            }
//            //검색 완료( 존재 )
//            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
//            {
//                printf("ID: %S  ", szUser_LoginID);
//                printf("PW: %S  ", szUser_LoginPW);
//                printf("로그인 성공.\n");
//
//                return true;
//            }
//            // 검색 실패 ( 없음 )
//            else {
//                printf("%S 로그인 정보가 잘못 되었습니다.\n", admin_id);
//                return false;
//            }
//        }
//    }
//    else {
//        std::cout << "DB Order Error\n";
//        return false;
//
//    }
//}
//
///* HandleDiagnosticRecord : display error/warning information
///*
///* Parameters:
///* hHandle ODBC handle
///* hType Type of handle (SQL_HANDLE_STMT, SQL_HANDLE_ENV, SQL_HANDLE_DBC)
///* RetCode Return code of failing command
///************************************************************************/
//void CDataBase::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
//{
//    SQLSMALLINT iRec = 0;
//    SQLINTEGER iError;
//    WCHAR wszMessage[1000];
//    WCHAR wszState[SQL_SQLSTATE_SIZE + 1];
//    if (RetCode == SQL_INVALID_HANDLE) {
//        fwprintf(stderr, L"Invalid handle!\n");
//        return;
//    }
//    while (SQLGetDiagRec(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
//        (SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
//        // Hide data truncated..
//        if (wcsncmp(wszState, L"01004", 5)) {
//            fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
//        }
//    }
//}
//
////setlocale(LC_ALL, "korean");
//   //// Allocate environment handle  
//   //retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
//
//   //// Set the ODBC version environment attribute  
//   //if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//   //    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
//
//   //    // Allocate connection handle  
//   //    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//   //        retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
//
//   //        // Set login timeout to 5 seconds  
//   //        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//   //            SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
//
//   //            // Connect to data source  
//   //            retcode = SQLConnect(hdbc, (SQLCHAR*)"game_db_odbc", SQL_NTS, (SQLCHAR*)NULL, 0, NULL, 0);
//
//   //            // Allocate statement handle  
//   //            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//   //                retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//   //                std::cout << "DB Connect " << std::endl;
//   //                retcode = SQLExecDirect(hstmt, (SQLCHAR*)"SELECT login_id, login_pw FROM login_data ORDER BY 1", SQL_NTS);
//
//   //                if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//
//   //                    // Bind columns 1, 2, and 3  
//   //                    //retcode = SQLBindCol(hstmt, 1, SQL_C_LONG, &dUser_id, 100, &cbID);
//   //                    //retcode = SQLBindCol(hstmt, 2, SQL_C_WCHAR, szUser_name, NAME_LEN, &cbName);    
//   //                    //retcode = SQLBindCol(hstmt, 3, SQL_C_LONG, &dUser_level, 100, &cblevel);
//   //                    
//   //                    
//   //                    retcode = SQLBindCol(hstmt, 1, SQL_C_WCHAR, szUser_LoginID, NAME_LEN, &cbLoginID);
//   //                    retcode = SQLBindCol(hstmt, 2, SQL_C_WCHAR, szUser_LoginPW, NAME_LEN, &cbLoginPW);
//
//   //                    // Fetch and print each row of data. On an error, display a message and exit.  
//   //                    for (int i = 0; ; i++) {
//
//   //                        retcode = SQLFetch(hstmt);
//   //                        if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
//   //                         //   show_error();
//
//   //                        }
//   //                        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
//   //                        {
//   //                            //replace wprintf with printf
//   //                            //%S with %ls
//   //                            //warning C4477: 'wprintf' : format string '%S' requires an argument of type 'char *'
//   //                            //but variadic argument 2 has type 'SQLWCHAR *'
//   //                            //wprintf(L"%d: %S %S %S\n", i + 1, sCustID, szName, szPhone);  
//   //                            printf("%d: %S %S\n", i+1, szUser_LoginID, szUser_LoginPW);
//
//   //                        }
//   //                        else
//   //                            break;
//   //                    }
//   //                }
//
//   //                // Process data  
//   //                if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//   //                    SQLCancel(hstmt);
//   //                    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
//   //                }
//
//   //                SQLDisconnect(hdbc);
//   //            }
//
//   //            SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
//   //        }
//   //    }
//   //    SQLFreeHandle(SQL_HANDLE_ENV, henv);
//   //}