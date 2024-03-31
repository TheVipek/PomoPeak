#ifndef TASKINPUTFILTER_H
#define TASKINPUTFILTER_H
#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QRegularExpression>
class TaskInputFilter : public QObject
{

    protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->text() == "\b")
            {
                return false;
            }
            if(NON_LETTER_SPECIAL_CHAR.match(keyEvent->text()).hasMatch())
            {
                return true;
            }
        }
        else if(event->type() == QEvent::ShortcutOverride)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->matches(QKeySequence::Copy) || keyEvent->matches(QKeySequence::Paste))
            {
                return true;
            }
            return false;
        }
        return QObject::eventFilter(obj, event);
    }
    private:
    const QRegularExpression NON_LETTER_SPECIAL_CHAR = QRegularExpression("[A-Za-z\\W]");
};

#endif // TASKINPUTFILTER_H
