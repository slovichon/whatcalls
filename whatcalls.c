/* $Id$ */

#include <elf_abi.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "symtab.h"

void  usage(void);

int
main(int argc, char **argv)
{
	unsigned long addr, base;
	char *prog, *func;
	struct symtab *st;
	int fd;

	if (argc != 2)
		usage();

	func = argv[1];
	prog = argv[2];

	if ((st = symtab_open(prog)) == NULL)
		err(1, "%s", prog);
	if ((addr = symtab_getsymaddr(st, func)) == NULL)
		errx(1, "%s: no such symbol", func);
	for (;; addr++) {
#if 0
		if ()
			base = addr;
		if (addr)
			printf("%s+0x%x\n",
			    symtab_getsymname(st, p), addr - base);
#endif
	}
	symtab_close(st);
	exit(0);
}

void
usage(void)
{
	extern char *__progname;

	fprintf(stderr, "usage: %s function executable\n",
	    __progname);
	exit(1);
}
