# Cấu hình thông tin chung
CC = gcc
CFLAGS = -Wall -g

# Thư mục nguồn
SERVER_SRC = server/src/main.c server/src/User.c server/src/UserController.c server/src/Flight.c server/src/BookingController.c server/src/Booked.c server/src/BookedController.c server/src/SearchController.c server/src/CompareController.c 
CLIENT_SRC = client/src/main.c client/src/LoginView.c client/src/BookingView.c client/src/MenuView.c client/src/BookedView.c client/src/SearchView.c client/src/CompareView.c

# Các file đối tượng
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

# Tên các file thực thi
SERVER_BIN = server_bin
CLIENT_BIN = client_bin

# Đối tượng mặc định
all: $(SERVER_BIN) $(CLIENT_BIN)

# Cấu hình cho server
$(SERVER_BIN): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_OBJ)

# Cấu hình cho client
$(CLIENT_BIN): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_OBJ)

# Quy trình biên dịch file đối tượng cho server
server/src/%.o: server/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Quy trình biên dịch file đối tượng cho client
client/src/%.o: client/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Xóa các file đối tượng và thực thi
clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_BIN) $(CLIENT_BIN)

# Lệnh chạy server và client
run-server: $(SERVER_BIN)
	./$(SERVER_BIN)

run-client: $(CLIENT_BIN)
	./$(CLIENT_BIN)

