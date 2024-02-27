//This is insane
#ifndef Rtcc_Lex_h
#define Rtcc_Lex_h

//== Retoccilus Cpp Compiler Library ==
#include <RetoGlobal.h>
#include <RXmlReader.h>

//== Standard C++ Library ==
#include <unordered_set>               //Keyword Set
#include <cassert>                     //Assert
#include <cstdint>                     //Fix integer width
#include <optional>                    //Show the value exists or not
#include <string>                      //C++ New String

using namespace std;
using namespace Retoccilus;

#if Reto_UseRetCC_Compiler
namespace retccUtils {
    class MemoryBufferReference;
}

namespace RetoCompiler {
    class CppLanguageOptions;

    class DiagnosticBldr;

    class Preprocessor;

    class SourceManagerX;

    /// Enumeration: Type of Version Control System to mark Conflict between branch/environment with online
    enum VCSConflictType {
        VCCT_None,
        VCCT_GitNormal,
        VCCT_Perforce,
        VCCT_CVS
    };

    struct FilePreamble {
        unsigned size; //Byte
        bool jumpOverPreambleEndsAtSOL; //SOL -> Start of Line
        FilePreamble(unsigned conSize, bool conJumpOverPreambleEndsAtSOL)
                : size(conSize), jumpOverPreambleEndsAtSOL(conJumpOverPreambleEndsAtSOL) {}
    };

    class rtccLexer : public PreprocessorLexer {
        friend class Preprocessor;
    public:
        const char *BufferStart, *BufferEnd;

    };
}
#endif

#endif //Rtcc_Lex_h