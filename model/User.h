#ifndef USER_H
#define USER_H

#define BUFFERSIZE 1024
typedef struct {
    char name[50];
    char email[50];
    char password[50];
    int money;
} User;
int saveUser(const char* filename, const User* user);
int findUser(const char* filename, const char* name, User* foundUser);
int updateUserBalance(const char* filename, const User* user);
int getUser(const char* filename, const char* name, User* user);
#endif // USER_H
