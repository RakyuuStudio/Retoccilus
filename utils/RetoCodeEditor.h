#ifndef RetoCodeEditor_h
#define RetoCodeEditor_h

#include <QPlainTextEdit>
#include <QChar>
#include <QFont>

namespace RetoUtils {
    struct defaultParenthese {
        QChar left, right;
        bool autoComplete, autoDeCompleteOnExist, ctrlTabJumpout;

        defaultParenthese(const QChar &lp = '(', const QChar &rp = ')', bool pComplete = true,
                          bool pDecomplete = true, bool pJumpout = true)
                          : left(lp), right(rp), autoComplete(pComplete), autoDeCompleteOnExist(pDecomplete), ctrlTabJumpout(pJumpout)
        {

        }
    };
}

#endif //RetoCodeEditor_h