#include <stdio.h>
#include <string.h>
#include "../../model/User.h"

int saveUser(const char* filename, const User* user) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        return -1;
    }
    fprintf(file, "%s %s %s %d\n", user->name, user->email, user->password, 10000);
    fclose(file);
    return 0;
}

int findUser(const char* filename, const char* name, User* foundUser) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    User temp;
    while (fscanf(file, "%s %s %s %d", 
                  temp.name, temp.email, temp.password, &temp.money) == 4) {
        if (strcmp(temp.name, name) == 0) {
            *foundUser = temp;
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return -1;
}

// Hàm cập nhật số dư tài khoản người dùng
int updateUserBalance(const char* filename, const User* user) {
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        return -1; // Lỗi mở file
    }

    char line[BUFFERSIZE];
    long pos;
    int found = 0;

    // Đọc file và tìm người dùng
    while (fgets(line, sizeof(line), file) != NULL) {
        User temp_user;
        sscanf(line, "%s %s %s %d", temp_user.name, temp_user.email, temp_user.password, &temp_user.money);

        // Kiểm tra nếu là người dùng cần cập nhật
        if (strcmp(temp_user.name, user->name) == 0) {
            found = 1;
            pos = ftell(file); // Lưu vị trí dòng cần sửa
            break;
        }
    }

    if (!found) {
        fclose(file);
        return -2; // Không tìm thấy người dùng
    }

    // Cập nhật lại số dư tài khoản người dùng
    fseek(file, pos - strlen(line), SEEK_SET); // Quay lại vị trí dòng cũ
    fprintf(file, "%s %s %s %d\n", user->name, user->email, user->password, user->money);

    fclose(file);
    return 0; // Cập nhật thành công
}

int getUser(const char* filename, const char* name, User* user) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // Lỗi mở file
    }

    char line[BUFFERSIZE];
    int found = 0;

    // Đọc từng dòng và tìm người dùng theo tên
    while (fgets(line, sizeof(line), file) != NULL) {
        // Dùng sscanf để tách thông tin người dùng từ dòng
        if (sscanf(line, "%s %s %s %d", 
                   user->name, 
                   user->email, 
                   user->password, 
                   &user->money) == 4) {
            if (strcmp(user->name, name) == 0) {
                found = 1;
                break; // Tìm thấy người dùng, thoát khỏi vòng lặp
            }
        }
    }

    fclose(file);

    return found ? 0 : -2; // Trả về 0 nếu tìm thấy, -2 nếu không tìm thấy
}