//==-- ASTGenerator.h - RCodeEditor AST Generator -----------------*- C++ -*-==//
//* Based on Clang LibTooling Libraries                                       *//
//* Copyright (c) 2023-2024 Rakyuu Studio. All rights reserved.               *//
//* Retoccilus licensed under GNU GPL v3 license.                             *//
//* See https://www.gnu.org/licenses/gpl-3.0.html for license information.    *//
//* Clang licensed under Apache License v2.0 with LLVM Exceptions.            *//
//* See https://llvm.org/LICENSE.txt for license information.                 *//
//==--------------------------------------------------------------------------*//

#ifndef RCodeEditor_ASTGenerator_h
#define RCodeEditor_ASTGenerator_h

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/EvaluatedExprVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include <vector>
#include <map>

using namespace clang;
using namespace std;

class Heap {
    map<long, long> rBuffers;
    map<long, long> rContents;
public:
    Heap() : rBuffers(), rContents() {}

    long rhMalloc(int size) {
        //assert (mBufs.find(addr) == mHeap.end());
        /// Allocate the buf
        int * buf = (int *)malloc(size * sizeof(int) );
        rBuffers.insert(std::make_pair((long)buf, size));

        /// Initialize the Content
        for (int i=0; i<size; i ++) {
            rContents.insert(std::make_pair((long)(buf+i), 0));
        }
        return (long)buf;
    }
};

class Environment {

};

class ASTVisitor : public EvaluatedExprVisitor<ASTVisitor> {
public:
    explicit ASTVisitor(const ASTContext &context, Environment *env) : EvaluatedExprVisitor<ASTVisitor>(context), rEnv(env) {}
    virtual ~ASTVisitor() = default;

};

/**
 * @brief This class prevent you an ASTHandler by clang::ASTConsumer.
 *
 */
class RASTConsumer : public clang::ASTConsumer {
public:
    explicit RASTConsumer(const ASTContext &context) : rEnv(), rVisitor(context &rEnv)
    {

    }
    ~RASTConsumer() override = default;
    virtual void HandleTranslationUnit(clang::ASTContext &Context) {
        TranslationUnitDecl * decl = Context.getTranslationUnitDecl();
        mEnv.init(decl);

        FunctionDecl * entry = mEnv.getEntry();
        mVisitor.VisitStmt(entry->getBody());
    }
private:
    Environment *rEnv;
    ASTVisitor *rVisitor;
};

#endif //RCodeEditor_ASTGenerator_h