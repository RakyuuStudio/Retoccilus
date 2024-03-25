//== RCEBase.h - RCodeEditor Widget Base and Low-level API -----------------*- C++ -*-==//
// Copyright (C) 2024 Rakyuu Studio. All rights reserved.                               //
// This Project is licensed under GNU GPL v3.0 and later.                               //
// See the LICENSE file in the project root for full license information.               //
// THIS FILE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.                          //
//===--------------------------------------------------------------------------------===//

//Some code in this file is based on the QScintilla project.
//QScintilla is licensed under the GNU GPL v3.0 and later.
//See the LICENSE file in the QScintilla project for full license information.

#ifndef RCE_BASE_H
#define RCE_BASE_H

#include "RCEGlobal.h"

#include <QAbstractScrollArea>
#include <QPoint>
#include <QTextBlock>
#include <QTimer>
#include <QTextCursor>

#include "qglobal.h"

QT_BEGIN_NAMESPACE
class QColor;
class QImage;
class QMimeData;
class QPainter;
class QPixmap;
class QUrl;

class QTextCursor;
class QTextDocument;
QT_END_NAMESPACE

class RetoCodeEditor;

/// \brief The RCEBase class is the base class for the RCodeEditor Widget.
/// RCodeEditor Widget integrated into Retoccilus Game Engine.
/// This class implements the low-level API for the RCodeEditor Widget.
class RCE_EXPORT RCEBase : public QAbstractScrollArea {
Q_OBJECT
public:
    enum class RceGlobalSignals {
        RCE_Start = 2000,
        RCE_OptionalStart = 3000,
        RCE_LexerStart = 4000,

        RCE_AddText = 2001,
        RCE_AddStyledText = 2002,
        RCE_InsertText = 2003,
        RCE_ClearAll = 2004,
        RCE_ClearDocumentStyle = 2005,

