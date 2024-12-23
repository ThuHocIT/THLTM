#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../include/BookingView.h"

#define BUFFER_SIZE 4096

void bookFlight(int sock) {
    char flightCode[50];
    char buffer[BUFFER_SIZE];
    int ticketCount;


    while (getchar() != '\n');
    // Nhập mã chuyến bay từ người dùng
    printf("Enter flight code: ");
    fgets(flightCode, sizeof(flightCode), stdin);
    flightCode[strcspn(flightCode, "\n")] = '\0'; // Loại bỏ ký tự xuống dòng

    // Nhập số lượng vé
    printf("Enter number of tickets you want to buy: ");
    while (scanf("%d", &ticketCount) != 1 || ticketCount <= 0) {
        printf("Invalid input. Please enter a positive number for tickets: ");
        while (getchar() != '\n');  // Xóa toàn bộ buffer nhập liệu
    }

    // Gửi yêu cầu đặt vé đến server
    snprintf(buffer, BUFFER_SIZE, "book %s %d ", flightCode, ticketCount);
    send(sock, buffer, strlen(buffer), 0);

    // Nhận phản hồi từ server
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        displayBookingResponse(buffer);
    } else if (bytes_received == 0) {
        printf("Server closed the connection.\n");
    } else {
        printf("Error receiving data from server.\n");
    }
}

void displayBookingResponse(const char* response) {
    printf("Booking response: %s\n", response);
}
