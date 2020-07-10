#include "tcpclient.h"
#include <iostream>
#include "lwip/sockets.h"
#include <string>
#include <list>

void TcpClient::Send()
{
    std::string name = "I'm ESP32";
    char tx_buffer[(name.length())];
    strcpy(tx_buffer, name.c_str());
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr("192.168.1.2");//ip.s_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(61424);
    int sock =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    std::cout << "TCP loc socket"<<sock<<"\n";
    int err1 = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
    if (err1 != 0) {
        std::cout << "TCP socket unable to connect "<<err1<<"\n";
        close(sock); 
        return;
    }  else  std::cout << "TCP socket connected from port :"<<ntohs(dest_addr.sin_port)<<"\n";
    
    int err2 = send(sock, &tx_buffer, sizeof(tx_buffer), 0);
    if (err2 < 0) {
        std::cout << "TCP socket error occurred during sending\n";
        return;       
    } /**/ 
    
    if (sock != -1) {
            shutdown(sock, 0);
            close(sock);
        }

}
list<char> TcpClient::ProcessMessage(char *buffer,struct in_addr ip)
{
    list<char> ret;
    
    return ret;
    /*uint8_t messageType = *(buffer);
    int16_t messageSize = (*(buffer+1)<<8 ) + (*(buffer+2) );
    uint16_t port = (*(buffer+3)<<8 ) + (*(buffer+4) );
    std::cout << "Received "<< messageType<<" from ip: "<<inet_ntoa(ip)<<" port:"<< port<<" size:"<< messageSize <<"\n";
    
    std::string name = "I'm ESP32";
    char tx_buffer[(name.length())];
    strcpy(tx_buffer, name.c_str());
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr("192.168.1.15");//ip.s_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(51633);

    sock =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    std::cout << "TCP loc socket"<<sock<<"\n";
    int err1 = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
    if (err1 != 0) {
        std::cout << "TCP socket unable to connect "<<err1<<"\n";
        close(sock); 
        return;
    }  else  std::cout << "TCP socket connected\n";
    
    int err2 = send(sock, &tx_buffer, sizeof(tx_buffer), 0);
    if (err2 < 0) {
        std::cout << "TCP socket error occurred during sending\n";
        return;       
    }  
    
    if (sock != -1) {
            shutdown(sock, 0);
            close(sock);
        }
     */

}

TcpClient::TcpClient(WiFi *wifi)
{
    /*
    wifi->WaitForConnection();
    sock =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        std::cout << "Unable to create TCP socket\n";
        return;
    }
    */
    
}

TcpClient::~TcpClient()
{
    shutdown(sock, 0);   
    close(sock);
}