        RCE_GetLength = 2006,
        RCE_GetCharAt = 2007,
        RCE_GetCurrentPosition = 2008,
        RCE_GetAnchorInstance = 2009,
        RCE_GetStyleAt = 2010,
        RCE_Redo = 2011,
        RCE_SetUndoCollection = 2012,
        RCE_SelectAll = 2013,
        RCE_SetSavePoint = 2014,
        RCE_GetStyledPoint = 2015,
        RCE_CanRedo = 2016,
        RCE_MarkerLineInstanceFromHandle = 2017,
        RCE_MarkerLineInstanceDeleteHandle = 2018,
        RCE_GetUndoCollection = 2019,
        RCE_GetViewWs = 2020,
        RCE_SetViewWs = 2021,
        RCE_PositionFromPoint = 2022,
        RCE_PositionFromPointClose = 2023,
        RCE_GotoLine = 2024,
        RCE_GotoPos = 2025,
        RCE_SetAnchor = 2026,
        RCE_GetCurrentLine = 2027,
        RCE_GetEndStyled = 2028,
        RCE_ConvertEOLs = 2029,
        RCE_GetEOLMode = 2030,
        RCE_SetEOLMode = 2031,
        RCE_StartStyling = 2032,
        RCE_SetStyling = 2033,
        RCE_GetBufferedDraw = 2034,
        RCE_SetBufferedDraw = 2035,
        RCE_SetTabDraw = 2036,
        RCE_GetTabWidth = 2121,
        RCE_SetCodePage = 2037,
        RCE_MarkerDefine = 2040,
        RCE_MarkerSetForeground = 2041,
        RCE_MarkerSetBackground = 2042,
        RCE_MarkerAdd = 2043,
        RCE_MarkerDelete = 2044,
        RCE_MarkerDeleteAll = 2045,
        RCE_MarkerGet = 2046,
        RCE_MarkerNext = 2047,
        RCE_MarkPrev = 2048,
        RCE_MarkerDefinePixmap = 2049,
        RCE_SetMarginTypeN = 2240,
        RCE_GetMarginTypeN = 2241,
        RCE_SetMarginWidthN = 2242,
        RCE_GetMarginWidthN = 2243,
        RCE_SetMarginMaskN = 2244,
        RCE_GetMarginMaskN = 2245,
        RCE_SetMarginSensitiveN = 2246,
        RCE_GetMarginSensitiveN = 2247,
        RCE_SetMarginCursorN = 2248,
        RCE_GetMarginCursorN = 2249,
        RCE_StyleClearAll = 2050,
        RCE_StyleSetForeground = 2051,
        RCE_StyleSetBackground = 2052,
        RCE_StyleSetBold = 2053,
        RCE_StyleSetItalic = 2054,
        RCE_StyleSetSize = 2055,
        RCE_StyleSetFont = 2056,
        RCE_StyleSetEOLFilled = 2057,
        RCE_StyleResetDefault = 2058,
        RCE_StyleSetUnderLine = 2059,
        RCE_StyleSetCase = 2060,
        RCE_StyleSetSizeFractional = 2061,
        RCE_StyleGetSizeFractional = 2062,
        RCE_StyleSetWeight = 2063,
        RCE_StyleGetWeight = 2064,
        RCE_StyleGetCharacterSet = 2066,
        RCE_SetSelectionForeground = 2067,
        RCE_SetSelectionBackground = 2068,
        RCE_SetCaretForeground = 2069,
        RCE_AssignCommandKey = 2070,
        RCE_ClearCommandKey = 2071,
        RCE_ClearAllCommandKeys = 2072,
        RCE_SetStylingEx = 2073,
        RCE_StyleSetVisible = 2074,
        RCE_GetCaretPeriod = 2075,
        RCE_SetCaretPeriod = 2076,
        RCE_SetWordCharacters = 2077,
        RCE_BeginUndoAction = 2078,
        RCE_EndUndoAction = 2079,
        RCE_IndicSetStyle = 2080,
        RCE_IndicGetStyle = 2081,
        RCE_IndicSetForeground = 2082,
        RCE_IndicGetForeground = 2083,
        RCE_SetWhiteSpaceForeground = 2084,
        RCE_SetWhiteSpaceBackground = 2085,
        RCE_SetWhiteSpaceSize = 2086,
        RCE_GetWhiteSpaceSize = 2087,
        RCE_SetStyleBits = 2090,
        RCE_GetStyleBits = 2091,
        RCE_SetLineState = 2092,
        RCE_GetLineState = 2093,
        RCE_GetMaxLineState = 2094,
        RCE_GetCaretLineVisible = 2095,
        RCE_SetCaretLineVisible = 2096,
        RCE_GetCaretLineBack = 2097,
        RCE_SetCaretLineBack = 2098,
        RCE_StyleSetChangeable = 2099,
        RCE_AutoCShow = 2100,
        RCE_AutoCCancel = 2101,
        RCE_AutoCActive = 2102,
        RCE_AutoCPosStart = 2103,
        RCE_AutoCComplete = 2104,
        RCE_AutoCStops = 2105,
        RCE_AutoCSetSeparator = 2106,
        RCE_AutoCGetSeparator = 2107,
        RCE_AutoCSelect = 2108,
        RCE_AutoCSetCancelAtStart = 2110,
        RCE_AutoCGetCancelAtStart = 2111,
        RCE_AutoCSetFillUps = 2112,
        RCE_AutoCSetChooseSingle = 2113,
        RCE_AutoCGetChooseSingle = 2114,
        RCE_AutoCSetIgnoreCase = 2115,
        RCE_AutoCGetIgnoreCase = 2116,
        RCE_UserListShow = 2117,
        RCE_AutoCSetAutoHide = 2118,
        RCE_AutoCGetAutoHide = 2119,
        RCE_AutoCSetDropRestOfWord = 2270,
        RCE_AutoCGetDropRestOfWord = 2271,
        RCE_SetIndent = 2122,
        RCE_GetIndent = 2123,
        RCE_SetUseTabs = 2124,
        RCE_GetUseTabs = 2125,
        RCE_SetLineIndentation = 2126,
        RCE_GetLineIndentation = 2127,
        RCE_GetLineIndentPosition = 2128,
        RCE_GetColumn = 2129,
        RCE_SetHorizontalScrollBar = 2130,
        RCE_GetHorizontalScrollBar = 2131,
        RCE_SetIndentationGuides = 2132,
        RCE_GetIndentationGuides = 2133,
        RCE_SetHighlightGuide = 2134,
        RCE_GetHighlightGuide = 2135,
        RCE_GetLineEndPoistion = 2136,
        RCE_GetCodePage = 2137,
        RCE_GetCaretForeground = 2138,
        RCE_GetReadOnly = 2140,
        RCE_SetCurrentPosition = 2141,
        RCE_SetSelectionStart = 2142,
        RCE_GetSelectionStart = 2143,
        RCE_SetSelectionEnd = 2144,
        RCE_GetSelectionEnd = 2145,
        RCE_SetPrintMagnification = 2146,
        RCE_GetPrintMagnification = 2147,
        RCE_SetPrintColorMode = 2148,
        RCE_GetPrintColorMode = 2149,
        RCE_FindText = 2150,
        RCE_FormatRange = 2151,
        RCE_GetFirstVisibleLine = 2152,
        RCE_GetLine = 2153,
        RCE_GetLineCount = 2154,
        RCE_SetMarginLeft = 2155,
        RCE_GetMarginLeft = 2156,
        RCE_SetMarginRight = 2157,
        RCE_GetMarginRight = 2158,
        RCE_GetModify = 2159,
        RCE_SetSelection = 2160,
        RCE_GetSelectionText = 2161,
        RCE_GetTextRange = 2162,
        RCE_HideSelection = 2163,
        RCE_PointXFromPosition = 2164,
        RCE_PointYFromPosition = 2165,
        RCE_LineFromPosition = 2166,
        RCE_PositionFromLine = 2167,

