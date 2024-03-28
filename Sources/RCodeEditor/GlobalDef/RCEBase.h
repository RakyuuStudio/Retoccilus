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
/// \sa QScintilla Repository, this class based on QScintilla
class RCE_EXPORT RCEBase : public QAbstractScrollArea {
Q_OBJECT
public:
    enum class RCEGlobalSignals {
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
        RCE_StyleSetCharacterSet = 2066,
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
        RCE_SetSel = 2160,
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
        RCE_EnsureVisibleEnfoRCEPolicy = 2234,
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
        RCE_StyleGetBold = 2483,
        RCE_StyleGetItalic = 2484,
        RCE_StyleGetSize = 2485,
        RCE_StyleGetFont = 2486,
        RCE_StyleGetEOLFilled = 2487,
        RCE_StyleGetUnderline = 2488,
        RCE_StyleGetCase = 2489,
        RCE_StyleGetCharacterSet = 2490,
        RCE_StyleGetVisible = 2491,
        RCE_StyleGetChangeable = 2492,
        RCE_StyleGetHotspot = 2493,
        RCE_GetHotspotActiveFore = 2494,
        RCE_GetHotspotActiveBack = 2495,
        RCE_GetHotspotActiveUnderline = 2496,
        RCE_GetHotspotSingleLine = 2497,
        RCE_BraceHighlightIndicator = 2498,
        RCE_BraceBadlightIndicator = 2499,
        RCE_SetIndicatorCurrent = 2500,
        RCE_GetIndicatorCurrent = 2501,
        RCE_SetIndicatorValue = 2502,
        RCE_GetIndicatorValue = 2503,
        RCE_IndicatorFillRange = 2504,
        RCE_IndicatorClearRange = 2505,
        RCE_IndicatorAllOnFor = 2506,
        RCE_IndicatorValueAt = 2507,
        RCE_IndicatorStart = 2508,
        RCE_IndicatorEnd = 2509,
        RCE_IndicSetUnder = 2510,
        RCE_IndicGetUnder = 2511,
        RCE_SetCaretStyle = 2512,
        RCE_GetCaretStyle = 2513,
        RCE_SetPositionCache = 2514,
        RCE_GetPositionCache = 2515,
        RCE_SetScrollWidthTracking = 2516,
        RCE_GetScrollWidthTracking = 2517,
        RCE_DelWordRightEnd = 2518,
        RCE_CopyAllowLine = 2519,
        RCE_GetCharacterPointer = 2520,
        RCE_IndicSetAlpha = 2523,
        RCE_IndicGetAlpha = 2524,
        RCE_SetExtraAscent = 2525,
        RCE_GetExtraAscent = 2526,
        RCE_SetExtraDescent = 2527,
        RCE_GetExtraDescent = 2528,
        RCE_MarkerSymbolDefined = 2529,
        RCE_MarginSetText = 2530,
        RCE_MarginGetText = 2531,
        RCE_MarginSetStyle = 2532,
        RCE_MarginGetStyle = 2533,
        RCE_MarginSetStyles = 2534,
        RCE_MarginGetStyles = 2535,
        RCE_MarginTextClearAll = 2536,
        RCE_MarginSetStyleOffset = 2537,
        RCE_MarginGetStyleOffset = 2538,
        RCE_SetMarginOptions = 2539,
        RCE_AnnotationSetText = 2540,
        RCE_AnnotationGetText = 2541,
        RCE_AnnotationSetStyle = 2542,
        RCE_AnnotationGetStyle = 2543,
        RCE_AnnotationSetStyles = 2544,
        RCE_AnnotationGetStyles = 2545,
        RCE_AnnotationGetLines = 2546,
        RCE_AnnotationClearAll = 2547,
        RCE_AnnotationSetVisible = 2548,
        RCE_AnnotationGetVisible = 2549,
        RCE_AnnotationSetStyleOffset = 2550,
        RCE_AnnotationGetStyleOffset = 2551,
        RCE_ReleaseAllExtendedStyles = 2552,
        RCE_AllocateExtendedStyles = 2553,
        RCE_SetEmptySelection = 2556,
        RCE_GetMarginOptions = 2557,
        RCE_IndicSetOutlineAlpha = 2558,
        RCE_IndicGetOutlineAlpha = 2559,
        RCE_AddUndoAction = 2560,
        RCE_CharPositionFromPoint = 2561,
        RCE_CharPositionFromPointClose = 2562,
        RCE_SetMultipleSelection = 2563,
        RCE_GetMultipleSelection = 2564,
        RCE_SetAdditionalSelectionTyping = 2565,
        RCE_GetAdditionalSelectionTyping = 2566,
        RCE_SetAdditionalCaretsBlink = 2567,
        RCE_GetAdditionalCaretsBlink = 2568,
        RCE_ScrollRange = 2569,
        RCE_GetSelections = 2570,
        RCE_ClearSelections = 2571,
        RCE_SetSelection = 2572,
        RCE_AddSelection = 2573,
        RCE_SetMainSelection = 2574,
        RCE_GetMainSelection = 2575,
        RCE_SetSelectionNCaret = 2576,
        RCE_GetSelectionNCaret = 2577,
        RCE_SetSelectionNAnchor = 2578,
        RCE_GetSelectionNAnchor = 2579,
        RCE_SetSelectionNCaretVirtualSpace = 2580,
        RCE_GetSelectionNCaretVirtualSpace = 2581,
        RCE_SetSelectionNAnchorVirtualSpace = 2582,
        RCE_GetSelectionNAnchorVirtualSpace = 2583,
        RCE_SetSelectionNStart = 2584,
        RCE_GetSelectionNStart = 2585,

