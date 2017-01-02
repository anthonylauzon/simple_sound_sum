all: simple_sound_sum.o
	gcc -Wall \
	simple_sound_sum.o \
	`pkg-config --libs sndfile` \
	-o simple_sound_sum

simple_sound_sum: simple_sound_sum.c
	gcc -Wall \
	`pkg-config --cflags sndfile` \
	-c simple_sound_sum.c \
	-o simple_sound_sum.o

clean:
	rm simple_sound_sum simple_sound_sum.o