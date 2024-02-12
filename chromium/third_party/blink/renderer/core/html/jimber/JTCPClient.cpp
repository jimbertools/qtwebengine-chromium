
#include "JTCPClient.h"
#include <numeric>
#include <sstream>
#include <thread>
// #include "WebProcess.h"

JTCPClient::JTCPClient() {
  sock = -1;
  port = 0;
  address = "";
}

bool JTCPClient::setup(string address, int port) {
  if (sock == -1) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
      cout << "Could not create socket" << endl;
    }
  }
  if (inet_addr(address.c_str()) == -1) {
    struct hostent* he;
    struct in_addr** addr_list;
    if ((he = gethostbyname(address.c_str())) == NULL) {
      herror("gethostbyname");
      cout << "Failed to resolve hostname\n";
      return false;
    }
    addr_list = (struct in_addr**)he->h_addr_list;
    for (int i = 0; addr_list[i] != NULL; i++) {
      server.sin_addr = *addr_list[i];
      break;
    }
  } else {
    server.sin_addr.s_addr = inet_addr(address.c_str());
  }
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return false;
  }

  std::thread t1(listenForMessage, std::ref(sock), 20);
  t1.detach();

  return true;
}

bool JTCPClient::Send(string data) {
  // static int counter = 0;
  // counter++;
  // if(counter % 1000 == 0) {
  //     std::cout << counter << std::endl;
  // }
  if (sock != -1) {
    if (send(sock, data.c_str(), strlen(data.c_str()), 0) < 0) {
      cout << "Send failed : " << data << endl;
      return false;
    }
  } else
    return false;
  return true;
}

bool JTCPClient::sendBinary(const char* ptr, int size) {
  if (sock != -1) {
    if (send(sock, ptr, size, 0) < 0) {
      cout << "Binary message send failed. " << endl;
      return false;
    }
  } else
    return false;
  return true;
}

string JTCPClient::receive(int size) {
  char buffer[size];
  memset(&buffer[0], 0, sizeof(buffer));

  string reply;
  if (recv(sock, buffer, size, 0) < 0) {
    cout << "receive failed!" << endl;
    return nullptr;
  }
  buffer[size - 1] = '\0';
  reply = buffer;
  return reply;
}

string JTCPClient::read() {
  char buffer[1] = {};
  string reply;
  while (buffer[0] != '\n') {
    if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
      cout << "receive failed!" << endl;
      return nullptr;
    }
    reply += buffer[0];
  }
  return reply;
}

void JTCPClient::exit() {
  close(sock);
}

/*void TCPClient::addToMQ()
{

    auto toet = TCPClient::instance().receive(1);

    RunLoop::main().dispatch(
        []() {
            TCPClient::instance().addToMQ();
        });
}*/

void JTCPClient::listenForMessage(int& sock, int size) {
  size = 4096;

  while (true) {
    char buffer[size];
    memset(&buffer[0], 0, sizeof(buffer));

    string reply;
    if (recv(sock, buffer, size, 0) < 0) {
      std::cout << "receive failed!" << std::endl;
    }
    buffer[size - 1] = '\0';
    reply = buffer;

    std::vector<std::string> command;
    std::istringstream f(reply);
    string s;
    while (getline(f, s, ';')) {
      command.push_back(s);
    }
  }
}