        RCE_SetSelectionNEnd = 2586,
        RCE_GetSelectionNEnd = 2587,
        RCE_SETRECTANGULARSELECTIONCARET = 2588,
        RCE_GETRECTANGULARSELECTIONCARET = 2589,
        RCE_SETRECTANGULARSELECTIONANCHOR = 2590,
        RCE_GETRECTANGULARSELECTIONANCHOR = 2591,
        RCE_SETRECTANGULARSELECTIONCARETVIRTUALSPACE = 2592,
        RCE_GETRECTANGULARSELECTIONCARETVIRTUALSPACE = 2593,
        RCE_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE = 2594,
        RCE_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE = 2595,
        RCE_SETVIRTUALSPACEOPTIONS = 2596,
        RCE_GETVIRTUALSPACEOPTIONS = 2597,
        RCE_SETRECTANGULARSELECTIONMODIFIER = 2598,
        RCE_GETRECTANGULARSELECTIONMODIFIER = 2599,
        RCE_SETADDITIONALSELFORE = 2600,
        RCE_SETADDITIONALSELBACK = 2601,
        RCE_SETADDITIONALSELALPHA = 2602,
        RCE_GETADDITIONALSELALPHA = 2603,
        RCE_SETADDITIONALCARETFORE = 2604,
        RCE_GETADDITIONALCARETFORE = 2605,
        RCE_ROTATESELECTION = 2606,
        RCE_SWAPMAINANCHORCARET = 2607,
        RCE_SETADDITIONALCARETSVISIBLE = 2608,
        RCE_GETADDITIONALCARETSVISIBLE = 2609,
        RCE_AUTOCGETCURRENTTEXT = 2610,
        RCE_SETFONTQUALITY = 2611,
        RCE_GETFONTQUALITY = 2612,
        RCE_SETFIRSTVISIBLELINE = 2613,
        RCE_SETMULTIPASTE = 2614,
        RCE_GETMULTIPASTE = 2615,
        RCE_GETTAG = 2616,
        RCE_CHANGELEXERSTATE = 2617,
        RCE_CONTRACTEDFOLDNEXT = 2618,
        RCE_VERTICALCENTRECARET = 2619,
        RCE_MOVESELECTEDLINESUP = 2620,
        RCE_MOVESELECTEDLINESDOWN = 2621,
        RCE_SETIDENTIFIER = 2622,
        RCE_GETIDENTIFIER = 2623,
        RCE_RGBAIMAGESETWIDTH = 2624,
        RCE_RGBAIMAGESETHEIGHT = 2625,
        RCE_MARKERDEFINERGBAIMAGE = 2626,
        RCE_REGISTERRGBAIMAGE = 2627,
        RCE_SCROLLTOSTART = 2628,
        RCE_SCROLLTOEND = 2629,
        RCE_SETTECHNOLOGY = 2630,
        RCE_GETTECHNOLOGY = 2631,
        RCE_CREATELOADER = 2632,
        RCE_COUNTCHARACTERS = 2633,
        RCE_AUTOCSETCASEINSENSITIVEBEHAVIOUR = 2634,
        RCE_AUTOCGETCASEINSENSITIVEBEHAVIOUR = 2635,
        RCE_AUTOCSETMULTI = 2636,
        RCE_AUTOCGETMULTI = 2637,
        RCE_FINDINDICATORSHOW = 2640,
        RCE_FINDINDICATORFLASH = 2641,
        RCE_FINDINDICATORHIDE = 2642,
        RCE_GETRANGEPOINTER = 2643,
        RCE_GETGAPPOSITION = 2644,
        RCE_DELETERANGE = 2645,
        RCE_GETWORDCHARS = 2646,
        RCE_GETWHITESPACECHARS = 2647,
        RCE_SETPUNCTUATIONCHARS = 2648,
        RCE_GETPUNCTUATIONCHARS = 2649,
        RCE_GETSELECTIONEMPTY = 2650,
        RCE_RGBAIMAGESETSCALE = 2651,
        RCE_VCHOMEDISPLAY = 2652,
        RCE_VCHOMEDISPLAYEXTEND = 2653,
        RCE_GETCARETLINEVISIBLEALWAYS = 2654,
        RCE_SETCARETLINEVISIBLEALWAYS = 2655,
        RCE_SETLINEENDTYPESALLOWED = 2656,
        RCE_GETLINEENDTYPESALLOWED = 2657,
        RCE_GETLINEENDTYPESACTIVE = 2658,
        RCE_AUTOCSETORDER = 2660,
        RCE_AUTOCGETORDER = 2661,
        RCE_FOLDALL = 2662,
        RCE_SETAUTOMATICFOLD = 2663,
        RCE_GETAUTOMATICFOLD = 2664,
        RCE_SETREPRESENTATION = 2665,
        RCE_GETREPRESENTATION = 2666,
        RCE_CLEARREPRESENTATION = 2667,
        RCE_SETMOUSESELECTIONRECTANGULARSWITCH = 2668,
        RCE_GETMOUSESELECTIONRECTANGULARSWITCH = 2669,
        RCE_POSITIONRELATIVE = 2670,
        RCE_DROPSELECTIONN = 2671,
        RCE_CHANGEINSERTION = 2672,
        RCE_GETPHASESDRAW = 2673,
        RCE_SETPHASESDRAW = 2674,
        RCE_CLEARTABSTOPS = 2675,
        RCE_ADDTABSTOP = 2676,
        RCE_GETNEXTTABSTOP = 2677,
        RCE_GETIMEINTERACTION = 2678,
        RCE_SETIMEINTERACTION = 2679,
        RCE_INDICSETHOVERSTYLE = 2680,
        RCE_INDICGETHOVERSTYLE = 2681,
        RCE_INDICSETHOVERFORE = 2682,
        RCE_INDICGETHOVERFORE = 2683,
        RCE_INDICSETFLAGS = 2684,
        RCE_INDICGETFLAGS = 2685,
        RCE_SETTARGETRANGE = 2686,
        RCE_GETTARGETTEXT = 2687,
        RCE_MULTIPLESELECTADDNEXT = 2688,
        RCE_MULTIPLESELECTADDEACH = 2689,
        RCE_SetRectangularSelectionCaret = 2588,
        RCE_GetRectangularSelectionCaret = 2589,
        RCE_SetRectangularSelectionAnchor = 2590,
        RCE_GetRectangularSelectionAnchor = 2591,
        RCE_SetRectangularSelectionCaretVirtualSpace = 2592,
        RCE_GetRectangularSelectionCaretVirtualSpace = 2593,
        RCE_SetRectangularSelectionAnchorVirtualSpace = 2594,
        RCE_GetRectangularSelectionAnchorVirtualSpace = 2595,
        RCE_SetVirtualSpaceOptions = 2596,
        RCE_GetVirtualSpaceOptions = 2597,
        RCE_SetRectangularSelectionModifier = 2598,
        RCE_GetRectangularSelectionModifier = 2599,
        RCE_SetAdditionalSelFore = 2600,
        RCE_SetAdditionalSelBack = 2601,
        RCE_SetAdditionalSelAlpha = 2602,
        RCE_GetAdditionalSelAlpha = 2603,
        RCE_SetAdditionalCaretFore = 2604,
        RCE_GetAdditionalCaretFore = 2605,
        RCE_RotateSelection = 2606,
        RCE_SwapMainAnchorCaret = 2607,
        RCE_SetAdditionalCaretsVisible = 2608,
        RCE_GetAdditionalCaretsVisible = 2609,
        RCE_AutoCGetCurrentText = 2610,
        RCE_SetFontQuality = 2611,
        RCE_GetFontQuality = 2612,
        RCE_SetFirstVisibleLine = 2613,
        RCE_SetMultiPaste = 2614,
        RCE_GetMultiPaste = 2615,
        RCE_GetTag = 2616,
        RCE_ChangeLexerState = 2617,
        RCE_ContractedFoldNext = 2618,
        RCE_VerticalCentreCaret = 2619,
        RCE_MoveSelectedLinesUp = 2620,
        RCE_MoveSelectedLinesDown = 2621,
        RCE_SetIdentifier = 2622,
        RCE_GetIdentifier = 2623,
        RCE_RgbaImageSetWidth = 2624,
        RCE_RgbaImageSetHeight = 2625,
        RCE_MarkerDefineRgbaImage = 2626,
        RCE_RegisterRgbaImage = 2627,
        RCE_ScrollToStart = 2628,
        RCE_ScrollToEnd = 2629,
        RCE_SetTechnology = 2630,
        RCE_GetTechnology = 2631,
        RCE_CreateLoader = 2632,
        RCE_CountCharacters = 2633,
        RCE_AutoCSetCaseInsensitiveBehaviour = 2634,
        RCE_AutoCGetCaseInsensitiveBehaviour = 2635,
        RCE_AutoCSetMulti = 2636,
        RCE_AutoCGetMulti = 2637,
        RCE_FindIndicatorShow = 2640,
        RCE_FindIndicatorFlash = 2641,
        RCE_FindIndicatorHide = 2642,
        RCE_GetRangePointer = 2643,
        RCE_GetGapPosition = 2644,
        RCE_DeleteRange = 2645,
        RCE_GetWordChars = 2646,
        RCE_GetWhitespaceChars = 2647,
        RCE_SetPunctuationChars = 2648,
        RCE_GetPunctuationChars = 2649,
        RCE_GetSelectionEmpty = 2650,
        RCE_RgbaImageSetScale = 2651,
        RCE_VCHomeDisplay = 2652,
        RCE_VCHomeDisplayExtend = 2653,
        RCE_GetCaretLineVisibleAlways = 2654,
        RCE_SetCaretLineVisibleAlways = 2655,
        RCE_SetLineEndTypesAllowed = 2656,
        RCE_GetLineEndTypesAllowed = 2657,
        RCE_GetLineEndTypesActive = 2658,
        RCE_AutoCSetOrder = 2660,
        RCE_AutoCGetOrder = 2661,
        RCE_FoldAll = 2662,
        RCE_SetAutomaticFold = 2663,
        RCE_GetAutomaticFold = 2664,
        RCE_SetRepresentation = 2665,
        RCE_GetRepresentation = 2666,
        RCE_ClearRepresentation = 2667,
        RCE_SetMouseSelectionRectangularSwitch = 2668,
        RCE_GetMouseSelectionRectangularSwitch = 2669,
        RCE_PositionRelative = 2670,
        RCE_DropSelectionN = 2671,
        RCE_ChangeInsertion = 2672,
        RCE_GetPhasesDraw = 2673,
        RCE_SetPhasesDraw = 2674,
        RCE_ClearTabStops = 2675,
        RCE_AddTabStop = 2676,
        RCE_GetNextTabStop = 2677,
        RCE_GetImeInteraction = 2678,
        RCE_SetImeInteraction = 2679,
        RCE_IndicSetHoverStyle = 2680,
        RCE_IndicGetHoverStyle = 2681,
        RCE_IndicSetHoverFore = 2682,
        RCE_IndicGetHoverFore = 2683,
        RCE_IndicSetFlags = 2684,
        RCE_IndicGetFlags = 2685,
        RCE_SetTargetRange = 2686,
        RCE_GetTargetText = 2687,
        RCE_MultipleSelectAddNext = 2688,
        RCE_MultipleSelectAddEach = 2689,
        RCE_TargetWholeDocument = 2690,
        RCE_IsRangeWord = 2691,
        RCE_SetIdleStyling = 2692,
        RCE_GetIdleStyling = 2693,
        RCE_MultiEdgeAddLine = 2694,
        RCE_MultiEdgeClearAll = 2695,
        RCE_SetMouseWheelCaptures = 2696,
        RCE_GetMouseWheelCaptures = 2697,
        RCE_GetTabDrawMode = 2698,
        RCE_SetTabDrawMode = 2699,
        RCE_ToggleFoldShowText = 2700,
        RCE_FoldDisplayTextSetStyle = 2701,
        RCE_SetAccessibility = 2702,
        RCE_GetAccessibility = 2703,
        RCE_GetCaretLineFrame = 2704,
        RCE_SetCaretLineFrame = 2705,
        RCE_StartRecord = 3001,
        RCE_StopRecord = 3002,
        RCE_SetLexer = 4001,
        RCE_GetLexer = 4002,
        RCE_Colourise = 4003,
        RCE_SetProperty = 4004,
        RCE_SetKeywords = 4005,
        RCE_SetLexerLanguage = 4006,
        RCE_LoadLexerLibrary = 4007,
        RCE_GetProperty = 4008,
        RCE_GetPropertyExpanded = 4009,
        RCE_GetPropertyInt = 4010,
        RCE_GetStyleBitsNeeded = 4011,
        RCE_GetLexerLanguage = 4012,
        RCE_PrivateLexerCall = 4013,
        RCE_PropertyNames = 4014,
        RCE_PropertyType = 4015,
        RCE_DescribeProperty = 4016,
        RCE_DescribeKeywordSets = 4017,
        RCE_GetLineEndTypesSupported = 4018,
        RCE_AllocateSubStyles = 4020,
        RCE_GetSubStylesStart = 4021,
        RCE_GetSubStylesLength = 4022,
        RCE_GetStyleFromSubStyle = 4027,
        RCE_GetPrimaryStyleFromStyle = 4028,
        RCE_FreeSubStyles = 4023,
        RCE_SetIdentifiers = 4024,
        RCE_DistanceToSecondaryStyles = 4025,
        RCE_GetSubStyleBases = 4026,
        RCE_GetLineCharacterIndex = 2710,
        RCE_AllocateLineCharacterIndex = 2711,
        RCE_ReleaseLineCharacterIndex = 2712,
        RCE_LineFromIndexPosition = 2713,
        RCE_IndexPositionFromLine = 2714,
        RCE_CountCodeUnits = 2715,
        RCE_PositionRelativeCodeUnits = 2716,
        RCE_GetNamedStyles = 4029,
        RCE_NameOfStyle = 4030,
        RCE_TagsOfStyle = 4031,
        RCE_DescriptionOfStyle = 4032,
        RCE_GetMoveExtendsSelection = 2706,
        RCE_SetCommandEvents = 2717,
        RCE_GetCommandEvents = 2718,
        RCE_GetDocumentOptions = 2379,
    };
    enum class ACSignals {
        RAC_Fillup = 1,
        RAC_DoubleClick= 2,
        RAC_Tab = 3,
        RAC_NewLine = 4,
        RAC_Command = 5,
    };
    enum class AlphaOpacitySignals {
        RAO_AlphaOpacity_Transparent = 0,
        RAO_AlphaOpacity_Opaque = 255,
        RAO_AlphaOpacity_NoAlpha = 256 //Solid
    };
    enum class CaretSticky {
        RCE_CaretSticky_Off = 0,
        RCE_CaretSticky_On = 1,
        RCE_CaretSticky_Whitespace = 2
    };
    enum class DocumentOption {
        RCE_DO_Default = 0x0000,
        RCE_DO_StylesNone = 0x0001,
        RCE_DO_TextLarge = 0x0002
    };
    enum class EffectQuality {
        RCE_EQ_Mask = 0x0f,
        RCE_EQ_Default = 0,
        RCE_EQ_NotAntiAliased = 1,
        RCE_EQ_Aliased = 2,
        RCE_EQ_LCDOptimized = 3
    };
    enum class IdleStyling {
        RCIS_None = 0,
        RCIS_ToVisible = 1,
        RCIS_AfterVisible = 2,
        RCIS_All = 3,
    };
    enum class IMEWindowed {
        RCIW_Windowed = 0,
        RCIW_Inline = 1,
    };
    enum class LineCharacterIndex {
        RCICI_None = 0,
        RCICI_Utf32 = 1,
        RCICI_Utf16 = 2,
    };
    enum class MarginOption {
        RCMO_None = 0x00,
        RCMO_SubLineSelect = 0x01
    };
    enum class MultiAutoC {
        RCMA_Once = 0,
        RCMA_Each = 1
    };
    enum class MultiPaste {
        RCMP_Once = 0,
        RCMP_Each = 1
    };
    enum class PopUp {
        RCPU_Never = 0,
        RCPU_All = 1,
        RCPU_Text = 2,
    };
    enum class SelectionModes {
        RCS_Stream = 0,
        RCS_Rectangle = 1,
        RCS_Lines = 2,
        RCS_Thin = 3
    };
    enum class RStatus {
        RCRS_OK = 0,
        RCRS_Failure = 1,
        RCRS_BadAllocation = 2,
        RCRS_WarnStart = 1000,
        RCRS_WarnRegularExp = 1001,
    };
    enum class Type {
        RCT_Boolean = 0,
        RCT_Integer = 1,
        RCT_String = 2
    };
    enum class UpdateObj {
        RCUO_Content = 0x01,
        RCUO_Selection = 0x02,
        RCUO_VerticalScroll = 0x04,
        RCUO_HorizontalScroll = 0x08
    };
    enum class WrapVisualFlag {
        RCE_WVF_None = 0x0000,
        RCE_WVF_End = 0x0001,
        RCE_WVF_Start = 0x0002,
        RCE_WVF_Margin = 0x0004
    };
    enum class WrapVisualFlagLoc {
        RCEWVFL_Default = 0x0000,
        RCEWVFL_EndByText = 0x0001,
        RCEWVFL_StartByText = 0x0002
    };
    enum class VisualFlag {
        RCVF_LongArrow = 0,
        RCVF_StrikeOut = 1,
    };
    enum class WrapType {
        RCWT_None = 0,
        RCWT_RectangularSelection = 1,
        RCWT_UserAccessible = 2,
        RCWT_NoWrapLineStart = 4,
    };
    enum class WrapVisualFlagInvisible {
        RCWCFI_Invisible = 0,
        RCWCFI_InvisibleAlways = 1,
        RCWCFI_VisibleAfterIndent = 2,
        RCWCFI_VisibleOnlyIndent = 3,
    };
    enum class EOFType {
        RCET_CRLF = 0,
        RCET_CR = 1,
        RCET_LF = 2
    };
    enum class CP {
        RCCP_DBCS = 1,
        RCCP_UTF8 = 65001
    };
    enum class MarkerSymbols {
        RCMS_Circle = 0,
        RCMS_RoundRect = 1,
        RCMS_Arrow = 2,
        RCMS_SmallRect = 3,
        RCMS_ShortArrow = 4,
        RCMS_EmptyInvisible = 5,
        RCMS_ArrowDown = 6,
        RCMS_Minus = 7,
        RCMS_Plus = 8,
        RCMS_MarkVerticalLine = 9,
        RCMS_BLeftCorner = 10,
        RCMS_TCorner = 11,
        RCMS_BoxPlus = 12,
        RCMS_BoxPlusConnected = 13,
        RCMS_BoxMinus = 14,
        RCMS_BoxMinusConnected = 15,
        RCMS_LCornerCurve = 16,
        RCMS_TCornerCurve = 17,
        RCMS_CirclePlus = 18,
        RCMS_CirclePlusConnected = 19,
        RCMS_CircleMinus = 20,
        RCMS_CircleMinusConnected = 21,
        RCMS_MarkBg = 22,
        RCMS_DotDotDot = 23,
        RCMS_MarkArrows = 24,
        RCMS_MarkPixmap = 25,
        RCMS_FullRect = 26,
        RCMS_LeftRect = 27,
        RCMS_Available = 28,
        RCMS_Underline = 29,
        RCMS_RGBAlphaImage = 30,
        RCMS_Bookmark = 31,
        RCMS_MarkCharacter = 10000
    };
    enum class MarkNum : std::uint64_t {
        RCMN_FolderEnd = 25,
        RCMN_FolderEndMid = 26,
        RCMN_FolderMidTail = 27,
        RCMN_FolderTail = 28,
        RCMN_FolderSub = 29,
        RCMN_Folder = 30,
        RCMN_FolderOpen = 31,
        RCMN_MaskFolders = 0xfe000000
    };
    enum class MarginContent {
        RCMC_Symbols = 0,
        RCMC_Number = 1,
        RCMC_Back = 2,
        RCMC_Fore = 3,
        RCMC_Text = 4,
        RCMC_RText = 5,
        RCMC_Color = 6,
    };
    enum class StyleType {
        RCST_Default = 32,
        RCST_LineNumber = 33,
        RCST_BraceLight = 34,
        RCST_BraceBad = 35,
        RCST_ControlChar = 36,
        RCST_IndentGuide = 37,
        RCST_CallTip = 38,
        RCST_FoldDisplayText = 39,
        RCST_LastPreDefined = 39,
        RCST_Max = 255
    };
    enum class Charset {
        RCCS_ANSI = 0,
        RCCS_DEFAULT = 1,
        RCCS_BALTIC = 186,
        RCCS_CHINESEBIG5 = 136,
        RCCS_EASTEUROPE = 238,
        RCCS_GB2312 = 134,
        RCCS_GREEK = 161,
        RCCS_HANGUL = 129,
        RCCS_MAC = 77,
        RCCS_OEM = 255,
        RCCS_RUSSIAN = 204,
        RCCS_OEM866 = 866,
        RCCS_CYRILLIC = 1251,
        RCCS_SHIFTJIS = 128,
        RCCS_SYMBOL = 2,
        RCCS_TURKISH = 162,
        RCCS_JOHAB = 130,
        RCCS_HEBREW = 177,
        RCCS_ARABIC = 178,
        RCCS_VIETNAMESE = 163,
        RCCS_THAI = 222,
        RCCS_8859_15 = 1000
    };
    enum class Cases {
        RCCA_Mixed = 0,
        RCCA_Upper = 1,
        RCCA_Lower = 2,
        RCCA_Camel = 3,
    };
    enum class IndentationGuides {
        RCIV_None = 0,
        RCIV_Real = 1,
        RCIV_LookForward = 2,
        RCIV_LookBoth = 3,
    };
    enum class Indic {
        INDIC_PLAIN = 0,
        INDIC_SQUIGGLE = 1,
        INDIC_TT = 2,
        INDIC_DIAGONAL = 3,
        INDIC_STRIKE = 4,
        INDIC_HIDDEN = 5,
        INDIC_BOX = 6,
        INDIC_ROUNDBOX = 7,
        INDIC_STRAIGHTBOX = 8,
        INDIC_DASH = 9,
        INDIC_DOTS = 10,
        INDIC_SQUIGGLELOW = 11,
        INDIC_DOTBOX = 12,
        INDIC_SQUIGGLEPIXMAP = 13,
        INDIC_COMPOSITIONTHICK = 14,
        INDIC_COMPOSITIONTHIN = 15,
        INDIC_FULLBOX = 16,
        INDIC_TEXTFORE = 17,
        INDIC_POINT = 18,
        INDIC_POINTCHARACTER = 19,
        INDIC_GRADIENT = 20,
        INDIC_GRADIENTCENTRE = 21,

