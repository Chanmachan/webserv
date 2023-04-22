SERVER = server
CLIENT = client
S_SRC = server.cpp
S_OBJ = $(S_SRC:.c=.o)
C_SRC = client.cpp
C_OBJ = $(C_SRC:.c=.o)
CC = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

all: $(SERVER) $(CLIENT)

$(SERVER): $(S_OBJ)
	$(CC) $(CXXFLAGS) -o $(SERVER) $(S_OBJ)

$(CLIENT): $(C_OBJ)
	$(CC) $(CXXFLAGS) -o $(CLIENT) $(C_OBJ)

clean:
	$(RM) $(S_OBJ) $(C_CLIENT)

fclean:
	$(RM) $(SERVER) $(CLIENT)

re: fclean all

.PHONY: all clean fclean re