#include "client.h"

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
  if (connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    perror("Erreur à la connexion");
    exit(1);
  } else {
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
  }
  while (1) {
    char buffer[1024];
    int n;
    if ((n = recv(clientSocket, buffer, sizeof(buffer), 0)) < 0) {
      perror("Erreur à la réception");
      exit(1);
    }
    buffer[n] = '\0';
    printf("%s", buffer);
    fgets(buffer, 1024, stdin);
    if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
      perror("Erreur à l'envoi");
      exit(1);
    }
  }
  close(clientSocket);
  return EXIT_SUCCESS;
}