        INDIC_IME = 32,
        INDIC_IME_MAX = 35,

        INDIC_CONTAINER = 8,
        INDIC_MAX = 35,
        INDIC0_MASK = 0x20,
        INDIC1_MASK = 0x40,
        INDIC2_MASK = 0x80,
        INDICS_MASK = 0xe0,

        RC_INDICVALUEBIT = 0x01000000,
        RC_INDICVALUEMASK = 0x00ffffff,
        RC_INDICFLAG_VALUEBEFORE = 1,
    };
    enum class Print{
        SC_PRINT_NORMAL = 0,
        SC_PRINT_INVERTLIGHT = 1,
        SC_PRINT_BLACKONWHITE = 2,
        SC_PRINT_COLOURONWHITE = 3,
        SC_PRINT_COLOURONWHITEDEFAULTBG = 4,
        SC_PRINT_SCREENCOLOURS = 5,
    };
    enum class Find {
        SCFIND_WHOLEWORD = 2,
        SCFIND_MATCHCASE = 4,
        SCFIND_WORDSTART = 0x00100000,
        SCFIND_REGEXP = 0x00200000,
        SCFIND_POSIX = 0x00400000,
        SCFIND_CXX11REGEX = 0x00800000,
    };
    enum class FoldDisplay{
        SC_FOLDDISPLAYTEXT_HIDDEN = 0,
        SC_FOLDDISPLAYTEXT_STANDARD = 1,
        SC_FOLDDISPLAYTEXT_BOXED = 2,
    };
    enum class FoldLevel {
        SC_FOLDLEVELBASE = 0x00400,
        SC_FOLDLEVELWHITEFLAG = 0x01000,
        SC_FOLDLEVELHEADERFLAG = 0x02000,
        SC_FOLDLEVELNUMBERMASK = 0x00fff
    };
    enum
    {
        SC_FOLDFLAG_LINEBEFORE_EXPANDED = 0x0002,
        SC_FOLDFLAG_LINEBEFORE_CONTRACTED = 0x0004,
        SC_FOLDFLAG_LINEAFTER_EXPANDED = 0x0008,
        SC_FOLDFLAG_LINEAFTER_CONTRACTED = 0x0010,
        SC_FOLDFLAG_LEVELNUMBERS = 0x0040,
        SC_FOLDFLAG_LINESTATE = 0x0080,
    };

