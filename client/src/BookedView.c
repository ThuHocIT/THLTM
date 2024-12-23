#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../include/BookedView.h"

#define BUFFER_SIZE 4096
// Hiển thị danh sách vé đã đặt (lấy từ server)
void displayBookedFlights(int socket_fd) {
    char buffer[BUFFER_SIZE * 5];
    memset(buffer, 0, sizeof(buffer));
    // Gửi yêu cầu tới server
    const char *request = "See Ticket";
    send(socket_fd, request, strlen(request), 0);
    printf("Sent request: [%s]\n", request);

    // Nhận dữ liệu từ server
    int bytes_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Đảm bảo buffer là chuỗi kết thúc bằng '\0'
        printf("%s\n", buffer);        // In danh sách chuyến bay đã đặt
    } else {
        perror("Failed to receive booked flights");
    }
}

void cancelBooked(int socket_fd) {
    char flightCode[20];
    int cancelCount;
    char buffer[BUFFER_SIZE];

    // Yêu cầu người dùng nhập mã chuyến bay và số lượng cần hủy
    printf("Enter the Flight code: ");
    scanf("%s", flightCode);

    printf("Enter the number of tickets: ");
    scanf("%d", &cancelCount);

    // Tạo yêu cầu gửi đến server
    snprintf(buffer, sizeof(buffer), "Cancel %s %d", flightCode, cancelCount);
    send(socket_fd, buffer, strlen(buffer), 0);
    printf("Sent request: [%s]\n", buffer);

    // Nhận phản hồi từ server
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer); // Hiển thị phản hồi từ server
    } else {
        perror("Failed to receive cancellation response");
    }
}


// Menu của BookingView
void bookedMenu(int socket_fd) {
    int choice;
    while (1) {
        printf("=====================\n");
        printf("1. See the booked flights\n");
        printf("2. Cancel the ticket\n");
        printf("3. Return Main Menu\n");
        printf("=====================\n");

        printf("Enter the option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayBookedFlights(socket_fd); // Hiển thị các chuyến đã đặt
                break;
            case 2: {
                cancelBooked(socket_fd);
                // cancelBooked(socket_fd, flightCode); // Hủy chuyến bay
                break;
            }
            case 3:
                return; // Quay lại Menu chính
            default:
                printf("Lựa chọn không hợp lệ. Vui lòng thử lại.\n");
        }
    }
}
