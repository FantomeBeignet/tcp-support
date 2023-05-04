#include "client.h"

void flush_stdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage : ./client <server_adress> <server_port>\n");
    exit(1);
  }
  char *serv_adress = argv[1];
  char *end;
  unsigned long serv_port = strtoul(argv[2], &end, 10);
  if (strcmp(end, "\0")) {
    fprintf(stderr, "%s is not a valid port number", argv[2]);
    exit(1);
  }
  if (serv_port > 65535) {
    fprintf(stderr, "%lu is not a valid port number", serv_port);
    exit(1);
  }
  int clientSocket;
  if ((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("error opening socket");
    exit(1);
  }
  struct sockaddr_in serv_addr;
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  serv_addr.sin_port = htons(serv_port);
  serv_addr.sin_addr.s_addr = inet_addr(serv_adress);
  char username[32];
  char role[4];
  if (connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    perror("Erreur à la connexion");
    exit(1);
  }
  printf("Entrez un nom d'utilisateur : ");
  if (!fgets(username, 32, stdin)) {
    perror("Veuillez rentrer un nom d'utilisateur.");
    exit(1);
  }
  username[strcspn(username, "\r\n")] = 0;
  if (!strcmp(username, "\0")) {
    perror("Veuillez rentrer un nom d'utilisateur.");
    exit(1);
  }
  printf("Se connecter en temps que :\n1: Client\n2: Agent de niveau 2\n3: "
         "Agent de niveau 3\n");
  int role_num = getchar() - 48;
  switch (role_num) {
  case 1:
    strcpy(role, "cli");
    break;
  case 2:
    strcpy(role, "ag2");
    break;
  case 3:
    strcpy(role, "ag3");
    break;
  default:
    fprintf(stderr, "%d n'est pas un numéro de rôle valide\n", role_num);
    exit(1);
  }
  char mk_buffer[1024], send_buffer[2048], recv_buffer[2048];
  flush_stdin();
  memset(send_buffer, 0, 2048);
  pack_msg(send_buffer, "con", username, role);
  if (send(clientSocket, send_buffer, 2 + 4 + 32 + 3, 0) < 0) {
    perror("Erreur à l'envoi");
    exit(1);
  }
  if (role_num == 1) {
    printf("Message : ");
    fgets(mk_buffer, 1024, stdin);
    memset(send_buffer, 0, 2048);
    pack_msg(send_buffer, "msg", username, mk_buffer);
    if (send(clientSocket, send_buffer, 2 + 4 + 32 + strlen(mk_buffer), 0) <
        0) {
      perror("Erreur à l'envoi");
      exit(1);
    }
  }
  while (1) {
    uint16_t msg_size;
    int n;
    if ((n = recv(clientSocket, recv_buffer, 2, 0)) < 0) {
      perror("Erreur à la réception");
      exit(1);
    }
    msg_size = unpack_uint16(recv_buffer);
    if ((n = recv(clientSocket, recv_buffer, 4 + 32 + msg_size, 0)) < 0) {
      perror("Erreur à la réception");
      exit(1);
    }
    proto_msg *unpacked_msg = unpack_msg(recv_buffer, msg_size);
    memset(recv_buffer, 0, 2048);
    printf("%s: %s\n", unpacked_msg->sender_uname, unpacked_msg->content);
    printf("Message : ");
    memset(mk_buffer, 0, 1024);
    fgets(mk_buffer, 1024, stdin);
    mk_buffer[strcspn(mk_buffer, "\r\n")] = 0;
    if (!strcmp(mk_buffer, "escalate"))
      pack_msg(send_buffer, "esc", username, mk_buffer);
    else
      pack_msg(send_buffer, "msg", username, mk_buffer);
    if (send(clientSocket, send_buffer, 2 + 4 + 32 + strlen(mk_buffer), 0) <
        0) {
      perror("Erreur à l'envoi");
      exit(1);
    }
  }
  close(clientSocket);
  return EXIT_SUCCESS;
}