    enum
    {
        SC_LINE_END_TYPE_DEFAULT = 0,
        SC_LINE_END_TYPE_UNICODE = 1,
    };

    enum
    {
        SC_TIME_FOREVER = 10000000
    };

    enum
    {
        SC_WRAP_NONE = 0,
        SC_WRAP_WORD = 1,
        SC_WRAP_CHAR = 2,
        SC_WRAP_WHITESPACE = 3,
    };

    enum
    {
        SC_WRAPINDENT_FIXED = 0,
        SC_WRAPINDENT_SAME = 1,
        SC_WRAPINDENT_INDENT = 2,
        SC_WRAPINDENT_DEEPINDENT = 3,
    };

    enum
    {
        SC_CACHE_NONE = 0,
        SC_CACHE_CARET = 1,
        SC_CACHE_PAGE = 2,
        SC_CACHE_DOCUMENT = 3
    };

    enum
    {
        SC_PHASES_ONE = 0,
        SC_PHASES_TWO = 1,
        SC_PHASES_MULTIPLE = 2,
    };

    enum
    {
        ANNOTATION_HIDDEN = 0,
        ANNOTATION_STANDARD = 1,
        ANNOTATION_BOXED = 2,
        ANNOTATION_INDENTED = 3,
    };

    enum
    {
        EDGE_NONE = 0,
        EDGE_LINE = 1,
        EDGE_BACKGROUND = 2,
        EDGE_MULTILINE = 3,
    };

