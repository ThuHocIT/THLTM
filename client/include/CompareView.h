#ifndef COMPAREVIEW_H
#define COMPAREVIEW_H

#define BUFFER_SIZE 4096

// Hàm để thực hiện tìm kiếm chuyến bay
void compareFlight(int sock);

// Hàm hiển thị kết quả tìm kiếm chuyến bay
void displayCompareResults(const char* response);


#endif
