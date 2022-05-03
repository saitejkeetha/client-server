// Server side
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <math.h>
#define min(a,b) fmin(a,b)
#define PORT 5056




int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	//	char* hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
			== 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
				sizeof(address))
			< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		printf("Listening\n");
		if ((new_socket	= accept(server_fd, (struct sockaddr*)&address,
						(socklen_t*)&addrlen))
				< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		else
			memset(&buffer, 0, sizeof(buffer));			
		printf("Client connected\n");

		FILE *fp;
		char buffer[1024];
		memset(&buffer, 0, sizeof(buffer));
		if(( fp = fopen(argv[2], "w")) == NULL)
			printf("Error in accessing file\n");
		int nbytes = 0;
		/*while((nbytes = recv(new_socket, buffer , 1024 , 0)) > 0)
		{
			printf("%s\n", buffer);
			printf("%d\n", nbytes);
			if( (fwrite(buffer, strlen(buffer), 1, fp)) < 0)
				printf("Error in writing\n");
			memset(&buffer, 0, sizeof(buffer));
		}*/
		recv(new_socket, buffer, 1024, 0);
		printf("%s\n", buffer);
			if( (fwrite(buffer, strlen(buffer), 1, fp)) < 0)
				printf("Error in writing\n");
		fclose(fp);

		FILE *fp_cmd;
		char path[1000];
		sprintf(path, "grep -w %s %s", argv[1], argv[2]);
		printf("%s\n", path);
		/* Open the command for reading. */
		fp_cmd = popen(path, "r");
		if (fp_cmd == NULL) {
			printf("Failed to run command\n" );
			exit(1);
		}

		/* Read the output a line at a time - output it. */
		while (fgets(buffer, sizeof(buffer), fp_cmd) != NULL) {
			printf("%s\n", buffer);
			ssize_t len =	send(new_socket, buffer, sizeof(buffer), 0);
		//	printf("%zd\n", len);
		}

		/* close */
		pclose(fp_cmd);
		/*valread = read(new_socket, buffer, 1024);
		  printf("%s\n", buffer);
		  send(new_socket, hello, strlen(hello), 0);
		  printf("Hello message sent\n");*/
	}
	return 0;
}

