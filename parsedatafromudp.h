#ifndef PARSEDATAFROMUDP_H
#define PARSEDATAFROMUDP_H



#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>



/*
 * Data format:
 *
 * 's'+'n'+'p'+(length)+(D_1)+(D_2)+...+(D_length)
 *
 * server work on port 13000
 *
*/


struct DataFromExternalSource {
    size_t numberOfMeasurements_;
    float time_;
    float elements_[9];
};


class ParseDataFromUDP {

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    boost::array<char, maxLength> recv_buffer_;
    static uint8_t const maxLength = 99;


    void start_receive();

    void handle_receive(const boost::system::error_code& error,
                        std::size_t bytes_transferred);

    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code& /*error*/,
                     std::size_t /*bytes_transferred*/)
    {}

public:

    ParseDataFromUDP(boost::asio::io_service& io_service);

    struct Packet {
        uint8_t dataLength;
        uint8_t data[maxLength];
    };

};

#endif // PARSEDATAFROMUDP_H
