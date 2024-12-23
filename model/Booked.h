#ifndef BOOKED_H
#define BOOKED_H

#include "User.h"
#include "Flight.h"

typedef struct {
    User user;           // Thông tin người dùng đã đặt vé
    Flight flight;       // Thông tin chuyến bay đã đặt
    char bookingDate[11]; // Ngày đặt vé (ví dụ: "2024-12-02")
    int numberOfSeats;    // Số ghế đã đặt
    float totalPrice;     // Tổng giá trị đơn đặt vé
} Booked;

int saveBookedFlight(const char* file_path, Booked* bookedFlight);

int findBookedFlight(const char* file_path, const char* username, const char* flightCode, Booked* foundBooked);

#endif // BOOKED_H