    enum
    {
        SC_CURSORNORMAL = -1,
        SC_CURSORARROW = 2,
        SC_CURSORWAIT = 4,
        SC_CURSORREVERSEARROW = 7
    };

    enum
    {
        UNDO_MAY_COALESCE = 1
    };

    enum
    {
        VISIBLE_SLOP = 0x01,
        VISIBLE_STRICT = 0x04
    };

    enum
    {
        CARET_SLOP = 0x01,
        CARET_STRICT = 0x04,
        CARET_JUMPS = 0x10,
        CARET_EVEN = 0x08
    };

    enum
    {
        CARETSTYLE_INVISIBLE = 0,
        CARETSTYLE_LINE = 1,
        CARETSTYLE_BLOCK = 2
    };

    enum
    {
        SC_MOD_INSERTTEXT = 0x1,
        SC_MOD_DELETETEXT = 0x2,
        SC_MOD_CHANGESTYLE = 0x4,
        SC_MOD_CHANGEFOLD = 0x8,
        SC_PERFORMED_USER = 0x10,
        SC_PERFORMED_UNDO = 0x20,
        SC_PERFORMED_REDO = 0x40,
        SC_MULTISTEPUNDOREDO = 0x80,
        SC_LASTSTEPINUNDOREDO = 0x100,
        SC_MOD_CHANGEMARKER = 0x200,
        SC_MOD_BEFOREINSERT = 0x400,
        SC_MOD_BEFOREDELETE = 0x800,
        SC_MULTILINEUNDOREDO = 0x1000,
        SC_STARTACTION = 0x2000,
        SC_MOD_CHANGEINDICATOR = 0x4000,
        SC_MOD_CHANGELINESTATE = 0x8000,
        SC_MOD_CHANGEMARGIN = 0x10000,
        SC_MOD_CHANGEANNOTATION = 0x20000,
        SC_MOD_CONTAINER = 0x40000,
        SC_MOD_LEXERSTATE = 0x80000,
        SC_MOD_INSERTCHECK = 0x100000,
        SC_MOD_CHANGETABSTOPS = 0x200000,
        SC_MODEVENTMASKALL = 0x3fffff
    };

