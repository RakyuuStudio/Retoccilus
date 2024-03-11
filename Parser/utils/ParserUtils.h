//Note: This parser is fit to Qt, and likely not fit to original c/cpp;
//      Trust me, I will make another project that rebuild this parser to fit original c/cpp :) >w<
#ifndef RetoParser_Utility_h
#define RetoParser_Utility_h

#include <QMap>
#include <QObject>
#include <QSet>
#include <QVector>

#include <memory>
#include <functional>

using namespace std;
using getFileStreamCallingBack = function<bool>(const QString&, const QStringList&);

namespace RParser {
    enum class ParserLanguage {
        C,
        Cpp,
        ObjC,
        ObjCpp,

        RetoExt
    };

    struct CodeSnippet {
        int sec;

        QString caption; //Snippet Name
        QString prefix;  //A prefix that fit the code suggester
        QString code;    //Main body of the code;
        QString desc;    //Description
    };

    struct Defination {
        QString name;
        QString arguments;
        QString value;
        QString filename;
        QString formatValue;

        bool codeIsHardcodeInMemory;   //Is the code Hard-Code in the memory? If yes, we don't free memory.
        int  variableArgumentIndex;

        QList<bool> argumentUsed;
    };

    enum class KeywordType {
        //===Operators====
        ktOp_SkipIt,
        ktOp_SkipNextColon,
        ktOp_SkipNextSemicolon,
        ktOp_SkipNextParentheses,
        ktOp_MoveToLBrace,
        ktOp_MoveToRBrace,
        //===Loop====
        kt_For,            //for(statements) {} Loop
        //===Exception====
        kt_Catch,          //catch {} Block
        //===Priority====
        kt_Public,
        kt_Private,
        kt_Protected,
        //===Function Declaration====
        kt_Inline,
        kt_Friend,
        //===Miscellaneous====
        kt_Namespace,
        kt_Enum,
        kt_Struct,
        kt_TypeDef,
        kt_Using,
        kt_DeclareType,
        kt_Concept,
        kt_Operator,
        kt_Requires,
        kt_NULL,
        kt_NotAKeyword
    };

    enum StatementKind {
        stk_Unknown,
        stk_Namespace,
        stk_NamespaceAlia,
        stk_Class,
        stk_Preprocessor,
        stk_Enumeration,
        stk_EnumerationType,
        stk_EnumerationClass,
        stk_TypeDefine,
        stk_Constructor,
        stk_Destructor,
        stk_Function,
        stk_Variable,
        stk_GlobalVariable,
        stk_PrivateVariable,
        stk_Operator,
        stk_ParameterList,
        stk_Block,
        stk_CodeSnippetUserDeclares,
        stk_Keyword,
        stk_KeywordType, //e.g. int, double, float, uint64 etc.
        stk_Alias,
        stk_NamespaceAlias,
    };

    enum class StatementScope {
        Global,
        ClassLocal,
        Local
    };

    enum class StatementPriority {
        None,
        Public,
        Private,
        Protected
    };

    enum class MemberOperator {
        PointerArrow, //->
        Dot, //.
        DoubleColon, //::
        Other
    };

    enum class EvaluateStatementType {
        Namespace,
        Type,
        Variable,
        Literal,
        Function
    };

    struct StatementMatchPosStructure {
        unsigned short s_start;
        unsigned short s_end;
    };

    enum StatementProperty {
        sp_None                               = 0x0,
        sp_Static                             = 0x0001,
        sp_DefinitionExist                    = 0x0002,
        sp_InAProject                         = 0x0004,
        sp_InLibraryHeader                    = 0x0008,
        sp_Inherited                          = 0x0010,
        sp_Virtual                            = 0x0020,
        sp_Override                           = 0x0040,
        sp_ConstExpression                    = 0x0080,
        sp_FunctionPointer                    = 0x0100,
        sp_OperatorOverload                   = 0x0200,
        sp_DummyStatement                     = 0x0400
    };

    struct Statement; //Forward Declaration

    using RCodeSnippetPtr = shared_ptr<CodeSnippet>;
    using RHashDefinationMap = QHash<QString, Defination>;
    using RDefinationMapPtr = shared_ptr<RHashDefinationMap>;
    using RStatementKindSet = QSet<StatementKind>;
//    using RStatementKindSet = QHash<StatementKind, KeywordType>; //I think it is useless
    using RStatementMatchPosition = shared_ptr<StatementMatchPosStructure>;
    using RStatement = shared_ptr<Statement>;
    using RStatementList = QList<RStatement>;
    using RStatementListPointer = shared_ptr<RStatementList>;
    using RStatementMultimap = QMultiMap<QString, RStatement>;
    Q_DECLARE_FLAGS(StatementProperties, StatementProperty)
    Q_DECLARE_OPERATORS_FOR_FLAGS(StatementProperties)

    struct Statement {
        weak_ptr<Statement> parentScope;
        QString type, command, argument, value;
        StatementKind kind;
        StatementScope scope;
        StatementPriority accPriority;
        int line, definationLine;
        QString fileName, definationFileName;
        RStatementMultimap childrenMap;
        QSet<QString> friends, usingList;
        QString fullName; //Include the class and parent identifiers
        QString noNameArguments;
        StatementProperties properties;

        int usageCc;
        unsigned short matchPositionTot, matchPosSpan, firstMatchLength, caseMatched;
        QList<RStatementMatchPosition> matchPositions;

        bool hasDefination() { return properties.testFlag(StatementProperty::sp_DefinitionExist); }
        bool inProject() { return properties.testFlag(StatementProperty::sp_InAProject); }
        bool inLibraryHeader() { return properties.testFlag(StatementProperty::sp_InLibraryHeader); }
        bool isStatic() { return properties.testFlag(StatementProperty::sp_Static); }
        bool isInherited() { return properties.testFlag(StatementProperty::sp_Inherited); }

        void setHasDefination(bool on) { properties.setFlag(StatementProperty::sp_DefinitionExist,on); }
        void setInProject(bool on) { properties.setFlag(StatementProperty::sp_InAProject,on); }
        void setInLibraryHeader(bool on) { properties.setFlag(StatementProperty::sp_InLibraryHeader,on); }
        void setIsStatic(bool on) { properties.setFlag(StatementProperty::sp_Static,on); }
    };
}

#endif