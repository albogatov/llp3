#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <libxml/xmlreader.h>
#include <libxml/encoding.h>


#include <cstdio>
#include <iostream>

#include "include/receiver.h"
#include "include/relation.h"
#include "include/database.h"


using boost::asio::ip::tcp;
using boost::asio::awaitable;

using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

#if defined(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
# define use_awaitable \
  boost::asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

xmlSchemaPtr schema = NULL;
xmlSchemaParserCtxtPtr schema_parser_ctxt = NULL;
xmlSchemaValidCtxtPtr valid_ctxt = NULL;



awaitable<void> request(tcp::socket& socket, database* db) {

    char data[1024 * 100];

    std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);
    data[n] = 0;
    std::string str(data);
    str.resize(n);

    xmlDocPtr xml = xmlParseDoc((xmlChar*) str.c_str());
    if ( xmlSchemaValidateDoc(valid_ctxt, xml) == 0 ) {
        std::cout << str << std::endl;
    }

    auto res = receiver::request_op(str, db);
    co_await async_write(socket, boost::asio::buffer(res.c_str(), res.size()), use_awaitable);

}

awaitable<void> accept_new_client(tcp::socket socket, database* db) {
    std::cout << "Client connected" << std::endl;
    try {
        for (;;) {
            co_await request(socket, db);
        }
    } catch (std::exception& e) {
        std::printf("Exception occurred: %s\n", e.what());
    }

}

awaitable<void> listener(database* db) {

    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), 8187});

    std::cout << "listener" << std::endl;

    for (;;) {

        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor, accept_new_client(std::move(socket), db), detached);

    }
}



int main() {

    struct database* test_database = database_get("test_db.bin", NEW);

    xmlInitParser();

    if ((schema_parser_ctxt = xmlSchemaNewParserCtxt("/Users/abogatov/CLionProjects/llp3_xml/server_side/scheme.xsd")))
    {
        schema = xmlSchemaParse(schema_parser_ctxt);
        xmlSchemaFreeParserCtxt(schema_parser_ctxt);
        if (schema)
        {
            valid_ctxt = xmlSchemaNewValidCtxt(schema);
        }
    }

    try {

        boost::asio::io_context io_context(1);

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto){ io_context.stop(); });

        co_spawn(io_context, listener(test_database), detached);

        io_context.run();

        database_close(test_database);

    } catch (std::exception& e) {
        std::printf("Exception: %s\n", e.what());
    }
}