    enum
    {
        SCK_DOWN = 300,
        SCK_UP = 301,
        SCK_LEFT = 302,
        SCK_RIGHT = 303,
        SCK_HOME = 304,
        SCK_END = 305,
        SCK_PRIOR = 306,
        SCK_NEXT = 307,
        SCK_DELETE = 308,
        SCK_INSERT = 309,
        SCK_ESCAPE = 7,
        SCK_BACK = 8,
        SCK_TAB = 9,
        SCK_RETURN = 13,
        SCK_ADD = 310,
        SCK_SUBTRACT = 311,
        SCK_DIVIDE = 312,
        SCK_WIN = 313,
        SCK_RWIN = 314,
        SCK_MENU = 315
    };

    //! This enum defines the different modifier keys.
    enum
    {
        //! No modifier key.
        SCMOD_NORM = 0,

        //! Shift key.
        SCMOD_SHIFT = 1,

        //! Control key (the Command key on OS/X, the Ctrl key on other
        //! platforms).
        SCMOD_CTRL = 2,

        //! Alt key.
        SCMOD_ALT = 4,

        //! This is the same as SCMOD_META on all platforms.
        SCMOD_SUPER = 8,

        //! Meta key (the Ctrl key on OS/X, the Windows key on other
        //! platforms).
        SCMOD_META = 16
    };

    //! This enum defines the different language lexers.
    //!
    //! \sa SCI_GETLEXER, SCI_SETLEXER
    enum
    {
        //! No lexer is selected and the SCN_STYLENEEDED signal is emitted so
        //! that the application can style the text as needed.  This is the
        //! default.
        SCLEX_CONTAINER = 0,

        //! Select the null lexer that does no syntax styling.
        SCLEX_NULL = 1,

        //! Select the Python lexer.
        SCLEX_PYTHON = 2,

        //! Select the C++ lexer.
        SCLEX_CPP = 3,

        //! Select the HTML lexer.
        SCLEX_HTML = 4,

        //! Select the XML lexer.
        SCLEX_XML = 5,

        //! Select the Perl lexer.
        SCLEX_PERL = 6,

        //! Select the SQL lexer.
        SCLEX_SQL = 7,

        //! Select the Visual Basic lexer.
        SCLEX_VB = 8,

        //! Select the lexer for properties style files.
        SCLEX_PROPERTIES = 9,

        //! Select the lexer for error list style files.
        SCLEX_ERRORLIST = 10,

        //! Select the Makefile lexer.
        SCLEX_MAKEFILE = 11,

        //! Select the Windows batch file lexer.
        SCLEX_BATCH = 12,

        //! Select the LaTex lexer.
        SCLEX_LATEX = 14,

        //! Select the Lua lexer.
        SCLEX_LUA = 15,

        //! Select the lexer for diff output.
        SCLEX_DIFF = 16,

        //! Select the lexer for Apache configuration files.
        SCLEX_CONF = 17,

        //! Select the Pascal lexer.
        SCLEX_PASCAL = 18,

        //! Select the Avenue lexer.
        SCLEX_AVE = 19,

        //! Select the Ada lexer.
        SCLEX_ADA = 20,

        //! Select the Lisp lexer.
        SCLEX_LISP = 21,

        //! Select the Ruby lexer.
        SCLEX_RUBY = 22,

        //! Select the Eiffel lexer.
        SCLEX_EIFFEL = 23,

        //! Select the Eiffel lexer folding at keywords.
        SCLEX_EIFFELKW = 24,

        //! Select the Tcl lexer.
        SCLEX_TCL = 25,

        //! Select the lexer for nnCron files.
        SCLEX_NNCRONTAB = 26,

        //! Select the Bullant lexer.
        SCLEX_BULLANT = 27,

        //! Select the VBScript lexer.
        SCLEX_VBSCRIPT = 28,

        //! Select the ASP lexer.
        SCLEX_ASP = SCLEX_HTML,

