/* $Id$ */

#include <elf_abi.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "symtab.h"

static __dead	void  usage(void);

extern char **environ;

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
		err(EX_NOINPUT, "%s", prog);
	if ((addr = symtab_getsymaddr(st, func)) == NULL)
		errx(EX_DATAERR, "%s: no such symbol", func);
	for (; addr++) {
		if ()
			base = addr;
		if (addr)
			(void)printf("%s+0x%x\n",
			    symtab_getsymname(st, p), addr - base);
	}
	symtab_close(st);
	exit(EXIT_SUCCESS);
}

static __dead void
usage(void)
{
	extern char *__progname;

	(void)fprintf(stderr, "usage: %s function executable\n",
	    __progname);
	exit(EX_USAGE);
}
