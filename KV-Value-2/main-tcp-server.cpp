#include "stdfax.h"
#pragma comment(lib , "WS2_32")
#include <windows.h>
#include <iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

DWORD WINAPI HandlerThread(LPVOID lpParameter){
    SOCKET sockClient = (SOCKET)lpParameter;
    char szRecvBuffer[MAXBYTE] = {0};
    char szSendBuffer[MAXBYTE] = {0};
    int iLen = 0;
    srand(time(NULL));
    SkipList<std::string, std::string> skiplist(6);
    std::string key, value;
    std::string res;
    int command;
    // strcpy(szSendBuffer, "-----------------SkipList Test-----------------\nEnter command: (1) insert, (2) search,  (3) delete \n               (4) update, (5) display, (6) dump \n               (7) load,   (8) clear,   (9) exit: \n                           (0) size               \n");
    // send(sockClient , szSendBuffer , strlen(szSendBuffer)+sizeof(char) , 0);

    while(true){
        // strcpy(szSendBuffer, "-----------------SkipList Test-----------------\nEnter command: (1) insert, (2) search,  (3) delete \n               (4) update, (5) display, (6) dump \n               (7) load,   (8) clear,   (9) exit: \n                           (0) size               \n");
        // // gets(szSendBuffer);
        // send(sockClient , szSendBuffer , strlen(szSendBuffer)+sizeof(char) , 0);
        iLen = recv(sockClient , szRecvBuffer , MAXBYTE , 0);
        if(iLen <= 0)
            break;
        printf("接收到客户端消息：%s\n",szRecvBuffer);
        // printf("请输入要发送给客户端的消息：");
        // std::cout << "-----------------SkipList Test-----------------" << std::endl;
        // std::cout << "Enter command: (1) insert, (2) search,  (3) delete " << std::endl;
        // std::cout << "               (4) update, (5) display, (6) dump "   << std::endl;
        // std::cout << "               (7) load,   (8) clear,   (9) help: "  << std::endl;
        // std::cout << "                           (0) size               "  << std::endl;
        // std::cout << "-----------------------------------------------" << std::endl;
        command = atoi(szRecvBuffer);
        switch (command) {
            case 0:
                // std::cout << "Size: " << skiplist.size() << std::endl;
                strcpy(szSendBuffer, ("Size: " + std::to_string(skiplist.size())).c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 1:
                // std::cout << "Enter key: ";
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter key: ");
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                // std::cin >> key;
                recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                printf("接收到客户端消息：%s\n",szRecvBuffer);
                key = std::string(szRecvBuffer);
                // std::cout << "Enter value: ";
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter value: ");
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                // std::cin >> value;
                recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                printf("接收到客户端消息：%s\n",szRecvBuffer);
                value = std::string(szRecvBuffer);
                res = skiplist.insert_element(key, value);
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 2:
                // std::cout << "Enter key: ";
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter key: ");
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                // std::cin >> key;
                recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                printf("接收到客户端消息：%s\n",szRecvBuffer);
                key = std::string(szRecvBuffer);
                res = skiplist.search_element(key);
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 3:
                // std::cout << "Enter key: ";
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter key: ");
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                // std::cin >> key;
                recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                printf("接收到客户端消息：%s\n",szRecvBuffer);
                key = std::string(szRecvBuffer);
                res = skiplist.delete_element(key);
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 4:
                std::cout << "Enter key: ";
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter key: ");
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                // std::cin >> key;
                recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                printf("接收到客户端消息：%s\n",szRecvBuffer);
                key = std::string(szRecvBuffer);
                std::cout << "Enter new value: " << std::endl;
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Enter new value: ");
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                // std::cin >> value;
                recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                printf("接收到客户端消息：%s\n",szRecvBuffer);
                value = std::string(szRecvBuffer);
                std::cout << "Insert if not exists? (1 for yes, 0 for no): " << std::endl;
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), "Insert if not exists? (1 for yes, 0 for no): ");
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                bool flag;
                // std::cin >> flag;
                recv(sockClient , szRecvBuffer , MAXBYTE , 0);
                printf("接收到客户端消息：%s\n",szRecvBuffer);
                flag = atoi(szRecvBuffer);
                res = skiplist.update_element(key, value, flag);
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 5:
                res = skiplist.display_list();
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 6:
                res = skiplist.dump_file();
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 7:
                res = skiplist.load_file();
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 8:
                res = skiplist.clear();
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
            case 9:
                strcpy(szSendBuffer, "-----------------SkipList Test-----------------\nEnter command: (1) insert, (2) search,  (3) delete \n               (4) update, (5) display, (6) dump \n               (7) load,   (8) clear,   (9) help: \n                           (0) size               \n");
                send(sockClient , szSendBuffer , strlen(szSendBuffer)+sizeof(char) , 0);
                break;
            default:
                std::cout << "Unknown command. Please try again." << std::endl;
                res = "Unknown command. Please try again.";
                strcpy_s(szSendBuffer, sizeof(szSendBuffer), res.c_str());
                send(sockClient, szSendBuffer, strlen(szSendBuffer)+sizeof(char), 0);
                break;
        }
    }
    closesocket(sockClient);
    return 0;
}

int main(){
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2),&wsaData);
    SOCKET sockSrv = socket(AF_INET , SOCK_STREAM , 0);

    sockaddr_in addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(5000);
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    bind(sockSrv , (SOCKADDR*)&addrSrv , sizeof(SOCKADDR));

    listen(sockSrv , 5);

    SOCKADDR newAddr;
    int len = sizeof(SOCKADDR);

    while(true){
        SOCKET newClient;
        newClient = accept(sockSrv , &newAddr , &len);
        printf("新连接客户端： %s\n",inet_ntoa(((struct sockaddr_in*)&newAddr)->sin_addr));
        CloseHandle(CreateThread(NULL , 0 , HandlerThread , (LPVOID)newClient , 0 , NULL));
    }
    closesocket(sockSrv);
    WSACleanup();

    return 0;
}