        //! Select the PHP lexer.
        SCLEX_PHP = SCLEX_HTML,

        //! Select the Baan lexer.
        SCLEX_BAAN = 31,

        //! Select the Matlab lexer.
        SCLEX_MATLAB = 32,

        //! Select the Scriptol lexer.
        SCLEX_SCRIPTOL = 33,

        //! Select the assembler lexer (';' comment character).
        SCLEX_ASM = 34,

        //! Select the C++ lexer with case insensitive keywords.
        SCLEX_CPPNOCASE = 35,

        //! Select the FORTRAN lexer.
        SCLEX_FORTRAN = 36,

        //! Select the FORTRAN77 lexer.
        SCLEX_F77 = 37,

        //! Select the CSS lexer.
        SCLEX_CSS = 38,

        //! Select the POV lexer.
        SCLEX_POV = 39,

        //! Select the Basser Lout typesetting language lexer.
        SCLEX_LOUT = 40,

        //! Select the EScript lexer.
        SCLEX_ESCRIPT = 41,

        //! Select the PostScript lexer.
        SCLEX_PS = 42,

        //! Select the NSIS lexer.
        SCLEX_NSIS = 43,

        //! Select the MMIX assembly language lexer.
        SCLEX_MMIXAL = 44,

        //! Select the Clarion lexer.
        SCLEX_CLW = 45,

        //! Select the Clarion lexer with case insensitive keywords.
        SCLEX_CLWNOCASE = 46,

        //! Select the MPT text log file lexer.
        SCLEX_LOT = 47,

        //! Select the YAML lexer.
        SCLEX_YAML = 48,

        //! Select the TeX lexer.
        SCLEX_TEX = 49,

        //! Select the Metapost lexer.
        SCLEX_METAPOST = 50,

        //! Select the PowerBASIC lexer.
        SCLEX_POWERBASIC = 51,

        //! Select the Forth lexer.
        SCLEX_FORTH = 52,

        //! Select the Erlang lexer.
        SCLEX_ERLANG = 53,

        //! Select the Octave lexer.
        SCLEX_OCTAVE = 54,

        //! Select the MS SQL lexer.
        SCLEX_MSSQL = 55,

        //! Select the Verilog lexer.
        SCLEX_VERILOG = 56,

        //! Select the KIX-Scripts lexer.
        SCLEX_KIX = 57,

        //! Select the Gui4Cli lexer.
        SCLEX_GUI4CLI = 58,

        //! Select the Specman E lexer.
        SCLEX_SPECMAN = 59,

        //! Select the AutoIt3 lexer.
        SCLEX_AU3 = 60,

        //! Select the APDL lexer.
        SCLEX_APDL = 61,

        //! Select the Bash lexer.
        SCLEX_BASH = 62,

        //! Select the ASN.1 lexer.
        SCLEX_ASN1 = 63,

        //! Select the VHDL lexer.
        SCLEX_VHDL = 64,

        //! Select the Caml lexer.
        SCLEX_CAML = 65,

        //! Select the BlitzBasic lexer.
        SCLEX_BLITZBASIC = 66,

        //! Select the PureBasic lexer.
        SCLEX_PUREBASIC = 67,

        //! Select the Haskell lexer.
        SCLEX_HASKELL = 68,

        //! Select the PHPScript lexer.
        SCLEX_PHPSCRIPT = 69,

        //! Select the TADS3 lexer.
        SCLEX_TADS3 = 70,

        //! Select the REBOL lexer.
        SCLEX_REBOL = 71,

        //! Select the Smalltalk lexer.
        SCLEX_SMALLTALK = 72,

        //! Select the FlagShip lexer.
        SCLEX_FLAGSHIP = 73,

        //! Select the Csound lexer.
        SCLEX_CSOUND = 74,

        //! Select the FreeBasic lexer.
        SCLEX_FREEBASIC = 75,

        //! Select the InnoSetup lexer.
        SCLEX_INNOSETUP = 76,

        //! Select the Opal lexer.
        SCLEX_OPAL = 77,

        //! Select the Spice lexer.
        SCLEX_SPICE = 78,

        //! Select the D lexer.
        SCLEX_D = 79,

        //! Select the CMake lexer.
        SCLEX_CMAKE = 80,

        //! Select the GAP lexer.
        SCLEX_GAP = 81,

        //! Select the PLM lexer.
        SCLEX_PLM = 82,

        //! Select the Progress lexer.
        SCLEX_PROGRESS = 83,

        //! Select the Abaqus lexer.
        SCLEX_ABAQUS = 84,

        //! Select the Asymptote lexer.
        SCLEX_ASYMPTOTE = 85,

        //! Select the R lexer.
        SCLEX_R = 86,

        //! Select the MagikSF lexer.
        SCLEX_MAGIK = 87,

        //! Select the PowerShell lexer.
        SCLEX_POWERSHELL = 88,

        //! Select the MySQL lexer.
        SCLEX_MYSQL = 89,

        //! Select the gettext .po file lexer.
        SCLEX_PO = 90,

        //! Select the TAL lexer.
        SCLEX_TAL = 91,

        //! Select the COBOL lexer.
        SCLEX_COBOL = 92,

        //! Select the TACL lexer.
        SCLEX_TACL = 93,

        //! Select the Sorcus lexer.
        SCLEX_SORCUS = 94,

        //! Select the PowerPro lexer.
        SCLEX_POWERPRO = 95,

        //! Select the Nimrod lexer.
        SCLEX_NIMROD = 96,

        //! Select the SML lexer.
        SCLEX_SML = 97,

        //! Select the Markdown lexer.
        SCLEX_MARKDOWN = 98,

        //! Select the txt2tags lexer.
        SCLEX_TXT2TAGS = 99,

        //! Select the 68000 assembler lexer.
        SCLEX_A68K = 100,

        //! Select the Modula 3 lexer.
        SCLEX_MODULA = 101,

        //! Select the CoffeeScript lexer.
        SCLEX_COFFEESCRIPT = 102,

        //! Select the Take Command lexer.
        SCLEX_TCMD = 103,

        //! Select the AviSynth lexer.
        SCLEX_AVS = 104,

        //! Select the ECL lexer.
        SCLEX_ECL = 105,

        //! Select the OScript lexer.
        SCLEX_OSCRIPT = 106,

        //! Select the Visual Prolog lexer.
        SCLEX_VISUALPROLOG = 107,

        //! Select the Literal Haskell lexer.
        SCLEX_LITERATEHASKELL = 108,

