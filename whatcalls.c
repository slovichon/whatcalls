/* $Id$ */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <elf_abi.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

static __dead void usage(void);
static char *getpath(const char *, const char *);

extern char **environ;

int
main(int argc, char **argv)
{
	char *prog, *func, *path;
	int status;

	if (argc < 3)
		usage();

	func = argv[1];
	prog = argv[2];

	switch (fork()) {
	case -1:
		err(EX_OSERR, "fork");
		/* NOTREACHED */
	case 0:
		if ((path = getpath(prog, getenv("PATH"))) == NULL) {
			if (!errno)
				errno = ENOENT;
			err(errno == ENOENT ? EX_DATAERR : EX_OSERR,
			    "%s", prog);
		}
		(void)execve(path, argv + 2, environ);
		free(path);
		err(EX_OSERR, "exec %s", prog);
		/* NOTREACHED */
	default:
		break;
	}

	status = EXIT_SUCCESS;
	(void)wait(&status);
	exit(status);
}

static char *
getpath(const char *prog, const char *e)
{
	char *env, *p, path[MAXPATHLEN];
	struct stat stb;

	if ((env = strdup(e)) == NULL)
		err(EX_OSERR, "strdup");

	path[0] = '\0';
	for (; env != NULL; env = p) {
		p = strchr(env, ':');
		if (p != NULL)
			*p++ = '\0';
		(void)snprintf(path, sizeof(path), "%s/%s", env, prog);
		if (stat(path, &stb) == -1) {
			switch (errno) {
			case ENOENT:
				break;
			default:
				err(EX_OSERR, "%s", path);
			}
		}
	}
	free(env);
	return (*path == '\0' ? NULL : strdup(path));
}
	
static __dead void
usage(void)
{
	extern char *__progname;

	(void)fprintf(stderr, "usage: %s function executable "
	    "[argument ...]\n", __progname);
	exit(EX_USAGE);
}
