#ifndef BOOKEDVIEW_H
#define BOOKEDVIEW_H

// Hàm hiển thị chuyến bay đã đặt
void displayBookedFlights(int socket_fd);

// Hàm hủy vé đã đặt
void cancelBooked(int socket_fd);

// Hàm menu chính cho BookedView
void bookedMenu(int socket_fd);

#endif // BOOKINGVIEW_H
