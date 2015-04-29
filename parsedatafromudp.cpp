#include "parsedatafromudp.h"



ParseDataFromUDP::ParseDataFromUDP(boost::asio::io_service& io_service)
        : socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 13000)) {
    start_receive();
}


void ParseDataFromUDP::start_receive() {
    socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&ParseDataFromUDP::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}


void ParseDataFromUDP::handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred) {
    if (!error || error == boost::asio::error::message_size)
    {

        std::cout << "packet acquaired: " << std::endl;

        start_receive();
    }
}
