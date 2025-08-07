#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int server_fd, valread, addrlen, new_socket;
  struct sockaddr_in address;
  const int PORT = 8080;
  FILE *file;
  char content[29769], hello[29769], temp[100], query[100];

  // file = fopen("index.html", "r");
  // fread(content, 1, 29769, file);
  // sprintf(hello,
  //         "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
  //         "29769\n\n%s",
  //         content);
  //
  // # Creating a socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("cannot create socket");
    return 0;
  }

  // # Defining the socket address structure
  memset((char *)&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);

  addrlen = sizeof(address);

  // # Naming a socket/binding an address for the sokcet
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    return 0;
  }

  if (listen(server_fd, 3) < 0) {
    perror("In listen");
    exit(EXIT_FAILURE);
  }

  while (1) {

    printf("\n++++++++ Waiting for new connection +++++++++\n\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&(addrlen))) < 0) {
      perror("In accept");
      exit(EXIT_FAILURE);
    }

    char buffer[30000] = {0};
    valread = read(new_socket, buffer, 30000);
    printf("%s\n", buffer);

    bool add = false;
    int j = 0;
    for (int i = 0; buffer[i] != '\n'; i++) {
      if (buffer[i] == ' ') {
        add = !add;
      }
      if (add) {
        temp[j] = buffer[i];
        j++;
      }
    }

    memset(query, 0, sizeof(query));

    for (int j = 2; temp[j] != '\0'; j++) {
      query[j - 2] = temp[j];
    }

    memset(temp, 0, sizeof(temp));
    file = fopen(query, "r");
    if ((file = fopen(query, "r")) != NULL) {

      fread(content, 1, 29769, file);
      sprintf(hello,
              "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
              "29769\n\n%s",
              content);

      write(new_socket, hello, strlen(hello));
      memset(content, 0, sizeof(content));
    } else {
      printf("page not found\r\n");
    }

    printf("------------- response message sent --------------\n");
    close(new_socket);
  }

  return 0;
}
