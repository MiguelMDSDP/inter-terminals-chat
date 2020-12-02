/* ===== IMPORTAÇÕES ===== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
/* ======================= */





/* ===== DEFINIÇÃO DA CLASSE DA CONEXÃO ===== */
class Connection {
    public:
        Connection(int portNumber) {
            _socket = createSocket();
            if (_socket < 0)
                printf("[ERRO]: Houve um erro ao criar o socket!\n");
            else {
                bzero((char *) &_serverAddress, sizeof(_serverAddress));
                if (configPort(portNumber) < 0) 
                    printf("[ERRO]: Houve um erro na configuração da porta %d!\n", portNumber);
                else {
                    _portNumber = portNumber;
                    printf("[INFO]: Porta disponível para conexão: %d!\n", _portNumber);
                }
            }
        }

        ~Connection() {
            close(_socket);
            printf("[DESCONEXAO]: %s saiu!\n", _nickName);
            pthread_exit();
        }

        int createSocket() {
            return socket(AF_INET, SOCK_STREAM, 0);
        }

        int configPort(int portNumber) {
            _serverAddress.sin_family = AF_INET;  
            _serverAddress.sin_addr.s_addr = INADDR_ANY;
            _serverAddress.sin_port = htons(portNumber);
            return bind(_socket, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress));
        }

        void run() {
            listen(_socket, 5);
            _clientLength = sizeof(_clientAddress);
            _socket = accept(_socket, (struct sockaddr *) &_clientAddress, &_clientLength);

            readNickName();
            fflush(stdin);

            while (true) {
                bzero(_buffer, 256);
                int n = read(_socket, _buffer, 255);
                if (n < 0) printf("[ERRO]: Erro no recebimento da última mensagem!");
                printf("[%s diz]: %s", _nickName, _buffer);
                if (strcmp(_buffer, "sairAgora\n") == 0)
                    break;
            }
        }

        void readNickName() {
            bzero(_nickName, 50);
            int n = read(_socket, _nickName, 49);
            if (n < 0) printf("[ERRO]: Erro no recebimento do apelido!");
            printf("[NOVO USUÁRIO CONECTADO]: %s\n", _nickName);
        }

    private:
        int _socket;
        int _portNumber;
        char _buffer[256];
        char _nickName[50];
        sockaddr_in _serverAddress;
        sockaddr_in _clientAddress;
        socklen_t _clientLength;
};
/* ====================================== */





void *runConn(void *connection) {
    ((Connection *)connection)->run();
}




/* ===== DEFINIÇÃO DA CLASSE SERVIDOR ===== */
class Server {
    public:
        Server(int numberOfConnections) {
            _numberOfConnections = numberOfConnections;

            int firstPort = 20000;
            for (int i = 0; i < _numberOfConnections; i++)
                connections.push_back(new Connection{firstPort + i});
        }

        ~Server() {
            for (int i = 0; i < _numberOfConnections; i++)
                delete connections[i];

            printf("[INFO]: Servidor encerrado!\n");
        }

        void init() {
            for (int i = 0; i < _numberOfConnections; i++) {
                pthread_t connectionThread;
                pthread_create(&connectionThread, NULL, runConn, (void *)connections[i]);
                threads.push_back(connectionThread);
            }
        }

        void run() {
            for (int i = 0; i < _numberOfConnections; i++)
                pthread_join(threads[i], NULL);
        }


    private:
        int _numberOfConnections;
        std::vector<Connection *> connections;
        std::vector<pthread_t> threads;

};
/* ======================================== */





/* ===== FUNÇÃO PRINCIPAL ===== */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[ERRO]: Você deve informar o número de conexões suportadas pelo servidor!\n");
        exit(1);
    }
    int numberOfConnections = atoi(argv[1]);

    Server server{numberOfConnections};
    server.init();
    server.run();

    return 0;
}
/* ============================ */