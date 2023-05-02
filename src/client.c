#include "client.h"

int client() {
  int clientSocket;
  if ((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("error opening socket");
    exit(1);
  }
  struct sockaddr_in serv_addr;
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  serv_addr.sin_port = htons(SERV_PORT);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    perror("Erreur à la connexion");
    exit(1);
  } else {
    char username[32];
    int k;
    printf("Entrez un nom d'utilisateur : ");
    if ((k = recv(clientSocket, username, sizeof(username), 0)) < 0) {
      perror("Pas de nom d'utilisateur saisi");
      exit(1); // Force la sortie pour le moment, peut-être à modifier plus tard
    }
    username[k] = '\0';
    fgets(username, 32, stdin);
    if (send(clientSocket, username, strlen(username), 0) < 0) {
      perror("Erreur à l'envoi du nom d'utilisateur");
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
}
