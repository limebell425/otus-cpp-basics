#include "astnode.hpp"

ASTNode::ASTNode(const std::string &repr)
    : repr_(repr)
    , lhs_{nullptr}
    , rhs_{nullptr} {}

ASTNode::ASTNode(const std::string &repr, ASTNode *lhs, ASTNode *rhs)
    : repr_(repr)
    , lhs_{lhs}
    , rhs_{rhs} {}

ASTNode::~ASTNode() {
    delete lhs_;
    delete rhs_;
}

void ASTNode::print(std::ostream &out) const { inner_print(out, 0); }

void ASTNode::inner_print(std::ostream &out, size_t indent) const {
    if (lhs_) {
        lhs_->inner_print(out, indent + 1);
    }
    for (size_t i = 0; i < indent; ++i) {
        out << "    ";
    }
    out << repr_ << '\n';
    if (rhs_) {
        rhs_->inner_print(out, indent + 1);
    }
}


AddNode::AddNode(ASTNode *lhs, ASTNode *rhs)
    : ASTNode("+", lhs, rhs)
{}

SubNode::SubNode(ASTNode *lhs, ASTNode *rhs)
    : ASTNode("-", lhs, rhs)
{}

MulNode::MulNode(ASTNode *lhs, ASTNode *rhs)
    : ASTNode("*", lhs, rhs)
{}

DivNode::DivNode(ASTNode *lhs, ASTNode *rhs)
    : ASTNode("/", lhs, rhs)
{}

Number::Number(int val)
    : ASTNode(std::to_string(val))
{}

Variable::Variable(std::string var)
    : ASTNode(var)
{}