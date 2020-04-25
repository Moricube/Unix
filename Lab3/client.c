#include <stdio.h>
#include <sys/socket.h>
#include <resolv.h>

#ifndef MSG_FIN
#error MSG_FIN or MSG_EOF not defined
#endif

void PANIC(char *msg);
#define PANIC(msg)	{perror(msg);abort();}

int main(int count, char *strings[]) {	
    int sd = socket(PF_INET, SOCK_STREAM, 0);;
	int port = 9000;
	char *host = "127.0.0.1";
    char *msg = strings[1];
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	char buffer[1024];

	if (count == 4) {
		host = strings[2];
		port = atoi(strings[3]);
        printf("Client using host: %s\n", host);
        printf("Client using port: %d\n", port);
	} else {
        printf("Client using default host: %s\n", host);
        printf("Client using default port: %d\n", port);
    }

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_aton(host, &addr.sin_addr);

    if (connect(sd, &addr, sizeof(addr)) != 0) {
        PANIC("connect");
    }

	// Send message
	// if (sendto(sd, msg, sizeof(msg), 0, &addr, sizeof(addr)) < 0) {
	// 	PANIC("sendto");
    // }

	if (sendto(sd, msg, sizeof(msg), 0, &addr, sizeof(addr)) < 0) {
		PANIC("sendto");
    }

	bzero(buffer, sizeof(buffer));

	// Get message
	recvfrom(sd, buffer, sizeof(buffer), 0, &addr, sizeof(addr));
	printf("message: %s\n", buffer);
	close(sd);
}