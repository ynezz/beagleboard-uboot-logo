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
#include "logo.h"
#include <stdio.h>

int main(void)
{
	char last = 0;
	char current = 0;
	(void) header_data_cmap;
	unsigned long i = 0;
	unsigned long count = 0;
	unsigned long rle_size = 0;

	fprintf(stdout, "static unsigned long header_data_rle[][2] = {\n");
	last = header_data[i];

	for (i=0; i < width*height; i++) {
		current = header_data[i];
		count++;

		if (current != last) {
			fprintf(stdout, "\t {%lu, %d}, \n", count, last);
			rle_size++;
			last = current;
			count = 0;
		}
	}

	rle_size++;
	fprintf(stdout, "\t {%lu, %d},\n};\n", count, last);
	fprintf(stdout, "static unsigned long header_data_rle_size = %lu;\n", rle_size);

	return 0;
}
