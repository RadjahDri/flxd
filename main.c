/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Bart Van Der Meerssche <bart@flukso.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libubox/uloop.h>
#include "config.h"
#include "flx.h"

struct config conf = {
	.verbosity = 0,
	.flx_ufd = {
		.cb = flx_rx
	}
};

static int usage(const char *progname)
{
	fprintf(stderr,
		"Usage: %s [<options>]\n"
		"Options:\n"
		"  -v:	Turn on verbosity\n"
		"\n", progname);
	return 1;
}

int main(int argc, char **argv)
{
	int ret = 0;
	int opt;

	while ((opt = getopt(argc, argv, "hv")) != -1) {
		switch (opt) {
		case 'v':
			conf.verbosity++;
			break;
		case 'h':
		default:
			return usage(argv[0]);
		}	
	}

	conf.flx_ufd.fd = open(FLX_DEV, O_RDWR);
	if (conf.flx_ufd.fd < 0) {
		perror(FLX_DEV);
		ret = -1;
		goto out;
	}

	uloop_init();
	uloop_fd_add(&conf.flx_ufd, ULOOP_READ);
	uloop_run();

out:
	uloop_done();
	close(conf.flx_ufd.fd);
	return ret;
}

