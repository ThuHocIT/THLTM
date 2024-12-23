#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "../../model/User.h"
#include "../../model/Flight.h"

// Định nghĩa đường dẫn đến file chứa dữ liệu chuyến bay
#define FLIGHTS_FILE "data/flights.txt"

// Hàm đặt vé cho người dùng
// Trả về 0 nếu thành công, các giá trị khác báo lỗi:
    // -1: Không tìm thấy chuyến bay
    // -2: Không đủ ghế
    // -3: Không đủ tiền
    // -4: Lỗi cập nhật thông tin người dùng
    // -5: Lỗi cập nhật thông tin chuyến bay
int findFlight(const char* filename, const char* flightCode, Flight* flight);

int bookFlight(const char* userFile, const char* flightFile, User* user, const char* flightCode, int ticketCount);

#endif // FLIGHT_CONTROLLER_H
