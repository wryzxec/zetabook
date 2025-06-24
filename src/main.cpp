#include "../inc/zetabook.hpp"
#include "../inc/parser.hpp"

#include <iostream>

int main() {
    const std::string MESSAGE_FILE_PATH = "../data/message-multi.csv";

    Parser parser(MESSAGE_FILE_PATH, 2);

    ZetaBook book {};

    while(auto msg = parser.read_message()) {
        book.process_message(*msg);
    }   

    auto snap = book.snapshot_top_levels(2);
    std::cout << snap.to_string() << std::endl;
}
