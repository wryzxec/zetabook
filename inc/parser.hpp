#pragma once

#include <string>
#include <cstddef>
#include <optional>
#include <fstream>

#include "message.hpp"

class Parser {
public:
    Parser(const std::string& message_file_path, size_t levels);
    
    bool is_message_file_open() const;
    std::optional<Message> read_message();
private:
    std::ifstream message_stream_;
    size_t levels_;
};
