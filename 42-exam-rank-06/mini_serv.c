#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>


#define ERR_ARG "Wrong number of arguments\n"
#define ERR_FAT "Fatal error\n"

typedef struct s_client {
	int fd, new, id;
	struct s_client *next;
} t_client;

static int id = 0;
t_client *clients;
fd_set wr_set, rd_set, cur_set;
int sockfd;
char buff[64];

void fatal() {
	write(1, ERR_FAT, strlen(ERR_FAT));
	exit(1);
}


int get_max_fd() {
	int max = sockfd;
	for (t_client *tmp = clients; tmp; tmp = tmp->next) {
		if (tmp->fd < max)
			max = tmp->fd;
	}
	return max;
}

void del_client(int fd) {
	t_client *del = clients;
	if (clients && clients->fd == fd) {
		FD_CLR(fd, &cur_set);
		clients = clients->next;
		free(del);
		return ;
	}
	for (t_client *tmp = clients; tmp->next; tmp = tmp->next)
		if (tmp->next->fd == fd) {
			del = tmp->next;
			FD_CLR(fd, &cur_set);
			tmp->next = tmp->next->next;
			free(del);
			return ;
		}
}

t_client *get_client(int fd) {
	for (t_client *tmp = clients; tmp; tmp = tmp->next)
		if (tmp->fd == fd)
			return tmp;
	return NULL;
}

void send_all(char *buffer, size_t length, int sender) {
        for (t_client *client = clients; client; client = client->next)
                if (client->fd != sender && FD_ISSET(client->fd, &wr_set))
                        if (send(client->fd, buffer, length, 0) < 0)
                                fatal();
}

void accept_client() {
	t_client *new = calloc(1, sizeof(t_client));
	new->id = id++;
	new->new = 1;
	new->fd = accept(sockfd, 0, 0);
	if (new->fd < 0)
		fatal();
	if (!clients)
		clients = new;
	else {
		t_client *tmp = clients;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	FD_SET(new->fd, &cur_set);
	sprintf(buff, "server: client %d just arrived\n", new->id);
	send_all(buff, strlen(buff), new->fd);
}

int main(int argc, char *argv[]) {
	struct sockaddr_in servaddr;

	if (argc != 2) {
		write(2, ERR_ARG, strlen(ERR_ARG));
		return 1;
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		fatal();
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(argv[1]));

	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal();

	if (listen(sockfd, 0) != 0)
		fatal();

	FD_SET(sockfd, &cur_set);
	printf("%d\n", FD_SETSIZE);
	printf("%d\n", get_max_fd());
	while (42) {
		wr_set = rd_set = cur_set;
		if (select(get_max_fd() + 1, &rd_set, &wr_set, 0, 0) < 0)
			continue ;
		for (int fd = 0; fd < get_max_fd() + 1; ++fd) {
			if (FD_ISSET(fd, &rd_set)) {
				printf("in\n");
				if (fd == sockfd) {
					printf("accept\n");
					accept_client();
				}
				else {
					t_client *client= get_client(fd);
					char c;
					if (recv(fd, &c, 1, 0) <= 0) {
						printf("quit\n");
						del_client(fd);
						sprintf(buff, "server: client %d just left\n", client->id);
						send_all(buff, strlen(buff), fd);
						close(fd);
					}
					else {
						printf("text\n");
						if (client->new) {
							sprintf(buff, "client %d: ", client->id);
							send_all(buff, strlen(buff), fd);
							client->new = 0;
						}
						if (c == '\n')
							client->new = 1;
						send_all(&c, 1, fd);
					}

				}
			}
		}
	}
}
