#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "../../model/User.h"

// Đăng ký người dùng mới
int registerUser(const char* filename, const User* user);

// Đăng nhập người dùng
int loginUser(const char* filename, const char* name, const char* password);

#endif // USER_CONTROLLER_H