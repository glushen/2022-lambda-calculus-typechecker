#pragma once

#include <string>
#include "declaration.h"

namespace lctp {
    struct Result {
        bool accepted = false;
        lct::Sequent output;
        std::string error_message;
    };
    Result parse_sequent(const std::string&);
}
