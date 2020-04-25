#include <stdio.h>
#include <sys/socket.h>
#include <resolv.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>

#define MAXPROCESSES 1
#define MAXCONNECTIONS 1
#define _XOPEN_SOURCE 700

int numChild = 0;

// handler
void sig_child(int signum) {
	wait(0);
	numChild--;
}

void servlet(int sd) {
    fd_set fdSet;
	int maxfd = sd;
	int ceiling = 0;

	FD_ZERO(&fdSet);
	FD_SET(sd, &fdSet);

	while (1) {
        struct timeval timeout = { 2, 0 }; // 2 sec

    	// Wait for some action
		// Using "select" instead "pselect" - not a big difference, i hope
	    if (select(maxfd + 1, &fdSet, 0, 0, &timeout) > 0) {
        	// If new connection, then connect and add to list
			if (FD_ISSET(sd, &fdSet)) {
			    if ( ceiling < MAXCONNECTIONS ) {
                    int client = accept(sd, 0, 0);

		        	if (maxfd < client) {
        		    	maxfd = client;
                    }

			        FD_SET(client, &fdSet);
    	    		ceiling++;
					printf("Select process #%d: %d has been connected\n", getpid(), ceiling);
			    }
	        } else {
                int i;

				for ( i = 0; i < maxfd + 1; i++ ) {
					if (FD_ISSET(i, &fdSet)) {
                        char buffer[1024];
						printf("Buffer is: %s\n\n", buffer);
						int bytes;

						bytes = recv(i, buffer, sizeof(buffer), 0);
						
                        // Сheck for closed
                        if (bytes < 0) {
							close(i);
							FD_CLR(i, &fdSet);
							ceiling--;
							printf("Select process #%d: %d has been connected\n", getpid(), ceiling);
						} else { 
                            send(i, buffer, bytes, 0);
                        }
					}
				}
			}
    	}
	}
	exit(0);
}

int main(int count, char *strings[]) {
    int sd = socket(PF_INET, SOCK_STREAM, 0);
	int port = 9000;
	struct sockaddr_in addr;
	struct sigaction action;

    if (count != 2) {
		printf("Server listening on default port: %d\n", port);
    } else {
		port = atoi(strings[1]);
        printf("Server listening on port: %d\n", port);
    }

	bzero(&action, sizeof(action));
	action.sa_handler = sig_child;
	action.sa_flags = SA_NOCLDSTOP | SA_RESTART;

	if (sigaction(SIGCHLD, &action, 0) != 0) {
		perror("sigaction()");
    }

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(strings[1]));
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sd, &addr, sizeof(addr)) == 0) {
		listen(sd, 20);

		while (1) {
			if (numChild < MAXPROCESSES) {
                int pid;

				if ((pid = fork()) == 0) {
					servlet(sd);
                } else if (pid > 0) {
					numChild++;
                } else {
					perror("fork()");
                }
			} else {
				sleep(1);
            }
		}
	} else {
        perror("bind()");
    }	

	return 0;
}