        RCE_LineScroll = 2168,
        RCE_ScrollCaret = 2169,
        RCE_ReplaceSelection = 2170,
        RCE_SetReadOnly = 2171,
        RCE_Null = 2172,
        RCE_CanPaste = 2173,
        RCE_CanUndo = 2174,
        RCE_EmptyUndoBuffer = 2175,
        RCE_Undo = 2176,
        RCE_Cut = 2177,
        RCE_Copy = 2178,
        RCE_Paste = 2179,
        RCE_Clear = 2180,
        RCE_SetText = 2181,
        RCE_GetText = 2182,
        RCE_GetTextLength = 2183,
        RCE_GetDirectionFunction = 2184,
        RCE_GetDirectPointer = 2185,
        RCE_SetOverType = 2186,
        RCE_GetOverType = 2187,
        RCE_SetCaretWidth = 2188,
        RCE_GetCaretWidth = 2189,
        RCE_SetTargetStart = 2190,
        RCE_GetTargetStart = 2191,
        RCE_SetTargetEnd = 2192,
        RCE_GetTargetEnd = 2193,
        RCE_ReplaceTarget = 2194,
        RCE_ReplaceTargetRe = 2195,
        RCE_SearchInTarget = 2197,
        RCE_SetSearchFlags = 2198,
        RCE_GetSearchFlags = 2199,
        RCE_CallTipShow = 2200,
        RCE_CallTipCancel = 2201,
        RCE_CallTipActive = 2202,
        RCE_CallTipPosStart = 2203,
        RCE_CallTipSetHlt = 2204,
        RCE_CallTipSetBackground = 2205,
        RCE_CallTipSetForeground = 2206,
        RCE_CallTipSetForegroundHlt = 2207,
        RCE_AutoCSetMaxWidth = 2208,
        RCE_AutoCGetMaxWidth = 2209,
        RCE_AutoCSetMaxHeight = 2210,
        RCE_AutoCGetMaxHeight = 2211,
        RCE_CallTipUseStyle = 2212,
        RCE_CallTipSetPosition = 2213,
        RCE_CallTipSetPosStart = 2214,
        RCE_VisibleFromDocLine = 2220,
        RCE_DocLineFromVisible = 2221,
        RCE_SetFoldLevel = 2222,
        RCE_GetFoldLevel = 2223,
        RCE_GetLastChild = 2224,
        RCE_GetFoldParent = 2225,
        RCE_ShowLines = 2226,
        RCE_HideLines = 2227,
        RCE_GetLineVisible = 2228,
        RCE_SetFoldExpanded = 2229,
        RCE_GetFoldExpanded = 2230,
        RCE_ToggleFold = 2231,
        RCE_EnsureVisible = 2232,
        RCE_SetFoldFlags = 2233,
        RCE_EnsureVisibleEnforcePolicy = 2234,
        RCE_WrapCount = 2235,
        RCE_GetALlLineVisible = 2236,
        RCE_FoldLine = 2237,
        RCE_FoldChildren = 2238,
        RCE_ExpandChildren = 2239,
        RCE_SetMarginBackN = 2250,
        RCE_GetMarginBackN = 2251,
        RCE_SetMargins = 2252,
        RCE_GetMargins = 2253,
        RCE_SetTabIndents = 2260,
        RCE_GetTabIndents = 2261,
        RCE_SetBackSpaceUnIndents = 2262,
        RCE_GetBackSpaceUnIndents = 2263,
        RCE_SetMouseDWellTime = 2264,
        RCE_GetMouseDWellTime = 2265,
        RCE_WordStartPos = 2266,
        RCE_WordEndPos = 2267,
        RCE_SetWrapMode = 2268,
        RCE_GetWrapMode = 2269,
        RCE_SetLayoutCache = 2272,
        RCE_GetLayoutCache = 2273,
        RCE_SetScrollWidth = 2274,
        RCE_GetScrollWidth = 2275,
        RCE_TextWidth = 2276,
        RCE_SetEndAtLastLine = 2277,
        RCE_GetEndAtLastLine = 2278,
        RCE_TextHeight = 2279,
        RCE_SetVerticalScrollBar = 2280,
        RCE_GetVerticalScrollBar = 2281,
        RCE_AppendText = 2282,
        RCE_GetTwoPhaseDraw = 2283,
        RCE_SetTwoPhaseDraw = 2284,
        RCE_AutoCSetTypeSeparator = 2285,
        RCE_AutoCGetTypeSeparator = 2286,
        RCE_TargetFromSelection = 2287,
        RCE_LinesJoin = 2288,
        RCE_LinesSplit = 2289,
        RCE_SetFoldMarginColor = 2290,
        RCE_SetFoldMarginHiColor = 2291,
        RCE_MarkerSetBackSelected = 2292,
        RCE_MarkerEnableHighlight = 2293,
        RCE_LineDown = 2300,
        RCE_LineDownExtend = 2301,
        RCE_LineUp = 2302,
        RCE_LineUpExtend = 2303,
        RCE_CharLeft = 2304,
        RCE_CharLeftExtend = 2305,
        RCE_CharRight = 2306,
        RCE_CharRightExtend = 2307,
        RCE_WordLeft = 2308,
        RCE_WordLeftExtend = 2309,
        RCE_WordRight = 2310,
        RCE_WordRightExtend = 2311,
        RCE_Home = 2312,
        RCE_HomeExtend = 2313,
        RCE_LineEnd = 2314,
        RCE_LineEndExtend = 2315,
        RCE_DocumentStart = 2316,
        RCE_DocumentStartExtend = 2317,
        RCE_DocumentEnd = 2318,
        RCE_DocumentEndExtend = 2319,
        RCE_PageUp = 2320,
        RCE_PageUpExtend = 2321,
        RCE_PageDown = 2322,
        RCE_PageDownExtend = 2323,
        RCE_EditToggleOverType = 2324,
        RCE_Cancel = 2325,
        RCE_DeleteBack = 2326,
        RCE_Tab = 2327,
        RCE_BackTab = 2328,
        RCE_NewLine = 2329,
        RCE_FormFeed = 2330,
        RCE_VChome = 2331,
        RCE_VChomeExtend = 2332,
        RCE_ZoomIn = 2333,
        RCE_ZoomOut = 2334,
        RCE_DeleteWordLeft = 2335,
        RCE_DeleteWordRight = 2336,
        RCE_LineCut = 2337,
        RCE_LineDelete = 2338,
        RCE_LineTranspose = 2339,
        RCE_Lowercase = 2340,
        RCE_Uppercase = 2341,
        RCE_LineScrollDown = 2342,
        RCE_LineScrollUp = 2343,
        RCE_DeleteBackNotLine = 2344,
        RCE_HomeDisplay = 2345,
        RCE_HomeDisplayExtend = 2346,
        RCE_LineEndDisplay = 2347,
        RCE_LineEndDisplayExtend = 2348,
        RCE_MoveCaretInsideView = 2401,
        RCE_LingLength = 2350,
        RCE_BraceHighlight = 2351,
        RCE_BraceBadLight = 2352,
        RCE_BraceMatch = 2353,
        RCE_LineReverse = 2354,
        RCE_GetViewEOL = 2355,
        RCE_SetViewEOL = 2356,
        RCE_GetDocPointer = 2357,
        RCE_SetDocPointer = 2358,
        RCE_SetModeVentMask = 2359,
        RCE_GetEdgeColumn = 2360,
        RCE_SetEdgeColumn = 2361,
        RCE_GetEdgeMode = 2362,
        RCE_SetEdgeMode = 2363,
        RCE_GetEdgeColor = 2364,
        RCE_SetEdgeColor = 2365,
        RCE_SearchAnchor = 2366,
        RCE_SearchNext = 2367,
        RCE_SearchPrev = 2368,
        RCE_LinesOnScreen = 2370,
        RCE_UsePopup = 2371,
        RCE_SelectionIsRectangle = 2372,
        RCE_SetZoom = 2373,
        RCE_GetZoom = 2374,
        RCE_CreateDocument = 2375,

