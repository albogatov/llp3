#include "main.h"

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/ast.h"
#include "parser.tab.h"
#include "include/string_utils_xml.h"

int main(int argc, char* argv[]) {

    if (argc < 1) {
        std::cout << "You must enter server port" << std::endl;
        return -1;
    }

    int port = atoi(argv[0]);

    ast_node* this_root;

    std::string inp;

    int32_t sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof (addr)) < 0) {
        std::cout << "connect error" << std::endl;
        return -1;
    }

    char buf[1024 * 100];

    while (true) {
        printf("$> ");
        this_root = yyparse();

        auto xml_string = ast_to_xml(this_root);

        char* str = xml_string.data();
        std::cout << str << std::endl;

        send(sock, str, xml_string.size(), 0);
        auto response_size = recv(sock, buf, 1024 * 100, 0);

        if (response_size == -1) {
            return -1;
        }

        buf[response_size] = 0;

        std::cout << buf << std::endl;

    }
    return 0;
}