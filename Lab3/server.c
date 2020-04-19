#include <stdio.h>
#include <sys/socket.h>
#include <resolv.h>

void PANIC(char* msg);
#define PANIC(msg) { perror(msg); abort(); }

int main(int count, char *strings[]) {
    int sd = socket(PF_INET, SOCK_STREAM, 0);
	int port = 9000;
	struct sockaddr_in addr;

	if (count != 2) {
		printf("Server listening on default port: %d\n", strings[1], port);
    } else {
		port = atoi(strings[1]);
        printf("Server listening on port: %d\n", port);
    }

	bzero(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sd, &addr, sizeof(addr)) != 0) {
        PANIC("bind");
    }

	if (listen(sd, 20) != 0) {
	    PANIC("listen");
    }

	while (1) {
        char buffer[1024];
		int client = accept(sd, 0, 0);

		printf("Connected socket\n");
        printf("Buffer is: %s\n\n", buffer);
		send(client, buffer, recv(client, buffer, sizeof(buffer), 0), 0);
		close(client);
	}

	return 0;
}