        RCE_AddRefDocument = 2376,
        RCE_ReleaseDocument = 2377,
        RCE_GetModeVentMask = 2378,
        RCE_SetFocus = 2380,
        RCE_GetFocus = 2381,
        RCE_SetStatus = 2382,
        RCE_GetStatus = 2383,
        RCE_SetMouseDownCaptures = 2384,
        RCE_GetMouseDownCapture = 2385,
        RCE_SetCursor = 2386,
        RCE_GetCursor = 2387,
        RCE_SetControlCharSymbol = 2388,
        RCE_GetControlCharSymbol = 2389,
        RCE_WordPartLeft = 2390,
        RCE_WordPartLeftExtend = 2391,
        RCE_WordPartRight = 2392,
        RCE_WordPartRightExtend = 2393,
        RCE_SetVisiblePolicy = 2394,
        RCE_DelLineLeft = 2395,
        RCE_DelLineRight = 2396,
        RCE_SetXOffset = 2397,
        RCE_GetXOffset = 2398,
        RCE_ChooseCaretX = 2399,
        RCE_GrabFocus = 2400,
        RCE_SetXCaretPolicy = 2402,
        RCE_SetYCaretPolicy = 2403,
        RCE_LineDuplicate = 2404,
        RCE_RegisterImage = 2405,
        RCE_SetPrintWrapMode = 2406,
        RCE_GetPrintWrapMode = 2407,
        RCE_ClearRegisteredImages = 2408,
        RCE_StyleSetHotSpot = 2409,
        RCE_SetHotSpotActiveForeground = 2410,
        RCE_SetHotSpotActiveBackground = 2411,
        RCE_SetHotSpotActiveUnderline = 2412,
        RCE_ParagraphDown = 2413,
        RCE_ParagraphDownExtend = 2414,
        RCE_ParagraphUp = 2415,
        RCE_ParagraphUpExtend = 2416,
        RCE_PositionBefore = 2417,
        RCE_PositionAfter = 2418,
        RCE_CopyRange = 2419,
        RCE_CopyText = 2420,
        RCE_SetHotSpotSingleLine = 2421,
        RCE_SetSelectionMode = 2422,
        RCE_GetSelectionMode = 2423,
        RCE_GetLineSelectionStartPosition = 2424,
        RCE_GetLineSelectionEndPosition = 2425,
        RCE_LineDownRectExtend = 2426,
        RCE_LineUpRectExtend = 2427,
        RCE_CharLeftRectExtend = 2428,
        RCE_CharRightRectExtend = 2429,
        RCE_HomeRectExtend = 2430,
        RCE_VChomeRectExtend = 2431,
        RCE_LineEndRectExtend = 2432,
        RCE_PageUpRectExtend = 2433,
        RCE_PageDownRectExtend = 2434,
        RCE_StutteredPageUp = 2435,
        RCE_StutteredPageUpExtend = 2436,
        RCE_StutteredPageDown = 2437,
        RCE_StutteredPageDownExtend = 2438,
        RCE_WordLeftEnd = 2439,
        RCE_WordLeftEndExtend = 2440,
        RCE_WordRightEnd = 2441,
        RCE_WordRightEndExtend = 2442,
        RCE_SetWhiteSpaceChars = 2443,
        RCE_SetCharsDefault = 2444,
        RCE_AutoCGetCurrent = 2445,
        RCE_ALLOCATE = 2446,
        RCE_HomeWrap = 2349,
        RCE_HomeWrapExtend = 2450,
        RCE_LineEndWrap = 2451,
        RCE_LineEndWrapExtend = 2452,
        RCE_VCHOMEWrap = 2453,
        RCE_VCHOMEWrapExtend = 2454,
        RCE_LineCopy = 2455,
        RCE_FindColumn = 2456,
        RCE_GetCaretSticky = 2457,
        RCE_SetCaretSticky = 2458,
        RCE_ToggleCaretSticky = 2459,
        RCE_SetWrapVisualFlags = 2460,
        RCE_GetWrapVisualFlags = 2461,
        RCE_SetWrapVisualFlagsLocation = 2462,
        RCE_GetWrapVisualFlagsLocation = 2463,
        RCE_SetWrapStartIndent = 2464,
        RCE_GetWrapStartIndent = 2465,
        RCE_MarkerAddStart = 2466,
        RCE_SetPasteConvertEndings = 2467,
        RCE_GetPasteConvertEndings = 2468,
        RCE_SelectionDuplicate = 2469,
        RCE_SetCaretLineBackAlpha = 2470,
        RCE_GetCaretLineBackAlpha = 2471,
        RCE_SetWrapIndentMode = 2472,
        RCE_GetWrapIndentMode = 2473,
        RCE_MarkerSetAlpha = 2476,
        RCE_GetSelectionAlpha = 2477,
        RCE_SetSelectionAlpha = 2478,
        RCE_GetSelectionEOLFilled = 2479,
        RCE_SetSelectionEOLFilled = 2480,
        RCE_StyleGetForeground = 2481,
        RCE_StyleGetBackground = 2482,
        RCE_STYLEGETBOLD = 2483,
        RCE_STYLEGETITALIC = 2484,
        RCE_STYLEGETSIZE = 2485,
        RCE_STYLEGETFONT = 2486,
        RCE_STYLEGETEOLFILLED = 2487,
        RCE_STYLEGETUNDERLINE = 2488,
        RCE_STYLEGETCASE = 2489,
        RCE_STYLEGETCHARACTERSET = 2490,
        RCE_STYLEGETVISIBLE = 2491,
        RCE_STYLEGETCHANGEABLE = 2492,
        RCE_STYLEGETHOTSPOT = 2493,
        RCE_GETHOTSPOTACTIVEFORE = 2494,
        RCE_GETHOTSPOTACTIVEBACK = 2495,
        RCE_GETHOTSPOTACTIVEUNDERLINE = 2496,
        RCE_GETHOTSPOTSINGLELINE = 2497,
        RCE_BRACEHIGHLIGHTINDICATOR = 2498,
        RCE_BRACEBADLIGHTINDICATOR = 2499,
        RCE_SETINDICATORCURRENT = 2500,
        RCE_GETINDICATORCURRENT = 2501,
        RCE_SETINDICATORVALUE = 2502,
        RCE_GETINDICATORVALUE = 2503,
        RCE_INDICATORFILLRANGE = 2504,
        RCE_INDICATORCLEARRANGE = 2505,
        RCE_INDICATORALLONFOR = 2506,
        RCE_INDICATORVALUEAT = 2507,
        RCE_INDICATORSTART = 2508,
        RCE_INDICATOREND = 2509,
        RCE_INDICSETUNDER = 2510,
        RCE_INDICGETUNDER = 2511,
        RCE_SETCARETSTYLE = 2512,
        RCE_GETCARETSTYLE = 2513,
        RCE_SETPOSITIONCACHE = 2514,
        RCE_GETPOSITIONCACHE = 2515,
        RCE_SETSCROLLWIDTHTRACKING = 2516,
        RCE_GETSCROLLWIDTHTRACKING = 2517,
        RCE_DELWORDRIGHTEND = 2518,
        RCE_COPYALLOWLINE = 2519,
        RCE_GETCHARACTERPOINTER = 2520,
        RCE_INDICSETALPHA = 2523,
        RCE_INDICGETALPHA = 2524,
        RCE_SETEXTRAASCENT = 2525,
        RCE_GETEXTRAASCENT = 2526,
        RCE_SETEXTRADESCENT = 2527,
        RCE_GETEXTRADESCENT = 2528,
        RCE_MARKERSYMBOLDEFINED = 2529,
        RCE_MARGINSETTEXT = 2530,
        RCE_MARGINGETTEXT = 2531,
        RCE_MARGINSETSTYLE = 2532,
        RCE_MARGINGETSTYLE = 2533,
        RCE_MARGINSETSTYLES = 2534,
        RCE_MARGINGETSTYLES = 2535,
        RCE_MARGINTEXTCLEARALL = 2536,
        RCE_MARGINSETSTYLEOFFSET = 2537,
        RCE_MARGINGETSTYLEOFFSET = 2538,
        RCE_SETMARGINOPTIONS = 2539,
        RCE_ANNOTATIONSETTEXT = 2540,
        RCE_ANNOTATIONGETTEXT = 2541,
        RCE_ANNOTATIONSETSTYLE = 2542,
        RCE_ANNOTATIONGETSTYLE = 2543,
        RCE_ANNOTATIONSETSTYLES = 2544,
        RCE_ANNOTATIONGETSTYLES = 2545,
        RCE_ANNOTATIONGETLINES = 2546,
        RCE_ANNOTATIONCLEARALL = 2547,
        RCE_ANNOTATIONSETVISIBLE = 2548,
        RCE_ANNOTATIONGETVISIBLE = 2549,
        RCE_ANNOTATIONSETSTYLEOFFSET = 2550,
        RCE_ANNOTATIONGETSTYLEOFFSET = 2551,
        RCE_RELEASEALLEXTENDEDSTYLES = 2552,
        RCE_ALLOCATEEXTENDEDSTYLES = 2553,
        RCE_SETEMPTYSELECTION = 2556,
        RCE_GETMARGINOPTIONS = 2557,
        RCE_INDICSETOUTLINEALPHA = 2558,
        RCE_INDICGETOUTLINEALPHA = 2559,
        RCE_ADDUNDOACTION = 2560,
        RCE_CHARPOSITIONFROMPOINT = 2561,
        RCE_CHARPOSITIONFROMPOINTCLOSE = 2562,
        RCE_SETMULTIPLESELECTION = 2563,
        RCE_GETMULTIPLESELECTION = 2564,
        RCE_SETADDITIONALSELECTIONTYPING = 2565,
        RCE_GETADDITIONALSELECTIONTYPING = 2566,
        RCE_SETADDITIONALCARETSBLINK = 2567,
        RCE_GETADDITIONALCARETSBLINK = 2568,
        RCE_SCROLLRANGE = 2569,
        RCE_GETSELECTIONS = 2570,
        RCE_CLEARSELECTIONS = 2571,
        RCE_SETSELECTION = 2572,
        RCE_ADDSELECTION = 2573,
        RCE_SETMAINSELECTION = 2574,
        RCE_GETMAINSELECTION = 2575,
        RCE_SETSELECTIONNCARET = 2576,
        RCE_GETSELECTIONNCARET = 2577,
        RCE_SETSELECTIONNANCHOR = 2578,
        RCE_GETSELECTIONNANCHOR = 2579,
        RCE_SETSELECTIONNCARETVIRTUALSPACE = 2580,
        RCE_GETSELECTIONNCARETVIRTUALSPACE = 2581,
        RCE_SETSELECTIONNANCHORVIRTUALSPACE = 2582,
        RCE_GETSELECTIONNANCHORVIRTUALSPACE = 2583,
        RCE_SETSELECTIONNSTART = 2584,
        RCE_GETSELECTIONNSTART = 2585,

