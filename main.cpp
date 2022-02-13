#include <iostream>
#include "lct.h"

void printContext(const std::vector<lct::Variable>& context) {
    if (context.empty()) {
        std::cout << "empty";
    } else {
        auto first = true;
        for (auto variable: context) {
            if (first) {
                first = false;
            } else {
                std::cout << ", ";
            }

            std::cout << variable.to_string();
        }
    }
}

int main() {
    auto firstTime = true;

    while (true) {
        if (firstTime) {
            firstTime = false;
            std::cout << "Enter a sequent in the form [Γ⊢]t[:A] (you could use | as ⊢, \\ as λ and -> as →):" << std::endl;
        } else {
            std::cout << std::endl << "Enter another sequent, or leave the line empty to exit:" << std::endl;
        }

        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) {
            break;
        }

        auto result = lct::check_sequent(line);
        if (result.accepted) {
            std::cout << "The sequent is correct." << std::endl;
        } else {
            std::cout << "Error: " << result.error_message << "." << std::endl;
        }

        std::cout << "Context: ";
        printContext(result.sequent.context);
        std::cout << std::endl;

        std::cout << "Term: ";
        if (result.sequent.term) {
            std::cout << result.sequent.term->to_string() << std::endl;
        } else {
            std::cout << "none" << std::endl;
        }

        std::cout << "Expected type: ";
        if (result.sequent.type) {
            std::cout << result.sequent.type->to_string() << std::endl;
        } else {
            std::cout << "none" << std::endl;
        }

        if (result.accepted) {
            std::cout << "Inferred type: " << result.inferred_type->to_string() << std::endl;
        }
    }
}
