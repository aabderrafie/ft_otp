CC = c++
CFLAGS = -Wall -Wextra -std=c++17 -O3 

INCLUDES = -I/opt/homebrew/opt/openssl@3/include
LIBS = -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

NAME = ft_otp

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

all: $(obj)
	$(CC) $(CFLAGS) -o $(NAME) $(obj) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(obj)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
