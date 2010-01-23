/* 
 * RLE logo converter for the Beagleboard's U-Boot
 *
 * Copyright (c) 2010 Petr Stetiar <ynezz@true.cz>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/* here include your Gimp logo */
#include "gaben-beagle-logo.h"

/* name of the output filename remember, that if you change it, don't forget
 * to change it in beagle.c also */
#define LOGO_FILENAME "logo.h"

/* usually you don't need to change this */
#define MAX_COLORS 256

#include <stdio.h>

static char header[] = "\
/*\n\
 * Automagically generated using gimp2rle (c) 2010 Petr Stetiar <ynezz@true.cz>\n\
 * It's available on http://github.com/ynezz/beagleboard-uboot-logo\n\
 */\n\n\
#ifndef _LOGO_H_\n\
#define _LOGO_H_\n\n\
#define HEADER_PIXEL(data,pixel) {\\\n\
pixel[0] = header_data_cmap[(unsigned char)data][0]; \\\n\
pixel[1] = header_data_cmap[(unsigned char)data][1]; \\\n\
pixel[2] = header_data_cmap[(unsigned char)data][2]; \\\n\
}\n\n\
";

int main(void)
{
	char last = 0;
	char current = 0;
	(void) header_data_cmap;
	unsigned long i = 0;
	unsigned long count = 0;
	unsigned long rle_size = 0;
	FILE *fd = NULL;

	fd = fopen(LOGO_FILENAME, "wb+");
	if (fd == NULL) {
		fprintf(stderr, "Can't open logo file '%s'\n",
			LOGO_FILENAME);
		return -1;
	}

	fprintf(fd, "%s", header);
	fprintf(fd, "static char header_data_cmap[%d][3] = {\n",
		MAX_COLORS);

	/* color table */
	for (i=0; i < MAX_COLORS; i++) {
		fprintf(fd, "\t{%3d, %3d, %3d},\n",
			(unsigned char) header_data_cmap[i][0],
			(unsigned char) header_data_cmap[i][1],
			(unsigned char) header_data_cmap[i][2]);
	}

	fprintf(fd, "};\n\n");
	fprintf(fd, "static unsigned long header_data_rle[][2] = {\n");

	/* RLE data */
	last = header_data[i];
	for (i=0; i < width*height; i++) {
		current = header_data[i];
		count++;

		if (current != last) {
			fprintf(fd, "\t {%lu, %d}, \n", count, last);
			rle_size++;
			last = current;
			count = 0;
		}
	}

	rle_size++;
	fprintf(fd, "\t {%lu, %d},\n};\n", count, last);
	fprintf(fd, "static unsigned long header_data_rle_size = %lu;\n", rle_size);
	fprintf(fd, "#endif /* _LOGO_H_ */\n");

	return 0;
}
