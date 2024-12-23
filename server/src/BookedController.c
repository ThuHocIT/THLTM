#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../include/BookedController.h"
#include "../../model/Booked.h"
#include "../../model/Flight.h"
#include "../../model/User.h"


#define BUFFER_SIZE 4096

// Hàm xử lý xem danh sách vé đã đặt
void handleSeeBookedFlights(int socket_fd, const char* bookedFile, const char* username) {
    FILE* file = fopen(bookedFile, "r");
    if (file == NULL) {
        perror("Could not open booked file");
        char response[] = "Error: Could not retrieve booked flights.";
        send(socket_fd, response, strlen(response), 0);
        return;
    }

    char line[BUFFER_SIZE];
    char response[BUFFER_SIZE * 5]; // Đủ lớn để chứa danh sách chuyến bay đã đặt
    snprintf(response, sizeof(response), "Danh sách chuyến bay đã đặt:\n");
    int found = 0;

    // Đọc từng dòng trong file BOOKED
    while (fgets(line, sizeof(line), file) != NULL) {
        char fileUsername[50], email[100], flightCode[20], departure[50], destination[50], bookingDate[20];
        int numberOfSeats;
        float totalPrice;

        // Tách thông tin từ dòng
        if (sscanf(line, "%s %s %s %s %s %s %d %f", 
                   fileUsername, email, flightCode, departure, destination, bookingDate, &numberOfSeats, &totalPrice) == 8) {
            // Kiểm tra nếu tên người dùng trùng với `username`
            if (strcmp(fileUsername, username) == 0) {
                found = 1;
                char bookedFlight[BUFFER_SIZE];
                snprintf(bookedFlight, sizeof(bookedFlight),
                         "- Flight: %s | From: %s | To: %s | Date: %s | Seats: %d | Total: %.2f\n",
                         flightCode, departure, destination, bookingDate, numberOfSeats, totalPrice);
                strncat(response, bookedFlight, sizeof(response) - strlen(response) - 1);
            }
        }
    }

    fclose(file);

    // Nếu không tìm thấy chuyến bay nào
    if (!found) {
        snprintf(response, sizeof(response), "No booked flights found for user: %s\n", username);
    }

    // Gửi danh sách chuyến bay hoặc thông báo không tìm thấy về client
    send(socket_fd, response, strlen(response), 0);
}

void handleCancelBooked(int client_socket, const char* bookedFile, const char* flightFile, const char* userFile, const char* username, const char* flightCode, int cancelCount) {
    char response[BUFFER_SIZE];

    // Tìm thông tin đặt vé
    Booked bookedInfo;
    if (findBookedFlight(bookedFile, username, flightCode, &bookedInfo) != 0) {
        snprintf(response, sizeof(response), "Error: No matching booking found for user %s and flight %s.", username, flightCode);
        send(client_socket, response, strlen(response), 0);
        return;
    }

    // Kiểm tra số lượng ghế cần hủy
    if (cancelCount > bookedInfo.numberOfSeats) {
        snprintf(response, sizeof(response), "Error: Cannot cancel %d seats. Only %d seats booked.", cancelCount, bookedInfo.numberOfSeats);
        send(client_socket, response, strlen(response), 0);
        return;
    }

    // Tính toán tiền hoàn lại
    float refundAmount = (bookedInfo.totalPrice / bookedInfo.numberOfSeats) * cancelCount;
    
    // Giảm số lượng ghế trong file BOOKED
    FILE* booked = fopen(bookedFile, "r+");
    if (!booked) {
        snprintf(response, sizeof(response), "Error: Could not access booking data.");
        send(client_socket, response, strlen(response), 0);
        return;
    }

    char line[BUFFER_SIZE];
    long position;
    while (fgets(line, sizeof(line), booked)) {
        position = ftell(booked) - strlen(line); // Lưu vị trí đầu dòng
        Booked tempBooked;

        if (sscanf(line, "%s %s %s %s %s %s %d %f", 
                   tempBooked.user.name,tempBooked.user.email, tempBooked.flight.flightCode,
                   tempBooked.flight.departure, tempBooked.flight.destination,
                   tempBooked.bookingDate, &tempBooked.numberOfSeats, &tempBooked.totalPrice) == 8) {
            if (strcmp(tempBooked.user.name, username) == 0 && strcmp(tempBooked.flight.flightCode, flightCode) == 0) {
                tempBooked.numberOfSeats -= cancelCount;
                tempBooked.totalPrice -= refundAmount;

                fseek(booked, position, SEEK_SET); // Quay lại vị trí đầu dòng
                if (tempBooked.numberOfSeats > 0) {
                    fprintf(booked, "%s %s %s %s %s %s %d %.2f\n",
                            tempBooked.user.name, tempBooked.user.email, tempBooked.flight.flightCode,
                            tempBooked.flight.departure, tempBooked.flight.destination,
                            tempBooked.bookingDate, tempBooked.numberOfSeats, tempBooked.totalPrice);
                } else {
                    fprintf(booked, "%-*s\n", (int)strlen(line) - 1, ""); // Xóa dòng nếu số ghế là 0
                }
                break;
            }
        }
    }
    fclose(booked);

    // Cập nhật số ghế trong FLIGHT
    Flight flight;
    if (getFlightByCode(flightFile, flightCode, &flight) == 0) {
        flight.remainingSeats += cancelCount;
        updateFlightSeats(flightFile, &flight);
    }

    // Cập nhật số dư trong USER
    User user;
    if (getUser(userFile, username, &user) == 0) {
        user.money += refundAmount;
        updateUserBalance(userFile, &user);
    }

    // Gửi phản hồi thành công
    snprintf(response, sizeof(response), "Cancellation successful. Refund amount: %.2f", refundAmount);
    send(client_socket, response, strlen(response), 0);
}