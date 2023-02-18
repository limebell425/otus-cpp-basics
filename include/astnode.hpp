// OTUS C++ basic course homework.
// ASTNode

#pragma once

#include <cctype>
#include <iostream>
#include <string>

class ASTNode {
  public:
    explicit ASTNode(const std::string &repr);

    ASTNode(const std::string &repr, ASTNode *lhs, ASTNode *rhs);

    ASTNode(const ASTNode &other) = delete;

    ASTNode &operator=(const ASTNode &other) = delete;

    ~ASTNode();

    std::string repr() const { return repr_; }

    void print(std::ostream &out) const;

  private:
    void inner_print(std::ostream &out, size_t indent) const;

    std::string repr_;
    ASTNode *lhs_;
    ASTNode *rhs_;
};

class AddNode : public ASTNode{
  public: 
    AddNode(ASTNode *lhs, ASTNode *rhs);

    AddNode(const AddNode &other) = delete;
    AddNode &operator=(const AddNode &other) = delete;
    ~AddNode() = default;
};

class SubNode : public ASTNode{
  public: 
    SubNode(ASTNode *lhs, ASTNode *rhs);

    SubNode(const SubNode &other) = delete;
    SubNode &operator=(const SubNode &other) = delete;
    ~SubNode() = default;
};

class MulNode : public ASTNode{
  public: 
    MulNode(ASTNode *lhs, ASTNode *rhs);

    MulNode(const MulNode &other) = delete;
    MulNode &operator=(const MulNode &other) = delete;
    ~MulNode() = default;
};

class DivNode : public ASTNode{
  public: 
    DivNode(ASTNode *lhs, ASTNode *rhs);

    DivNode(const DivNode &other) = delete;
    DivNode &operator=(const DivNode &other) = delete;
    ~DivNode() = default;
};

class Number : public ASTNode {
  public:
    Number(int val);

    Number(const Number &other) = delete;
    Number &operator=(const Number &other) = delete;
    ~Number() = default;  
};

class Variable : public ASTNode {
  public:
    Variable( std::string var);

    Variable(const Variable &other) = delete;
    Variable &operator=(const Variable &other) = delete;
    ~Variable() = default;  
};