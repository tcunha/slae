#include <curl/curl.h>

#include <openssl/sha.h>

#include <err.h>
#include <stdio.h>
#include <string.h>

#define COMMON_APIKEY		"decfe466-4db7-4a7c-b0b8-f067cceaf94d"
#define COMMON_BASE		"https://content.guardianapis.com"
#define COMMON_ENDPOINT		"/search"
#define COMMON_ENDPOINT_ARGS	"?section=science&page-size=1&api-key="
#define COMMON_URL		\
    COMMON_BASE			\
    COMMON_ENDPOINT		\
    COMMON_ENDPOINT_ARGS	\
    COMMON_APIKEY

size_t
common_write(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	char	*buf = (char *) userdata;

	strncpy(buf, ptr, BUFSIZ - 1);
	buf[BUFSIZ - 1] = '\0';		/* Guarantee a NUL. */

	return (nmemb * size);		/* Size is always 1; no overflow. */
}

void
common_key(unsigned char *key)
{
	CURL		*curl;
	char		*end;
	const char	*start;
	char	 	 r[BUFSIZ];	/* Response (should be enough). */

	if (curl_global_init(CURL_GLOBAL_ALL) != 0)
		errx(1, "curl_global_init");
	if ((curl = curl_easy_init()) == NULL)
		errx(1, "curl_easy_init");
	if (curl_easy_setopt(curl, CURLOPT_URL, COMMON_URL) != 0)
		errx(1, "curl_easy_setopt");
	if (curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, common_write) != 0)
		errx(1, "curl_easy_perform");
	if (curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &r) != 0)
		errx(1, "curl_easy_perform");
	if (curl_easy_perform(curl) != 0)
		errx(1, "curl_easy_perform");

	/* Generate a key by finding the id and performing a SHA256 over it. */
	if ((start = strstr(r, "id")) == NULL)
		errx(1, "start not found");
	start += 5;				/* Go over the attribute. */

	if ((end = strchr(start, '"')) == NULL)
		errx(1, "end not found");
	*end = '\0';				/* Turn it into a NUL byte. */

	/* The key needs to be 32 bytes. */
	printf("Generating key from: %s\n", start);
	SHA256((unsigned char *) start, strlen(start), key);
}
