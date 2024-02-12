#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

class JTCPClient
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
    bool enabled = false;

public:
    JTCPClient();
    bool setup(string address, int port);
    bool Send(string data);
    bool sendBinary(const char* ptr, int size);
    string receive(int size = 4096);
    string read();
    void exit();
    //   void addToMQ();
    static void listenForMessage(int &sock, int size);
    //  void setCurrentWebPage(WebKit::WebPage* page);
    // WebKit::WebPage* _currentWebPage;

    static JTCPClient &instance()
    {
        static JTCPClient tcpclient;
        if (!tcpclient.enabled)
        {
            tcpclient.setup("localhost", 6662);
            tcpclient.enabled = true;
        }
        return tcpclient;
    }
};
#endif