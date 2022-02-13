#include "gtest/gtest.h"
#include "lct.h"

void assert_success(const std::string& input) {
    ASSERT_TRUE(lct::check_sequent(input).accepted);
}

void assert_fail(const std::string& input, const std::string& error_message) {
    auto result = lct::check_sequent(input);
    ASSERT_FALSE(result.accepted);
    ASSERT_EQ(error_message, result.error_message);
}


TEST(test, sample1) {
    assert_success("λf:(α→α→γ).λx:α.λy:β.fxx:(α→α→γ)→α→β→γ");
}

TEST(test, sample2) {
    assert_success("λf:(α→β)→α.λg:α→α→β.g(f(λx:α.gxx))(f(λx:α.gxx)):((α→β)→α)→(α→α→β)→β");
}

TEST(test, sample3) {
    assert_success("x:α→β→γ,z:α,y:α→β⊢xz(yz):γ");
}
