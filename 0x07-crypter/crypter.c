#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

unsigned char shellcode[] = STR(SHELLCODE);

int
main(void)
{
	EVP_CIPHER_CTX	*ctx;
	unsigned char	 b[BUFSIZ];	/* Crypted buffer (should be enough). */
	unsigned char	 iv[12];	/* The IV length is 96 bits. */
	unsigned char	 key[32];	/* The key length is 256 bits. */
	unsigned char	*ptr = b;
	int		 bl, ol;

	common_key(key);				/* Get dynamic key. */

	if (RAND_bytes(iv, sizeof iv) == 0)		/* Get a random IV. */
		CRYPTER_SSLERR();
	if ((ctx = EVP_CIPHER_CTX_new()) == NULL)
		CRYPTER_SSLERR();
	if (EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv) == 0)
		CRYPTER_SSLERR();
	if (EVP_EncryptUpdate(
	    ctx, b, &bl, shellcode, strlen((char *) shellcode)) == 0)
		CRYPTER_SSLERR();
	if (EVP_EncryptFinal_ex(ctx, b + bl, &ol) == 0)
		CRYPTER_SSLERR();

	/* Check for NUL bytes in the crypted shellcode. */
	while (ptr != b + bl + ol) {	/* Final buffer size is bl plus ol. */
		if (*ptr == 0)
			errx(1, "NUL byte found!"); /* There might be more! */
		ptr++;
	}

	for (ptr = key; ptr != key + sizeof key; ptr++)
		printf("\\x%02x", *ptr);
	puts("");

	/* Print the shellcode prefixed with the randomly generated IV. */
	for (ptr = iv; ptr != iv + sizeof iv; ptr++)
		printf("\\x%02x", *ptr);
	for (ptr = b; ptr != b + bl + ol; ptr++)
		printf("\\x%02x", *ptr);
	puts("");

	return (0);
}
