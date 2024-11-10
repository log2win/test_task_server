#include <array>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/describe.hpp>
#include <boost/endian.hpp>
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/mp11.hpp>
#include <boost/version.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_set>

#include "settings.hpp"
#include "text_processing.hpp"
#include "util.hpp"

using boost::asio::ip::tcp;

int64_t readFileSize(tcp::socket &socket, boost::system::error_code &error) {
    std::array<unsigned char, sizeof(int64_t)> buf;
    boost::asio::read(socket, boost::asio::buffer(buf),
                      boost::asio::transfer_exactly(sizeof(int64_t)), error);
    int64_t size =
        boost::endian::endian_load<int64_t, sizeof(int64_t),
                                   boost::endian::order::little>(buf.data());
    std::cout << "expected file size: " << size << std::endl;
    return size;
}

std::map<std::string, int64_t> readAndProcessFile(
    const int64_t fileSize, tcp::socket &socket,
    boost::system::error_code &error) {
    std::array<unsigned char, BUFFER_SIZE> buf;
    WordsBufferedReader wordsBuffer;
    WordsStatistics wordsStatistics;

    int64_t total_read = 0;
    while (total_read < fileSize) {
        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        std::string readString = std::string(buf.begin(), buf.begin() + len);
        std::cout << "received data size: " << readString.size() << std::endl;
        auto words = wordsBuffer.readWords(readString);
        std::cout << "count of words in data: " << words.size() << std::endl;
        for (auto word : words) {
            wordsStatistics.addWord(std::move(word));
        }
        total_read += len;
    }

    std::cout << "unique words sequence: "
              << wordsStatistics.getUniqueWordsSequence() << std::endl;
    std::map<std::string, int64_t> result;
    result["unique words count"] = wordsStatistics.getUniqueWords().size();
    result["words count"] = wordsStatistics.getWordsCount();
    result["unique words sequence size"] =
        wordsStatistics.getUniqueWordsSequence().size();
    return result;
}

void processClient(tcp::socket socket) {
    boost::system::error_code error;

    auto fileSize = readFileSize(socket, error);
    auto statistics = readAndProcessFile(fileSize, socket, error);
    auto statisticsJson = boost::json::value_from(statistics);

    std::string statisticsStr =
        boost::lexical_cast<std::string>(statisticsJson);
    statisticsStr.push_back('\0');
    std::cout << statisticsStr << std::endl;

    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(statisticsStr),
                       ignored_error);
}

int main(int argc, char *argv[]) {
    boost::asio::thread_pool pool(NUMBER_OF_THREADS);
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context,
                               tcp::endpoint(tcp::v4(), SERVER_PORT));

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            // acknowledge that async functionality would be better,
            // but I want to keep it simple
            boost::asio::post(pool, [socket = std::move(socket)]() mutable {
                processClient(std::move(socket));
            });
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "finishing tasks..." << std::endl;
    pool.join();
    return 0;
}