        //!
        RCE_SETSELECTIONNEND = 2586,

        //!
        RCE_GETSELECTIONNEND = 2587,

        //!
        RCE_SETRECTANGULARSELECTIONCARET = 2588,

        //!
        RCE_GETRECTANGULARSELECTIONCARET = 2589,

        //!
        RCE_SETRECTANGULARSELECTIONANCHOR = 2590,

        //!
        RCE_GETRECTANGULARSELECTIONANCHOR = 2591,

        //!
        RCE_SETRECTANGULARSELECTIONCARETVIRTUALSPACE = 2592,

        //!
        RCE_GETRECTANGULARSELECTIONCARETVIRTUALSPACE = 2593,

        //!
        RCE_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE = 2594,

        //!
        RCE_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE = 2595,

        //!
        RCE_SETVIRTUALSPACEOPTIONS = 2596,

        //!
        RCE_GETVIRTUALSPACEOPTIONS = 2597,

        //!
        RCE_SETRECTANGULARSELECTIONMODIFIER = 2598,

        //!
        RCE_GETRECTANGULARSELECTIONMODIFIER = 2599,

        //!
        RCE_SETADDITIONALSELFORE = 2600,

        //!
        RCE_SETADDITIONALSELBACK = 2601,

        //!
        RCE_SETADDITIONALSELALPHA = 2602,

