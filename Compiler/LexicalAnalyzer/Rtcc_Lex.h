/*
 * Retoccilus Source Code Library Header: Rtcc_Lex.h
 * Copyright (C) 2023-2024 Rakyuu Studio, All rights reserved.
 *
 * This Source Code Library is licensed under the GNU General Public License (GPL) version 3.
 * For more information about this license, visit: https://www.gnu.org/licenses/lgpl-3.0.en.html
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Rakyuu Studio nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL RAKYUU STUDIO OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Contact Information:
 * For any questions regarding this Source Code Library, please contact:
 */

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
namespace RetoCompiler {
    /**
     * @brief These Enumeration defined lots of markers that used in Git or other VCS to show developer the conflict exist
     */
    enum ConflictType {
        Conflict_None,
        /*
         * e.g. (7 < and >)
         * <<<<<<< HEAD
         * Modified on current branch
         * =======
         * Modified on other branch
         * >>>>>>> other_branch
         */
        Conflict_Normal,
        /*
         * e.g. (8 < and >)
         * <<<<<<<< yours
         * Modified from your workspace
         * =======
         * Modified from other user's workspace
         * >>>>>>>> theirs
         */
        Conflict_Perforce,
        /*
         * <<<<<<<
         * Modified from your workspace
         * =======
         * Modified from repository
         * >>>>>>>
         */
        Conflict_SVN,
        /*
         * <<<<<<<
         * Modified from your workspace
         * =======
         * Modified from repository
         * >>>>>>>
         */
        Conflict_CVS
        //No Mercurial because it use inline markers not these
    };


}
#endif

#endif //Rtcc_Lex_h