#include <stdio.h>
#include <string.h>
#include "../../model/User.h"  // Sử dụng User.h từ thư mục common
#include "../include/LoginView.h"

// Hàm nhập thông tin đăng ký từ người dùng
void getRegisterInfo(User* user) {
    printf("Enter Name: ");
    fgets(user->name, 50, stdin);
    user->name[strcspn(user->name, "\n")] = '\0';  // Loại bỏ ký tự newline nếu có
    printf("Enter Email: ");
    fgets(user->email, 50, stdin);
    user->email[strcspn(user->email, "\n")] = '\0';  // Loại bỏ ký tự newline nếu có
    printf("Enter Password: ");
    fgets(user->password, 50, stdin);
    user->password[strcspn(user->password, "\n")] = '\0';  // Loại bỏ ký tự newline nếu có
}

// Hàm nhập thông tin đăng nhập từ người dùng
void getLoginInfo(char* name, char* password) {
    printf("Enter Username: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';  // Loại bỏ ký tự newline nếu có
    printf("Enter Password: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = '\0';  // Loại bỏ ký tự newline nếu có
}
