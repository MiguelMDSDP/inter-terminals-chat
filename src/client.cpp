/* ===== IMPORTAÇÕES ===== */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
/* ======================= */





/* ===== DEFINIÇÃO DA CLASSE CLIENTE ===== */
class Client {
    public:
        Client(char *nickName) {
            strcpy(_nickName, nickName);
            _host = gethostbyname("127.0.0.1");
            if (_host == NULL) {
                fprintf(stderr, "[ERRO]: Servidor host não encontrado!\n");
                exit(0);
            }

            _socket = createSocket();
            if (_socket < 0) {
                printf("[ERRO]: Houve um erro ao criar o socket!\n");
                exit(0);
            }
        }

        ~Client() {
            strcpy(_buffer, "sairAgora\n");
            sendMessage();
            close(_socket);
            printf("[INFO]: Desconectado com sucesso!\n");
        }

        int createSocket() {
            return socket(AF_INET, SOCK_STREAM, 0);
        }

        void connectToPort(int portNumber) {
            bzero((char *) &_serverAddress, sizeof(_serverAddress));
            _serverAddress.sin_family = AF_INET;
            bcopy((char *)_host->h_addr, (char *)&_serverAddress.sin_addr.s_addr, _host->h_length);
            _serverAddress.sin_port = htons(portNumber);
            if (connect(_socket, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress)) < 0) {
                printf("[ERRO]: Houve um erro na tentativa de conexão na porta %d!\n", portNumber);
                exit(0);
            } else {
                _portNumber = portNumber;
                printf("[INFO]: Conectado ao servidor 127.0.0.1:%d!\n", _portNumber);
                noticeNickName();
            }
        }

        void run() {
            printf("[INFO]: Para fechar o chat basta digitar sairAgora\n\n");
            while (true) {
                fflush(stdin);
                bzero(_buffer, 256);
                std::cout << "[Digite sua mensagem]: ";
                fgets(_buffer, 256, stdin);
                if (strcmp(_buffer, "sairAgora\n") == 0)
                    break;
                if (sendMessage() < 0) 
                    printf("[ERRO]: Ocorreu um erro ao enviar a última mensagem!");
            }
        }

        int sendMessage() {
            return write(_socket, _buffer, strlen(_buffer));
        }

        int noticeNickName() {
            return write(_socket, _nickName, strlen(_nickName));
        }

    private:
        int _socket;
        int _portNumber;
        char _buffer[256];
        char _nickName[50];
        sockaddr_in _serverAddress;
        sockaddr_in _clientAddress;
        socklen_t _clientLength;
        hostent *_host;
};
/* ======================================= */





/* ===== FUNÇÃO PRINCIPAL ===== */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "[ERRO]: Você deve informar a porta para conexão e o seu apelido para identificação!\n");
        exit(1);
    }
    int portNumber = atoi(argv[1]);
    char nickName[50];
    strcpy(nickName, argv[2]);

    Client client{nickName};
    client.connectToPort(portNumber);
    client.run();

    return 0;
}
/* ============================ */