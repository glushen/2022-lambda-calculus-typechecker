#include "lct.h"
#include "lct_parser.h"

lct::Result lct::check_sequent(const std::string& s) {
    auto parse_result = lctp::parse_sequent(s);
    if (!parse_result.accepted) {
        return lct::Result { .accepted = false, .error_message = parse_result.error_message };
    }

    auto sequent = parse_result.output;
    try {
        auto inferred_type = sequent.check_type();
        return Result { .accepted = true, .sequent = sequent, .inferred_type = inferred_type };
    } catch (const std::invalid_argument& exception) {
        return Result { .accepted = false, .sequent = sequent, .error_message = exception.what() };
    }
}
