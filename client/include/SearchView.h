#ifndef SEARCH_VIEW_H
#define SEARCH_VIEW_H

// Hàm nhập mã chuyến bay và gửi yêu cầu đặt vé đến server
void searchFlight(int sock);

// Hàm hiển thị kết quả từ server khi thực hiện đặt vé
void displaySearchResults(const char* response);
void clearBuffer();

#endif // SEARCH_VIEW_H
