#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

#pragma comment(lib, "WS2_32")

int main() {
    srand(time(NULL));
    SkipList<std::string, std::string> skiplist(6);
    std::string key, value;
    std::string res;
    int command;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockServer == INVALID_SOCKET) {
        printf("socket error\n");
        return 0;
    }

    SOCKADDR_IN addrServer;
    addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(3456);

    if (SOCKET_ERROR == bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR_IN))) {
        printf("bind error\n");
        return -1;
    }

    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR_IN);
    char szRecvBuffer[1024] = "";

    while (true) {
        int nRecv = recvfrom(sockServer, szRecvBuffer, 1024, 0, (SOCKADDR*)&addrClient, &len);
        if (nRecv > 0) {
            // szRecvBuffer[nRecv] = '\0';
            printf("���յ�������(%s): %s\n", inet_ntoa(addrClient.sin_addr), szRecvBuffer);
            
            // ���ͻظ���Ϣ���ͻ���
            char szSendBuffer[1024] = "";
            // printf("������Ҫ���͵�����: ");
            // gets(szReply);
            // sendto(sockServer, szReply, strlen(szReply) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
            command = atoi(szRecvBuffer);
            switch (command) {
                case 0:
                    // std::cout << "Size: " << skiplist.size() << std::endl;
                    strcpy(szSendBuffer, ("Size: " + std::to_string(skiplist.size())).c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 1:
                    // std::cout << "Enter key: ";
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter key: ");
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    // std::cin >> key;
                    // recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                    recvfrom(sockServer, szRecvBuffer, 1024, 0, (SOCKADDR*)&addrClient, &len);
                    printf("���յ��ͻ�����Ϣ��%s\n",szRecvBuffer);
                    key = std::string(szRecvBuffer);
                    // std::cout << "Enter value: ";
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter value: ");
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    // std::cin >> value;
                    // recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                    recvfrom(sockServer, szRecvBuffer, 1024, 0, (SOCKADDR*)&addrClient, &len);
                    printf("���յ��ͻ�����Ϣ��%s\n",szRecvBuffer);
                    value = std::string(szRecvBuffer);
                    res = skiplist.insert_element(key, value);
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 2:
                    // std::cout << "Enter key: ";
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter key: ");
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    // std::cin >> key;
                    // recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                    recvfrom(sockServer, szRecvBuffer, 1024, 0, (SOCKADDR*)&addrClient, &len);
                    printf("���յ��ͻ�����Ϣ��%s\n",szRecvBuffer);
                    key = std::string(szRecvBuffer);
                    res = skiplist.search_element(key);
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 3:
                    // std::cout << "Enter key: ";
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter key: ");
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    // std::cin >> key;
                    // recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                    recvfrom(sockServer, szRecvBuffer, 1024, 0, (SOCKADDR*)&addrClient, &len);
                    printf("���յ��ͻ�����Ϣ��%s\n",szRecvBuffer);
                    key = std::string(szRecvBuffer);
                    res = skiplist.delete_element(key);
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 4:
                    std::cout << "Enter key: ";
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter key: ");
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    // std::cin >> key;
                    // recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                    recvfrom(sockServer, szRecvBuffer, 1024, 0, (SOCKADDR*)&addrClient, &len);
                    printf("���յ��ͻ�����Ϣ��%s\n",szRecvBuffer);
                    key = std::string(szRecvBuffer);
                    std::cout << "Enter new value: " << std::endl;
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter new value: ");
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    // std::cin >> value;
                    // recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                    recvfrom(sockServer, szRecvBuffer, 1024, 0, (SOCKADDR*)&addrClient, &len);
                    printf("���յ��ͻ�����Ϣ��%s\n",szRecvBuffer);
                    value = std::string(szRecvBuffer);
                    std::cout << "Insert if not exists? (1 for yes, 0 for no): " << std::endl;
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Insert if not exists? (1 for yes, 0 for no): ");
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    bool flag;
                    // std::cin >> flag;
                    // recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                    recvfrom(sockServer, szRecvBuffer, 1024, 0, (SOCKADDR*)&addrClient, &len);
                    printf("���յ��ͻ�����Ϣ��%s\n",szRecvBuffer);
                    flag = atoi(szRecvBuffer);
                    res = skiplist.update_element(key, value, flag);
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 5:
                    res = skiplist.display_list();
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 6:
                    res = skiplist.dump_file();
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 7:
                    res = skiplist.load_file();
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 8:
                    res = skiplist.clear();
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                case 9:
                    strcpy(szSendBuffer, "-----------------SkipList Test-----------------\nEnter command: (1) insert, (2) search,  (3) delete \n               (4) update, (5) display, (6) dump \n               (7) load,   (8) clear,   (9) help: \n                           (0) size               \n");
                    // send(sockClient , szSendBuffer , strlen(szSendBuffer)+sizeof(char) , 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
                default:
                    std::cout << "Unknown command. Please try again." << std::endl;
                    res = "Unknown command. Please try again.";
                    strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                    // send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                    sendto(sockServer, szSendBuffer, strlen(szSendBuffer) + 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR_IN));
                    break;
            }
        }
    }

    closesocket(sockServer);
    WSACleanup();

    return 0;
}
/*
    ��ʼ��Winsock�⣺����WSAStartup��������ʼ��Winsock�⣬�Ա�ʹ�����繦�ܡ�
    ����Socket��ʹ��socket��������һ��UDP������Socket��
    ��Socket������SOCKADDR_IN�ṹ��addrServer��������󶨵���������IP��ַ�Ͷ˿��ϡ�����ʹ��INADDR_ANY��ʾ���������Խ�����������ӿ��ϵ����ݡ�
    ���պͷ������ݣ�
    ʹ��recvfrom�������տͻ��˷��͵���Ϣ������ӡ��Ϣ���ݺͿͻ��˵�IP��ַ��
    ʹ��sendto�������ͻظ���Ϣ���ͻ��ˣ�ȷ�Ͻ��յ���Ϣ��
    �ر�Socket�������ڳ������ǰ���ر�Socket������WSACleanup��������Winsock�⡣
    ���UDP�����������ϵؽ��տͻ��˷��͵���Ϣ�������ͻظ���Ϣȷ�Ͻ��յ��ͻ��˵���Ϣ��
*/