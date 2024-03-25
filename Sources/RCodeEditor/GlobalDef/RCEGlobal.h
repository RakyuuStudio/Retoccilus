//== RCEGlobal.h - RCodeEditor global definitions -----------------*- C++ -*-==//
// Copyright (C) 2024 Rakyuu Studio. All rights reserved.                      //
// This Project is licensed under GNU GPL v3.0 and later.                      //
// See the LICENSE file in the project root for full license information.     //
// THIS FILE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.                //
//===----------------------------------------------------------------------===//

#ifndef RCE_GLOBAL_H
#define RCE_GLOBAL_H

#include "qglobal.h"

#define RCE_VERSION "RtIntegrated_1.0.0"
#define RCE_VERSION_HEX 0x010000

//We only support Qt 6.0 and later
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#  error "This version of RCodeEditor requires Qt 6.0 or later"
#endif

// Define the RCE_EXPORT macro
#ifdef RCE_STATIC
#define RCE_EXPORT Q_DECL_EXPORT
#else
#define RCE_EXPORT Q_DECL_EXPORT
#endif

#if !defined(QT_BEGIN_NAMESPACE)
#define QT_BEGIN_NAMESPACE
#define QT_END_NAMESPACE
#endif

#endif // RCE_GLOBAL_H