#include "../inc/parser.hpp"

#include <cstddef>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

Parser::Parser(const std::string& message_file_path, size_t levels) :
    levels_(levels) {

    message_stream_.open(message_file_path);
    if(!message_stream_.is_open()) {
        throw std::runtime_error("Failed to open message file at path: " + message_file_path);
    }
}

bool Parser::is_message_file_open() const {
    return message_stream_.is_open();
}

std::optional<Message> Parser::read_message() {
    std::string line;
    if(!std::getline(message_stream_, line)) {
        return std::nullopt;
    }

    std::istringstream ss(line);
    std::string token;
    Message message;
    
    if(!std::getline(ss, token, ',')) return std::nullopt;
    message.timestamp = std::stod(token);

    if(!std::getline(ss, token, ',')) return std::nullopt;
    message.message_type = std::stoi(token);

    if(!std::getline(ss, token, ',')) return std::nullopt;
    message.order_id = std::stoull(token);

    if(!std::getline(ss, token, ',')) return std::nullopt;
    message.size = std::stoull(token);

    if(!std::getline(ss, token, ',')) return std::nullopt;
    message.raw_price = std::stoull(token);

    if(!std::getline(ss, token, ',')) return std::nullopt;
    message.side = std::stoi(token);

    if(!std::getline(ss, token, ',')) return std::nullopt;
    message.entity_id = std::stoull(token);
    
    return message;
}