        //!
        RCE_GETADDITIONALSELALPHA = 2603,

        //!
        RCE_SETADDITIONALCARETFORE = 2604,

        //!
        RCE_GETADDITIONALCARETFORE = 2605,

        //!
        RCE_ROTATESELECTION = 2606,

        //!
        RCE_SWAPMAINANCHORCARET = 2607,

        //!
        RCE_SETADDITIONALCARETSVISIBLE = 2608,

        //!
        RCE_GETADDITIONALCARETSVISIBLE = 2609,

        //!
        RCE_AUTOCGETCURRENTTEXT = 2610,

        //!
        RCE_SETFONTQUALITY = 2611,

        //!
        RCE_GETFONTQUALITY = 2612,

        //!
        RCE_SETFIRSTVISIBLELINE = 2613,

        //!
        RCE_SETMULTIPASTE = 2614,

        //!
        RCE_GETMULTIPASTE = 2615,

        //!
        RCE_GETTAG = 2616,

        //!
        RCE_CHANGELEXERSTATE = 2617,

        //!
        RCE_CONTRACTEDFOLDNEXT = 2618,

        //!
        RCE_VERTICALCENTRECARET = 2619,

        //!
        RCE_MOVESELECTEDLINESUP = 2620,

        //!
        RCE_MOVESELECTEDLINESDOWN = 2621,

        //!
        RCE_SETIDENTIFIER = 2622,

