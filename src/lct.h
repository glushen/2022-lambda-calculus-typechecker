#pragma once

#include <string>
#include "lct_parser.h"

namespace lct {
    struct Result {
        bool accepted = false;
        lct::Sequent sequent;
        std::shared_ptr<lct::Type> inferred_type;
        std::string error_message;
    };
    Result check_sequent(const std::string&);
}
