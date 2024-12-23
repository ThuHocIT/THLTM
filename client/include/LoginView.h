#ifndef CLIENT_VIEW_H
#define CLIENT_VIEW_H

#include "../../model/User.h"

// Nhập thông tin đăng ký
void getRegisterInfo(User* user);

// Nhập thông tin đăng nhập
void getLoginInfo(char* name, char* password);

#endif // CLIENT_VIEW_H
