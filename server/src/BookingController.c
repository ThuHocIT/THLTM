#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/BookingController.h"
#include "../../model/User.h"
#include "../../model/Flight.h"

#define USER_FILE "data/accounts.txt"
#define FLIGHT_FILE "data/flights.txt"

// Hàm tìm chuyến bay theo mã chuyến bay
int findFlight(const char* flightFile, const char* flightCode, Flight* flight) {
    // Gọi hàm getFlightByCode từ Flight.c để tìm chuyến bay theo mã chuyến bay
    if (getFlightByCode(flightFile, flightCode, flight) == 0) {
        return 0;  // Chuyến bay tìm thấy
    } else {
        return -1; // Không tìm thấy chuyến bay
    }
}

// Hàm xử lý đặt vé
int bookFlight(const char* userFile, const char* flightFile, User* user, const char* flightCode, int ticketCount) {
    Flight flight;

    printf("Ticket Count inside bookFlight: %d\n", ticketCount);
    // Tìm chuyến bay
    if (findFlight(flightFile, flightCode, &flight) != 0) {
        return -1;  // Chuyến bay không tìm thấy
    }

    // Kiểm tra số ghế còn lại và số dư tài khoản người dùng
    if (flight.remainingSeats >= ticketCount && user->money >= flight.price * ticketCount) {
        // Giảm số ghế còn lại
        flight.remainingSeats -= ticketCount;

        // Cập nhật lại thông tin chuyến bay trong file
        if (updateFlightSeats(flightFile, &flight) != 0) {
            return -1;  // Không thể cập nhật thông tin chuyến bay
        }

        // Trừ tiền tài khoản người dùng
        user->money -= flight.price * ticketCount;

        // Cập nhật lại thông tin người dùng trong file
        if (updateUserBalance(userFile, user) != 0) {
            return -1;  // Cập nhật tài khoản không thành công
        }

        return 0;  // Đặt vé thành công
    } else {
        return -1;  // Không đủ ghế hoặc không đủ tiền
    }
}

