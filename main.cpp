#include "DataFrame.h"
#include "command.h"
#include <iostream>
#include <memory>

// driver
int main() {
    // Read, Evaluate, Print, Load (RELP) owns the DataFrame
    auto df = std::make_unique<DataFrame>();

    // commands system
    CommandRegistry registry;
    CommandParser parser;
    CommandHandler handler(df, registry);

    std::cout << "simple DataFrame\n";
    std::cout << "type 'help' to see available commands.\n";

    while (true) {
        std::cout << "\n> ";

        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            std::cout << "Goodbye!\n";
            break;
        }

        auto [cmdName, args] = parser.parse(input);

        if (cmdName.empty()) {
            std::cout << "unknown command. Type 'help' for list.\n";
            continue;
        }
        handler.dispatch(cmdName, args);
    }
    return 0;
}
