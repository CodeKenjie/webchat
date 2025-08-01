#include <iostream>
#include <string>
#include <thread>
#include <memory>
// boost beast include
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace websocket = boost::beast::websocket;
using tcp = boost::asio::ip::tcp;
 
void handler_t(tcp::socket socket){
    try{

        websocket::stream<tcp::socket> ws(std::move(socket));

        ws.accept();

        boost::beast::flat_buffer buffer;
        
        while (true) {    

            ws.read(buffer);
            std::cout << "Received: " << boost::beast::make_printable(buffer.data()) << std::endl;

            ws.text(ws.got_text());
            ws.write(buffer.data());

            buffer.consume(buffer.size());
        }            
    } catch (boost::beast::system_error const& se) {
        if (se.code() == websocket::error::closed){
            std::cout << "Client disconnected" << std::endl;
        } else {
            std::cout << "Websocket error" << se.code().message() << std::endl;
        }
    }

}


int main() {
    try {
        auto const address = boost::asio::ip::make_address("127.0.0.1");
        auto const port = static_cast<unsigned short>(8080);

        boost::asio::io_context ioc;

        tcp::endpoint endpoint(address, port);
        tcp::acceptor acceptor(ioc, endpoint);
        
        tcp::socket socket(ioc);

        std::cout << "Waiting for a client to connect on port 8080....." << std::endl;
        acceptor.accept(socket);

        websocket::stream<tcp::socket> ws(std::move(socket));

        std::thread (handler_t, std::move(socket)).detach();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    return 0;
}