        //!
        RCE_GETIDENTIFIER = 2623,

        //! This message sets the width of an RGBA image specified by a future
        //! call to RCE_MARKERDEFINERGBAIMAGE or RCE_REGISTERRGBAIMAGE.
        //!
        //! \sa RCE_RGBAIMAGESETHEIGHT, RCE_MARKERDEFINERGBAIMAGE,
        //! RCE_REGISTERRGBAIMAGE.
        RCE_RGBAIMAGESETWIDTH = 2624,

        //! This message sets the height of an RGBA image specified by a future
        //! call to RCE_MARKERDEFINERGBAIMAGE or RCE_REGISTERRGBAIMAGE.
        //!
        //! \sa RCE_RGBAIMAGESETWIDTH, RCE_MARKERDEFINERGBAIMAGE,
        //! RCE_REGISTERRGBAIMAGE.
        RCE_RGBAIMAGESETHEIGHT = 2625,

        //! This message sets the symbol used to draw one of the 32 markers to
        //! an RGBA image.  RGBA images use the SC_MARK_RGBAIMAGE marker
        //! symbol.
        //! \a wParam is the number of the marker.
        //! \a lParam is a pointer to a QImage instance.  Note that in other
        //! ports of Scintilla this is a pointer to raw RGBA image data.
        //!
        //! \sa RCE_MARKERDEFINE, RCE_MARKERDEFINEPIXMAP
        RCE_MARKERDEFINERGBAIMAGE = 2626,

        //! This message takes a copy of an image and registers it so that it
        //! can be refered to by a unique integer identifier.
        //! \a wParam is the image's identifier.
        //! \a lParam is a pointer to a QImage instance.  Note that in other
        //! ports of Scintilla this is a pointer to raw RGBA image data.
        //!
        //! \sa RCE_CLEARREGISTEREDIMAGES, RCE_REGISTERIMAGE
        RCE_REGISTERRGBAIMAGE = 2627,

        //!
        RCE_SCROLLTOSTART = 2628,

        //!
        RCE_SCROLLTOEND = 2629,

        //!
        RCE_SETTECHNOLOGY = 2630,

        //!
        RCE_GETTECHNOLOGY = 2631,

        //!
        RCE_CREATELOADER = 2632,

        //!
        RCE_COUNTCHARACTERS = 2633,

        //!
        RCE_AUTOCSETCASEINSENSITIVEBEHAVIOUR = 2634,

        //!
        RCE_AUTOCGETCASEINSENSITIVEBEHAVIOUR = 2635,

        //!
        RCE_AUTOCSETMULTI = 2636,

        //!
        RCE_AUTOCGETMULTI = 2637,

        //!
        RCE_FINDINDICATORSHOW = 2640,

        //!
        RCE_FINDINDICATORFLASH = 2641,

        //!
        RCE_FINDINDICATORHIDE = 2642,

        //!
        RCE_GETRANGEPOINTER = 2643,

        //!
        RCE_GETGAPPOSITION = 2644,

        //!
        RCE_DELETERANGE = 2645,

        //!
        RCE_GETWORDCHARS = 2646,

        //!
        RCE_GETWHITESPACECHARS = 2647,

        //!
        RCE_SETPUNCTUATIONCHARS = 2648,

        //!
        RCE_GETPUNCTUATIONCHARS = 2649,

        //!
        RCE_GETSELECTIONEMPTY = 2650,

        //!
        RCE_RGBAIMAGESETSCALE = 2651,

        //!
        RCE_VCHOMEDISPLAY = 2652,

        //!
        RCE_VCHOMEDISPLAYEXTEND = 2653,

        //!
        RCE_GETCARETLINEVISIBLEALWAYS = 2654,

        //!
        RCE_SETCARETLINEVISIBLEALWAYS = 2655,

        //!
        RCE_SETLINEENDTYPESALLOWED = 2656,

        //!
        RCE_GETLINEENDTYPESALLOWED = 2657,

        //!
        RCE_GETLINEENDTYPESACTIVE = 2658,

        //!
        RCE_AUTOCSETORDER = 2660,

        //!
        RCE_AUTOCGETORDER = 2661,

        //!
        RCE_FOLDALL = 2662,

        //!
        RCE_SETAUTOMATICFOLD = 2663,

        //!
        RCE_GETAUTOMATICFOLD = 2664,

        //!
        RCE_SETREPRESENTATION = 2665,

        //!
        RCE_GETREPRESENTATION = 2666,

        //!
        RCE_CLEARREPRESENTATION = 2667,

        //!
        RCE_SETMOUSESELECTIONRECTANGULARSWITCH = 2668,

        //!
        RCE_GETMOUSESELECTIONRECTANGULARSWITCH = 2669,

        //!
        RCE_POSITIONRELATIVE = 2670,

        //!
        RCE_DROPSELECTIONN = 2671,

        //!
        RCE_CHANGEINSERTION = 2672,

        //!
        RCE_GETPHASESDRAW = 2673,

        //!
        RCE_SETPHASESDRAW = 2674,

        //!
        RCE_CLEARTABSTOPS = 2675,

        //!
        RCE_ADDTABSTOP = 2676,

        //!
        RCE_GETNEXTTABSTOP = 2677,

        //!
        RCE_GETIMEINTERACTION = 2678,

        //!
        RCE_SETIMEINTERACTION = 2679,

