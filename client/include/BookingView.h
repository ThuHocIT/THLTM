#ifndef BOOKING_VIEW_H
#define BOOKING_VIEW_H

// Hàm nhập mã chuyến bay và gửi yêu cầu đặt vé đến server
void bookFlight(int sock);

// Hàm hiển thị kết quả từ server khi thực hiện đặt vé
void displayBookingResponse(const char* response);

#endif // BOOKING_VIEW_H
