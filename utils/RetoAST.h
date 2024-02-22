#ifndef RetoAST_h
#define RetoAST_h

#include <string>
#include <memory>
#include <utility>
#include <vector>

using namespace std;

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual void print() const = 0;
};

class NumericExpressionASTNode : public ASTNode {
    double value;
public:
    explicit NumericExpressionASTNode(double val) : value(val) {}
};

class VariableNameASTNode : public ASTNode {
    string VarName;
public:
    explicit VariableNameASTNode(const string& varName) : VarName(VarName) {}
};

class CalculationASTNode : public ASTNode {
public:
    enum class OperatorType {
        OT_Plus,
        OT_Minus,
        OT_Multiply,
        OT_Divide,
        OT_Mod,
    };
    unique_ptr<ASTNode> lhs, rhs;
    OperatorType optType;

    CalculationASTNode(unique_ptr<ASTNode> left, OperatorType op, unique_ptr<ASTNode> right)
            : lhs(std::move(left)), optType(op), rhs(std::move(right)) {

    }
};

class UnaryOperatorASTNode : public ASTNode {
public:
    enum class UOperatorType {
        UOT_SelfPlus,
        UOT_SelfMinus
    };

    UOperatorType operatorType;
    unique_ptr<ASTNode> operand;

    UnaryOperatorASTNode(UOperatorType op, unique_ptr<ASTNode> operand_)
            : operatorType(op), operand(std::move(operand_)) {

    }
};

class FunctionCallASTNode : public ASTNode {
public:
    string functionName;
    vector<unique_ptr<ASTNode>> arguments;

    FunctionCallASTNode(string name, vector<unique_ptr<ASTNode>> args)
            : functionName(std::move(name)), arguments(std::move(args)) {

    }
};

class AssignmentASTNode : public ASTNode {
public:
    unique_ptr<VariableNameASTNode> variable;
    unique_ptr<ASTNode> value;

    AssignmentASTNode(unique_ptr<VariableNameASTNode> var, unique_ptr<ASTNode> val)
            : variable(std::move(var)), value(std::move(val)) {

    }
};

class CodeBlockStatementASTNode : public ASTNode {
public:
    vector<unique_ptr<ASTNode>> statements;

    explicit CodeBlockStatementASTNode(vector<unique_ptr<ASTNode>> stamts) : statements(std::move(stamts)) {

    }
};

class IfStatementASTNode : public ASTNode {
public:
    unique_ptr<ASTNode> condition;
    unique_ptr<CodeBlockStatementASTNode> thenBlock;
    unique_ptr<CodeBlockStatementASTNode> elseBlock;

    IfStatementASTNode(unique_ptr<ASTNode> cond, unique_ptr<CodeBlockStatementASTNode> thenStmt,
                       unique_ptr<CodeBlockStatementASTNode> elseStmt = nullptr)
            : condition(std::move(cond)), thenBlock(std::move(thenStmt)), elseBlock(std::move(elseStmt)) {

    }
};

class WhileStatementASTNode : public ASTNode {
public:
    unique_ptr<ASTNode> condition;
    unique_ptr<CodeBlockStatementASTNode> body;

    WhileStatementASTNode(unique_ptr<ASTNode> cond, unique_ptr<CodeBlockStatementASTNode> body_)
            : condition(std::move(cond)), body(std::move(body_)) {

    }
};

class CaseStatementASTNode {
public:
    unique_ptr<ASTNode> caseExpression;
    unique_ptr<CodeBlockStatementASTNode> body;

    CaseStatementASTNode(unique_ptr<ASTNode> expr, unique_ptr<CodeBlockStatementASTNode> stmt)
            : caseExpression(std::move(expr)), body(std::move(stmt)) {

    }
};

class SwitchStatementASTNode : public ASTNode {
public:
    unique_ptr<ASTNode> switchExpression;
    vector<std::unique_ptr<CaseStatementASTNode>> cases;
    unique_ptr<CodeBlockStatementASTNode> defaultCase;

    explicit SwitchStatementASTNode(unique_ptr<ASTNode> expr)
            : switchExpression(std::move(expr)) {}

    void addCase(unique_ptr<ASTNode> caseExpr, unique_ptr<CodeBlockStatementASTNode> caseBody) {
        cases.push_back(make_unique<CaseStatementASTNode>(std::move(caseExpr), std::move(caseBody)));
    }

    void setDefaultCase(unique_ptr<CodeBlockStatementASTNode> defaultStmt) {
        defaultCase = std::move(defaultStmt);
    }
};

class ArrayAccessASTNode : public ASTNode {
public:
    unique_ptr<ASTNode> arrayExpression;
    unique_ptr<ASTNode> indexExpression;

    ArrayAccessASTNode(unique_ptr<ASTNode> arrExpr, unique_ptr<ASTNode> idxExpr)
            : arrayExpression(std::move(arrExpr)), indexExpression(std::move(idxExpr)) {}
};

class ReturnASTNode : public ASTNode {
public:
    unique_ptr<ASTNode> returnValue;

    explicit ReturnASTNode(unique_ptr<ASTNode> retValue = nullptr) : returnValue(std::move(retValue)) {}
};


#endif //RetoAST_h