        //! Select the Structured Text lexer.
        SCLEX_STTXT = 109,

        //! Select the KVIrc lexer.
        SCLEX_KVIRC = 110,

        //! Select the Rust lexer.
        SCLEX_RUST = 111,

        //! Select the MSC Nastran DMAP lexer.
        SCLEX_DMAP = 112,

        //! Select the assembler lexer ('#' comment character).
        SCLEX_AS = 113,

        //! Select the DMIS lexer.
        SCLEX_DMIS = 114,

        //! Select the lexer for Windows registry files.
        SCLEX_REGISTRY = 115,

        //! Select the BibTex lexer.
        SCLEX_BIBTEX = 116,

        //! Select the Motorola S-Record hex lexer.
        SCLEX_SREC = 117,

        //! Select the Intel hex lexer.
        SCLEX_IHEX = 118,

        //! Select the Tektronix extended hex lexer.
        SCLEX_TEHEX = 119,

        //! Select the JSON hex lexer.
        SCLEX_JSON = 120,

        //! Select the EDIFACT lexer.
        SCLEX_EDIFACT = 121,

        //! Select the pseudo-lexer used for the indentation-based folding of
        //! files.
        SCLEX_INDENT = 122,

        //! Select the Maxima lexer.
        SCLEX_MAXIMA = 123,

        //! Select the Stata lexer.
        SCLEX_STATA = 124,

        //! Select the SAS lexer.
        SCLEX_SAS = 125,
    };

    enum
    {
        SC_WEIGHT_NORMAL = 400,
        SC_WEIGHT_SEMIBOLD = 600,
        SC_WEIGHT_BOLD = 700,
    };

    enum
    {
        SC_TECHNOLOGY_DEFAULT = 0,
        SC_TECHNOLOGY_DIRECTWRITE = 1,
        SC_TECHNOLOGY_DIRECTWRITERETAIN = 2,
        SC_TECHNOLOGY_DIRECTWRITEDC = 3,
    };

    enum
    {
        SC_CASEINSENSITIVEBEHAVIOUR_RESPECTCASE = 0,
        SC_CASEINSENSITIVEBEHAVIOUR_IGNORECASE = 1,
    };

    enum
    {
        SC_FONT_SIZE_MULTIPLIER = 100,
    };

    enum
    {
        SC_FOLDACTION_CONTRACT = 0,
        SC_FOLDACTION_EXPAND = 1,
        SC_FOLDACTION_TOGGLE = 2,
    };

    enum
    {
        SC_AUTOMATICFOLD_SHOW = 0x0001,
        SC_AUTOMATICFOLD_CLICK = 0x0002,
        SC_AUTOMATICFOLD_CHANGE = 0x0004,
    };

    enum
    {
        SC_ORDER_PRESORTED = 0,
        SC_ORDER_PERFORMSORT = 1,
        SC_ORDER_CUSTOM = 2,
    };
Q_SIGNALS:
    void RCE_SelChanged(bool y);
    void RCE_AutoCCancled();
    void RCE_AutoCHardDeleted();
    void RCE_AutoCCompleted(const char *selection, int pos, int ch, int method);

    void RCE_AutoCSelection(const char *selection, int position);
    void RCE_AutoCSelection(const char *selection, int position, int ch, int method);

    void RCE_AutoCSelectionChange(const char *selection, int id, int position);
    void RCE_NChanged();
    void RCE_CallTipClicked(int direction);
    void RCE_CharAdded(int charadded);
    void RCE_DoubleClick(int position, int line, int modifiers);
    void RCE_DWellEnd(int position, int x, int y);
    void RCE_DWellStart(int position, int x, int y);
    void RCE_FocusIn();
    void RCE_FocusOut();
    void RCE_HotSpotClick(int position, int modifiers);
    void RCE_HotSpotDoubleClick(int position, int modifiers);
    void RCE_HotSpotReleaseClick(int position, int modifiers);
    void RCE_IndicatorClick(int position, int modifiers);
    void RCE_IndicatorRelease(int position, int modifiers);
    void RCE_MacroRecord(unsigned int, unsigned long, void *);
    void RCE_MarginClick(int position, int modifiers, int margin);
    void RCE_MarginRightClick(int position, int modifiers, int margin);
    void RCE_Modified(int, int, const char *, int, int, int, int, int, int, int);
    void RCE_ModifyAttempRo();
    void RCE_NeedShowN(int, int);
    void RCE_Painted();
    void RCE_SavePointLeft();
    void RCE_SavePointReach();
    void RCE_StyleNeeded(int position);
    void RCE_URIDropped(const QUrl &url);
    void RCE_UpdateUI(int updated);

    void RCE_UserListSelection(const char *selection, int id);
    void RCE_UserListSelection(const char *selection, int id, int ch, int method);
    void RCE_UserListSelection(const char *selection, int id, int ch, int method, int position);

    void RCE_Zoom();
protected:
    virtual bool canInsertFromMimeData(const QMimeData *src) const;
    virtual QByteArray fromMimeData(const QMimeData *source, bool &rectangular) const;
    virtual QMimeData *toMimeData(const QByteArray &text, bool rectangular) const;

    void changeEvent(QEvent *) override;
    void contextMenuEvent(QContextMenuEvent *) override;
    void dragEnterEvent(QDragEnterEvent *) override;
    void dragLeaveEvent(QDragLeaveEvent *) override;
    void dragMoveEvent(QDragMoveEvent *) override;
    void dropEvent(QDropEvent *) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    bool focusNextPrevChild(bool next) override;
    void keyPressEvent(QKeyEvent *) override;
    void inputMethodEvent(QInputMethodEvent *) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery) const override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void scrollContentsBy(int dx, int dy) override;
    void setScrollBars();

    typedef QByteArray RCBytes;
private slots:
    void handleVerticalScrollBar(int value);
    void handleHorizontalScrollBar(int value);
private:
    void connectHorizontalScrollBar();
    void connectVerticalScrollBar();
    void acceptAction(QDropEvent *e);
    int eventModifiers(QMouseEvent *e);
private:
    //Disable Unexpect Copies
    RCEBase(const RCEBase&);
    RCEBase *operator=(const RCEBase&);

    friend class RetoCodeEditor;
    RetoCodeEditor *rce;
    QPoint tripleClickAt;
    QTimer tripleClick;

    int preEditPos, preEditNrBytes;
    QString preEditString;
    bool clickCausedFocus;
};

#endif // RCE_BASE_H