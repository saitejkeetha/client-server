// Client side
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h> // open function
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <math.h>
#define min(a,b) fmin(a,b)
#define PORT 5056

void func(int sock, const char *filename)
{
	char buffer[1024];
	/*	while(1) {
		recv(sock, buffer , sizeof(buffer) , 0);
		printf("%s\n", buffer);
		}*/
	int nbytes = 0;
	while((nbytes = recv(sock, buffer , 1024 , 0)) > 0)
	{
		printf("%s:%s", filename, buffer);
		/*if( (fwrite(buffer, strlen(buffer), 1, fp)) < 0)
		  printf("Error in writing\n");
		  memset(&buffer, 0, sizeof(buffer));*/
	}
	/*send(sock, hello, strlen(hello), 0);
	  printf("Hello message sent\n");
	  valread = read(sock, buffer, 1024);
	  printf("%s\n", buffer);*/
}


int main(int argc, char const* argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buf[1024] = { 0 };
	struct stat file_stat;
	char file_size[256];
	ssize_t len;
	int offset;
	int remain_data;

	if(argc < 4)
	{
		printf("Usage: %s <pattern_to_search> <file1_path> <file2_path>\n", argv[0]);
		return -1;
	}

	printf("Pattern to search: %s\n", argv[1]);
	printf("File path 1 %s\n", argv[2]);
	printf("File path 2 %s\n", argv[3]);

	FILE *file1, *file2;
	if((file1 = fopen(argv[2],"r"))!=NULL)
	{
		fclose(file1);
	}
	else
	{
		printf("Error in accessing file: %s\n", argv[2]);
		return -1;
	}
	if((file2 = fopen(argv[3],"r"))!=NULL)
	{
		fclose(file2);
	}
	else
	{
		printf("Error in accessing file: %s\n", argv[3]);
		return -1;
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "192.10.23.13", &serv_addr.sin_addr)
			<= 0) {
		printf(
				"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr))
			< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	else
		printf("Connection Successful\n");

	FILE *fp;
	char buffer[1024];
	fp = fopen(argv[3], "r");

	while (!feof(fp)) // to read file
	{
		// function used to read the contents of file
		fread(buffer, sizeof(buffer), 1, fp);
		//printf("%s\n", buffer);
		send(sock, buffer, sizeof(buffer), 0);
	}
	fclose(fp);

	char temp[1000];
	sprintf(temp, "grep -w %s %s", argv[1], argv[2]);
	printf("%s\n", temp);
	FILE *fp_cmd = popen(temp, "r");
	if (fp_cmd == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}

	/* Read the output a line at a time - output it. */
	while (fgets(buffer, sizeof(buffer), fp_cmd) != NULL) {
		printf("%s:%s", argv[2], buffer);
		//ssize_t len =   send(new_socket, buffer, sizeof(buffer), 0);
		//      printf("%zd\n", len);
	}

	/* close */
	pclose(fp_cmd);
	//system(temp);
	func(sock, argv[3]);
        return 0;
}

