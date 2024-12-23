#include <string.h>
#include "../../model/User.h"
#include "../include/UserController.h"

int registerUser(const char* filename, const User* user) {
    User foundUser;
    if (findUser(filename, user->name, &foundUser) == 0) {
        // Username đã tồn tại
        return -1;
    }
    return saveUser(filename, user);
}

int loginUser(const char* filename, const char* name, const char* password) {
    User foundUser;
    if (findUser(filename, name, &foundUser) == 0) {
        if (strcmp(foundUser.password, password) == 0) {
            return 0; // Đăng nhập thành công
        }
    }
    return -1; // Đăng nhập thất bại
}
