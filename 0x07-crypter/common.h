#ifndef COMMON_H
#define COMMON_H

#define CRYPTER_SSLERR() do {		\
    ERR_print_errors_fp(stderr);	\
    exit(1);				\
} while (0)

#define QUOTE(s)	#s
#define STR(s)		QUOTE(s)

extern void	common_key(unsigned char *);

#endif
