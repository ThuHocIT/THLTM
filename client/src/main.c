#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../model/User.h"  // Sử dụng User.h từ thư mục common
#include "../include/LoginView.h"
#include "../include/MenuView.h" 
#include "../include/BookingView.h"
#include "../include/SearchView.h"
#include "../include/CompareView.h"

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Tạo socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    // Thiết lập thông tin địa chỉ server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Chuyển địa chỉ IP thành dạng nhị phân
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    // Kết nối đến server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    int choice;
    User user;
    char name[50], password[50];

    while (1) {
        // Menu chọn hành động
        printf("\n--- Menu ---\n1. Register\n2. Login\n0. Exit\nYour choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input, please enter a number.\n");
            while (getchar() != '\n'); // Xóa toàn bộ buffer nhập liệu
            continue;
        }
        getchar();

        if (choice == 1) {
            // Nhập thông tin đăng ký
            getRegisterInfo(&user);
            snprintf(buffer, BUFFER_SIZE, "register %s %s %s", 
                     user.name, user.email, user.password);
            send(sock, buffer, strlen(buffer), 0);  // Gửi thông tin đăng ký đến server
        } else if (choice == 2) {
            // Nhập thông tin đăng nhập
            getLoginInfo(name, password);
            snprintf(buffer, BUFFER_SIZE, "login %s %s", name, password);
            send(sock, buffer, strlen(buffer), 0);  // Gửi thông tin đăng nhập đến server
        } else if (choice == 0) {
            break;  // Thoát khỏi vòng lặp
        }else {
            printf("Invalid choice, please select again.\n");
            continue;
        }

        // Đọc phản hồi từ server
        memset(buffer, 0, BUFFER_SIZE);  // Xóa buffer trước khi nhận phản hồi
        int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);  // Nhận dữ liệu
        if (bytes_received > 0) {
        buffer[bytes_received] = '\0';  // Đảm bảo kết thúc chuỗi

        }else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            printf("Error receiving data from server.\n");
        }
    printf("Response: %s\n", buffer);
    //Sau khi Login success
    if (strncmp(buffer, "LOGIN_SUCCESS", 13) == 0) {
                char username[50];
                sscanf(buffer, "LOGIN_SUCCESS %s", username); // Lấy username từ phản hồi
                printf("Login successful. Welcome, %s!\n", username);

                while (1) {
                displayMenu();
                int menuChoice = getUserChoice();

                if (menuChoice == 5) { // Thoát khỏi menu
                    printf("Logging out. Goodbye!\n");
                    break;
                }

                handleMenuSelection(menuChoice, sock); // Gọi hàm xử lý menu
            }
        
        }  
    }
    // Đóng kết nối
    close(sock);
    return 0;

}