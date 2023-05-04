#include "server.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int available_expert(int *experts_list) {
  for (int i = 0; i < MAX_USERS + 3; i++) {
    if (experts_list[i] == 0)
      return i;
  }
  return -1;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage : ./server <server_port>\n");
    exit(1);
  }
  char *end;
  unsigned long serv_port = strtoul(argv[1], &end, 10);
  if (strcmp(end, "\0")) {
    fprintf(stderr, "%s is not a valid port number", argv[1]);
    exit(1);
  }
  if (serv_port > 65535) {
    fprintf(stderr, "%lu is not a valid port number", serv_port);
    exit(1);
  }
  fd_set master;                      // master file descriptor list
  fd_set read_fds;                    // temp file descriptor list for select()
  int fdmax;                          // maximum file descriptor number

  int listener;                       // listening socket descriptor
  int newfd;                          // newly accept()ed socket descriptor
  struct sockaddr_storage remoteaddr; // client address
  socklen_t addrlen;

  char buf[2048]; // buffer for client data
  int nbytes;

  char remoteIP[INET6_ADDRSTRLEN];

  int yes = 1; // for setsockopt() SO_REUSEADDR, below
  int i, rv;

  struct addrinfo hints, *ai, *p;

  FD_ZERO(&master); // clear the master and temp sets
  FD_ZERO(&read_fds);

  // get us a socket and bind it
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if ((rv = getaddrinfo(NULL, argv[1], &hints, &ai)) != 0) {
    fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
    exit(1);
  }

  for (p = ai; p != NULL; p = p->ai_next) {
    listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (listener < 0) {
      continue;
    }

    // lose the pesky "address already in use" error message
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
      close(listener);
      continue;
    }

    break;
  }

  // if we got here, it means we didn't get bound
  if (p == NULL) {
    fprintf(stderr, "selectserver: failed to bind\n");
    exit(2);
  }

  freeaddrinfo(ai); // all done with this

  // listen
  if (listen(listener, 10) == -1) {
    perror("listen");
    exit(3);
  }

  // add the listener to the master set
  FD_SET(listener, &master);

  // keep track of the biggest file descriptor
  fdmax = listener; // so far, it's this one

  int clients[MAX_USERS + 3];
  int experts_lvl2_available[MAX_USERS + 3];
  int experts_lvl3_available[MAX_USERS + 3];
  int relationship[MAX_USERS + 3];

  memset(clients, 1, MAX_USERS + 3);
  memset(experts_lvl2_available, -1, MAX_USERS + 3);
  memset(experts_lvl3_available, -1, MAX_USERS + 3);

  // main loop
  for (;;) {
    read_fds = master; // copy it
    if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select");
      exit(4);
    }

    // run through the existing connections looking for data to read
    for (i = 0; i <= fdmax; i++) {
      if (FD_ISSET(i, &read_fds)) { // we got one!!
        if (i == listener) {
          // handle new connections
          addrlen = sizeof remoteaddr;
          newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

          if (newfd == -1) {
            perror("accept");
          } else {
            FD_SET(newfd, &master); // add to master set
            if (newfd > fdmax) {    // keep track of the max
              fdmax = newfd;
            }
            printf("selectserver: new connection from %s on "
                   "socket %d\n",
                   inet_ntop(remoteaddr.ss_family,
                             get_in_addr((struct sockaddr *)&remoteaddr),
                             remoteIP, INET6_ADDRSTRLEN),
                   newfd);
          }
        } else {
          // handle data from a client
          if ((nbytes = recv(i, buf, 2, 0)) <= 0) {
            // got error or connection closed by client
            if (nbytes == 0) {
              // connection closed
              printf("selectserver: socket %d hung up\n", i);
            } else {
              perror("recv");
            }
            close(i);           // bye!
            FD_CLR(i, &master); // remove from master set
          } else {
            // we got some data from a client
            u_int16_t msg_size = unpack_uint16(buf);
            memset(buf, 0, 2048);
            nbytes = recv(i, buf, 4 + 32 + msg_size, 0);
            proto_msg *unpacked_msg = unpack_msg(buf, msg_size);
            if (strcmp(unpacked_msg->msg_type, "con") == 0) {
              char *content = unpacked_msg->content;
              if (strcmp(content, "ag2") == 0) {
                experts_lvl2_available[i] = 0;
              } else if (strcmp(content, "ag3") == 0) {
                experts_lvl3_available[i] = 0;
              } else {
                clients[i] = 1;
              }
            } else if (strcmp(unpacked_msg->msg_type, "msg") == 0) {
              if (clients[i] == 1) {
                // Connected to BOT
                if (FD_ISSET(i, &master)) {
                  char *automatic_answer = "Dites m'en plus...";
                  char buff[2048];
                  pack_msg(buff, "msg", "BOT", automatic_answer);
                  if (send(i, buff, 2 + 4 + 32 + strlen(automatic_answer), 0) ==
                      -1) {
                    perror("send");
                  }
                }
              } else {
                int target_fd = relationship[i];
                if (FD_ISSET(target_fd, &master)) {
                  char sendbuf[2048];
                  pack_uint16(sendbuf, msg_size);
                  memcpy(sendbuf + 2, buf, 4 + 32 + msg_size);
                  if (send(target_fd, sendbuf, 4 + 32 + msg_size, 0) == -1) {
                    perror("send");
                  }
                }
              }
            } else if (strcmp(unpacked_msg->msg_type, "esc") == 0) {
              int expert = -1;
              if (clients[i] == 1) {
                clients[i]++;
                expert = available_expert(experts_lvl2_available);
                add_connection(relationship, i, expert);
              } else if (clients[i] == 2) {
                clients[i]++;
                expert = available_expert(experts_lvl3_available);
                update_connection(relationship, i, expert);
              }
              if (expert < 0) {
                char *no_expert = "Pas d'expert de disponibles, désolé";
                char buff[2048];
                pack_msg(buff, "msg", "BOT", no_expert);
                if (send(i, buff, 2 + 4 + 32 + strlen(no_expert), 0) == -1) {
                  perror("send");
                }
              } else {
                printf("%d\n", expert);
                char *transaction = "Client en approche !";
                char buff[2048];
                pack_msg(buff, "msg", "BOT", transaction);
                if (send(expert, buff, 2 + 4 + 32 + strlen(transaction), 0) ==
                    -1) {
                  perror("send");
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}
