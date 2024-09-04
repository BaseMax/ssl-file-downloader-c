# SSL File Downloader in C

This project is a simple SSL file downloader written in C. The program connects to a remote server over HTTPS, sends an HTTP GET request, and downloads the specified file. The downloaded file is saved locally as downloaded_file.txt.

The project utilizes OpenSSL for handling the SSL/TLS connections and includes basic error handling for networking and SSL-related operations.

## Features

- Connects to an HTTPS server using SSL/TLS.
- Sends a GET request to download a file.
- Parses the HTTP response to skip the headers.
- Saves the file content to a local file named downloaded_file.txt.

## Prerequisites

To build and run this project, you'll need:

- GCC (or any C compiler)
- OpenSSL library

## Building the Project

To compile the code, run the following command:

```bash
gcc -o ssl-file-downloader ssl-file-downloader.c -lssl -lcrypto
```

This will produce an executable named ssl-file-downloader.

## Running the Program

Once compiled, you can run the program using:

```bash
./ssl-file-downloader
```

This will download the file from the specified URL (`https://raw.githubusercontent.com/SalamLang/Salam/main/src/array.c`) and save it as `downloaded_file.txt` in the current directory.

## Installing OpenSSL (Linux)

If OpenSSL is not installed, you can install it using the following command:

```bash
sudo apt-get install libssl-dev
```

## Code Explanation

- SSL Initialization: The program initializes the OpenSSL library and sets up an SSL context using the TLS_client_method() function.
- Socket Setup: The program resolves the hostname using getaddrinfo() and establishes a connection to the server using socket() and connect().
- SSL Connection: An SSL object is created and associated with the socket file descriptor using SSL_set_fd(). The program then performs an SSL handshake using SSL_connect().
- HTTP Request and Response Handling: The program sends a GET request to the server and reads the response. It searches for the end of the HTTP headers and begins writing the file content to downloaded_file.txt.
- Cleanup: After the download is complete, the program releases all resources, including closing the file, the SSL connection, and the socket.

## Important Notes

- Hardcoded URL: The hostname and file path are currently hardcoded in the source code. Modify these variables to download different files.
- File Output: The downloaded file is saved as downloaded_file.txt. Ensure that the program has write permissions in the directory where it is executed.

### License

This project is licensed under the GPL-3.0 License. See the LICENSE file for more details.

### Author

Max Base

### Contributions

Contributions, issues, and feature requests are welcome! Feel free to check the issues page if you have any questions or suggestions.

Copyright 2024, Max Base
