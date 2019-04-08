CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror `pkg-config gtk+-3.0 --cflags`
LDFLAGS = `pkg-config gtk+-3.0 --libs`

NAME = reservation-system

$(NAME): main.o views.o time_my.o database.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(NAME)