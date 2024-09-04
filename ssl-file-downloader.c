/**
 *
 * @Author: Max Base
 * @Repository: https://github.com/BaseMax/ssl-file-downloader-c
 * @Date: 09/04/2024
 * @Build: gcc -o ssl-file-downloader ssl-file-downloader.c -lssl -lcrypto
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFFER_SIZE 2048

int main()
{
	const char *hostname = "raw.githubusercontent.com";
	const char *path = "/SalamLang/Salam/main/src/array.c";

	const char *port = "443";
	char request[1024];
	char buffer[BUFFER_SIZE];

	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	size_t bytes_received;
	FILE *fp;
	int header_found = 0;
	SSL_CTX *ctx;
	SSL *ssl;

	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

	ctx = SSL_CTX_new(TLS_client_method());
	if (!ctx)
	{
		fprintf(stderr, "SSL_CTX_new() failed.\n");
		return 1;
	}

	snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, hostname);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		SSL_CTX_free(ctx);
		return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("connect");
			continue;
		}
		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "Failed to connect\n");
		freeaddrinfo(servinfo);
		SSL_CTX_free(ctx);
		return 2;
	}

	freeaddrinfo(servinfo);

	ssl = SSL_new(ctx);
	if (!ssl)
	{
		fprintf(stderr, "SSL_new() failed.\n");
		close(sockfd);
		SSL_CTX_free(ctx);
		return 1;
	}

	SSL_set_fd(ssl, sockfd);

	if (SSL_connect(ssl) <= 0)
	{
		ERR_print_errors_fp(stderr);
		SSL_free(ssl);
		close(sockfd);
		SSL_CTX_free(ctx);
		return 1;
	}

	SSL_write(ssl, request, strlen(request));

	fp = fopen("downloaded_file.txt", "wb");

	if (!fp)
	{
		perror("fopen");
		SSL_free(ssl);
		close(sockfd);
		SSL_CTX_free(ctx);
		return 3;
	}

	printf("open file for writing\n");

	while ((bytes_received = SSL_read(ssl, buffer, BUFFER_SIZE)) > 0)
	{
		printf("byte received\n");
		if (!header_found)
		{
			char *header_end = strstr(buffer, "\r\n\r\n");
			if (header_end)
			{
				header_found = 1;
				size_t header_length = header_end - buffer + 4;
				fwrite(header_end + 4, 1, bytes_received - header_length, fp);
			}
		}
		else
		{
			fwrite(buffer, 1, bytes_received, fp);
		}
	}

	// while ((bytes_received = SSL_read(ssl, buffer, BUFFER_SIZE)) > 0)
	// {
	// 	fwrite(buffer, 1, bytes_received, fp);
	// }

	fclose(fp);
	SSL_free(ssl);
	close(sockfd);
	SSL_CTX_free(ctx);

	return 0;
}
