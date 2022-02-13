#include "declaration.h"

#include <iostream>
#include <utility>
#include <algorithm>

lct::TypeVariable::TypeVariable(std::string name):
    name(std::move(name)) { }

lct::TypeArrow::TypeArrow(std::shared_ptr<Type> type1, std::shared_ptr<Type> type2):
    type1(std::move(type1)),
    type2(std::move(type2)) { }

lct::Variable::Variable(std::string name, std::shared_ptr<Type> type):
    name(std::move(name)),
    type(std::move(type)) { }

lct::TermVariable::TermVariable(std::string name):
    name(std::move(name)) { }

lct::TermApplication::TermApplication(std::shared_ptr<Term> term1, std::shared_ptr<Term> term2):
    term1(std::move(term1)),
    term2(std::move(term2)) { }

lct::TermAbstraction::TermAbstraction(lct::Variable variable, std::shared_ptr<Term> term):
    variable(std::move(variable)),
    term(std::move(term)) { }

lct::Sequent::Sequent(std::vector<lct::Variable> context, std::shared_ptr<Term> term, std::shared_ptr<Type> type):
    context(std::move(context)),
    term(std::move(term)),
    type(std::move(type)) { }


std::shared_ptr<lct::Type> lct::TermVariable::infer_type(const std::vector<Variable>& context) {
    for (const auto& variable : context) {
        if (this->name == variable.name) {
            return variable.type;
        }
    }
    throw std::invalid_argument(std::string("No type for variable ") + this->name);
}

std::shared_ptr<lct::Type> lct::TermAbstraction::infer_type(const std::vector<Variable>& context) {
    auto new_context = context;
    new_context.erase(std::remove_if(
        new_context.begin(),
        new_context.end(),
        [this](const Variable& v) { return v.name == this->variable.name; }
    ), new_context.end());
    new_context.push_back(this->variable);

    auto inner_type = this->term->infer_type(new_context);
    return std::make_shared<TypeArrow>(this->variable.type, inner_type);
}

std::shared_ptr<lct::Type> lct::TermApplication::infer_type(const std::vector<Variable>& context) {
    auto type1 = this->term1->infer_type(context);
    auto type2 = this->term2->infer_type(context);

    auto casted_type1 = std::dynamic_pointer_cast<lct::TypeArrow>(type1);
    if (!casted_type1) {
        throw std::invalid_argument(
            "Type of " + this->term1->to_string() + " should be an arrow type, not " + type1->to_string()
        );
    }

    if (!casted_type1->type1->deep_equals(type2)) {
        throw std::invalid_argument(
            "Term " + this->term1->to_string() + " with type " + type1->to_string()
            + " could not be applied to the term " + this->term2->to_string() + " with type " + type2->to_string()
        );
    }

    return casted_type1->type2;
}


bool lct::TypeVariable::deep_equals(std::shared_ptr<lct::Type> other) {
    if (auto casted_other = std::dynamic_pointer_cast<lct::TypeVariable>(other)) {
        return this->name == casted_other->name;
    }
    return false;
}

bool lct::TypeArrow::deep_equals(std::shared_ptr<lct::Type> other) {
    if (auto casted_other = std::dynamic_pointer_cast<lct::TypeArrow>(other)) {
        return this->type1->deep_equals(casted_other->type1) && this->type2->deep_equals(casted_other->type2);
    }
    return false;
}


std::string lct::TypeVariable::to_string() {
    return this->name;
}

std::string lct::TypeArrow::to_string() {
    auto type1_string = this->type1->to_string();
    if (std::dynamic_pointer_cast<lct::TypeArrow>(this->type1)) {
        type1_string = "(" + type1_string + ")";
    }
    return type1_string + "→" + this->type2->to_string();
}


std::string lct::TermVariable::to_string() {
    return this->name;
}

std::string lct::TermApplication::to_string() {
    auto term1_string = this->term1->to_string();
    if (std::dynamic_pointer_cast<lct::TermAbstraction>(this->term1)) {
        term1_string = "(" + term1_string + ")";
    }

    auto term2_string = this->term2->to_string();
    if (std::dynamic_pointer_cast<lct::TermApplication>(this->term2)
        || std::dynamic_pointer_cast<lct::TermAbstraction>(this->term2)) {
        term2_string = "(" + term2_string + ")";
    }

    return term1_string + term2_string;
}

std::string lct::TermAbstraction::to_string() {
    return "λ" + this->variable.to_string() + "." + this->term->to_string();
}


std::string lct::Variable::to_string() {
    return this->name + ":" + this->type->to_string();
}


std::shared_ptr<lct::Type> lct::Sequent::check_type() {
    auto inferred_type = this->term->infer_type(this->context);
    if (this->type && !inferred_type->deep_equals(this->type)) {
        throw std::invalid_argument(
            "Inferred type of the sequent is " + inferred_type->to_string()
            + ", but " + this->type->to_string() + " expected"
        );
    }
    return inferred_type;
}
