#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

namespace beast = boost::beast;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main() {
    try {
        // creat host port and ioc
        std::string host = "127.0.0.1";
        std::string port = "8080";

        net::io_context ioc;

        // bind them together
        tcp::resolver resolver(ioc);
        auto endpoint = resolver.resolver(host, port);

        // creat a socket stream
        websocket::stream<tcp::socket> ws(ioc);
        net::connect(ws.next_layer(), endpoint.begin(), endpoint());

        // perform a handshake
        ws.handshake(host, "/");

        
    } catch (const exception& e) {
        std::cerr << "Error" << e.what() << std::endl;
    }

    return 0;
}