        //!
        RCE_INDICSETHOVERSTYLE = 2680,

        //!
        RCE_INDICGETHOVERSTYLE = 2681,

        //!
        RCE_INDICSETHOVERFORE = 2682,

        //!
        RCE_INDICGETHOVERFORE = 2683,

        //!
        RCE_INDICSETFLAGS = 2684,

        //!
        RCE_INDICGETFLAGS = 2685,

        //!
        RCE_SETTARGETRANGE = 2686,

        //!
        RCE_GETTARGETTEXT = 2687,

        //!
        RCE_MULTIPLESELECTADDNEXT = 2688,

        //!
        RCE_MULTIPLESELECTADDEACH = 2689,

        //!
        RCE_TARGETWHOLEDOCUMENT = 2690,

        //!
        RCE_ISRANGEWORD = 2691,

        //!
        RCE_SETIDLESTYLING = 2692,

        //!
        RCE_GETIDLESTYLING = 2693,

        //!
        RCE_MULTIEDGEADDLINE = 2694,

        //!
        RCE_MULTIEDGECLEARALL = 2695,

        //!
        RCE_SETMOUSEWHEELCAPTURES = 2696,

        //!
        RCE_GETMOUSEWHEELCAPTURES = 2697,

        //!
        RCE_GETTABDRAWMODE = 2698,

        //!
        RCE_SETTABDRAWMODE = 2699,

        //!
        RCE_TOGGLEFOLDSHOWTEXT = 2700,

        //!
        RCE_FOLDDISPLAYTEXTSETSTYLE = 2701,

        //!
        RCE_SETACCESSIBILITY = 2702,

        //!
        RCE_GETACCESSIBILITY = 2703,

        //!
        RCE_GETCARETLINEFRAME = 2704,

        //!
        RCE_SETCARETLINEFRAME = 2705,

        //!
        RCE_STARTRECORD = 3001,

        //!
        RCE_STOPRECORD = 3002,

        //! This message sets the number of the lexer to use for syntax
        //! styling.
        //! \a wParam is the number of the lexer and is one of the SCLEX_*
        //! values.
        RCE_SETLEXER = 4001,

        //! This message returns the number of the lexer being used for syntax
        //! styling.
        RCE_GETLEXER = 4002,

        //!
        RCE_COLOURISE = 4003,

        //!
        RCE_SETPROPERTY = 4004,

        //!
        RCE_SETKEYWORDS = 4005,

        //! This message sets the name of the lexer to use for syntax styling.
        //! \a wParam is unused.
        //! \a lParam is the name of the lexer.
        RCE_SETLEXERLANGUAGE = 4006,

        //!
        RCE_LOADLEXERLIBRARY = 4007,

        //!
        RCE_GETPROPERTY = 4008,

        //!
        RCE_GETPROPERTYEXPANDED = 4009,

        //!
        RCE_GETPROPERTYINT = 4010,

        //!
        RCE_GETSTYLEBITSNEEDED = 4011,

        //!
        RCE_GETLEXERLANGUAGE = 4012,

        //!
        RCE_PRIVATELEXERCALL = 4013,

        //!
        RCE_PROPERTYNAMES = 4014,

        //!
        RCE_PROPERTYTYPE = 4015,

        //!
        RCE_DESCRIBEPROPERTY = 4016,

        //!
        RCE_DESCRIBEKEYWORDSETS = 4017,

        //!
        RCE_GETLINEENDTYPESSUPPORTED = 4018,

        //!
        RCE_ALLOCATESUBSTYLES = 4020,

        //!
        RCE_GETSUBSTYLESSTART = 4021,

        //!
        RCE_GETSUBSTYLESLENGTH = 4022,

        //!
        RCE_GETSTYLEFROMSUBSTYLE = 4027,

        //!
        RCE_GETPRIMARYSTYLEFROMSTYLE = 4028,

        //!
        RCE_FREESUBSTYLES = 4023,

        //!
        RCE_SETIDENTIFIERS = 4024,

        //!
        RCE_DISTANCETOSECONDARYSTYLES = 4025,

        //!
        RCE_GETSUBSTYLEBASES = 4026,

        //!
        RCE_GETLINECHARACTERINDEX = 2710,

        //!
        RCE_ALLOCATELINECHARACTERINDEX = 2711,

        //!
        RCE_RELEASELINECHARACTERINDEX = 2712,

        //!
        RCE_LINEFROMINDEXPOSITION = 2713,

        //!
        RCE_INDEXPOSITIONFROMLINE = 2714,

        //!
        RCE_COUNTCODEUNITS = 2715,

        //!
        RCE_POSITIONRELATIVECODEUNITS = 2716,

        //!
        RCE_GETNAMEDSTYLES = 4029,

        //!
        RCE_NAMEOFSTYLE = 4030,

        //!
        RCE_TAGSOFSTYLE = 4031,

        //!
        RCE_DESCRIPTIONOFSTYLE = 4032,

        //!
        RCE_GETMOVEEXTENDSSELECTION = 2706,

        //!
        RCE_SETCOMMANDEVENTS = 2717,

        //!
        RCE_GETCOMMANDEVENTS = 2718,

        //!
        RCE_GETDOCUMENTOPTIONS = 2379,
    };
};
}

#endif // RCE_BASE_H