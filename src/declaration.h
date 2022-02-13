#pragma once

#include <string>
#include <memory>
#include <vector>

namespace lct {
    class Type {
    public:
        virtual ~Type() = default;
        virtual bool deep_equals(std::shared_ptr<lct::Type>) = 0;
        virtual std::string to_string() = 0;
    };

    class TypeVariable: public Type {
    public:
        std::string name;
        explicit TypeVariable(std::string name);
        TypeVariable() = default;
        bool deep_equals(std::shared_ptr<lct::Type> other) override;
        std::string to_string() override;
    };

    class TypeArrow: public Type {
    public:
        std::shared_ptr<Type> type1;
        std::shared_ptr<Type> type2;
        TypeArrow(std::shared_ptr<Type> type1, std::shared_ptr<Type> type2);
        TypeArrow() = default;
        bool deep_equals(std::shared_ptr<lct::Type>) override;
        std::string to_string() override;
    };

    class Variable {
    public:
        std::string name;
        std::shared_ptr<Type> type;
        Variable(std::string name, std::shared_ptr<Type> type);
        Variable() = default;
        std::string to_string();
    };

    class Term {
    public:
        virtual std::shared_ptr<Type> infer_type(const std::vector<Variable>& context) = 0;
        virtual ~Term() = default;
        virtual std::string to_string() = 0;
    };

    class TermVariable: public Term {
    public:
        std::string name;
        explicit TermVariable(std::string name);
        TermVariable() = default;
        std::shared_ptr<Type> infer_type(const std::vector<Variable>& context) override;
        std::string to_string() override;
    };

    class TermApplication: public Term {
    public:
        std::shared_ptr<Term> term1;
        std::shared_ptr<Term> term2;
        TermApplication(std::shared_ptr<Term> term1, std::shared_ptr<Term> term2);
        TermApplication() = default;
        std::shared_ptr<Type> infer_type(const std::vector<Variable>& context) override;
        std::string to_string() override;
    };

    class TermAbstraction: public Term {
    public:
        Variable variable;
        std::shared_ptr<Term> term;
        TermAbstraction(Variable variable, std::shared_ptr<Term> term);
        TermAbstraction() = default;
        std::shared_ptr<Type> infer_type(const std::vector<Variable>& context) override;
        std::string to_string() override;
    };

    class Sequent {
    public:
        std::vector<Variable> context;
        std::shared_ptr<Term> term;
        std::shared_ptr<Type> type;
        Sequent(std::vector<Variable> context, std::shared_ptr<Term> term, std::shared_ptr<Type> type);
        Sequent() = default;
        std::shared_ptr<Type> check_type();
    };
}
