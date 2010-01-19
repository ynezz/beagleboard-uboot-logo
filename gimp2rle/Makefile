CFLAGS=-Wall -W -Wmissing-prototypes -Wmissing-declarations -Werror

all: gimp2rle

gimp2rle: gimp2rle.c
	gcc $(CFLAGS) $< -o $@
clean:
	-rm gimp2rle
