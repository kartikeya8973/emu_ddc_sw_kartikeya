#ifndef IDLEEVENTFILTER_H
#define IDLEEVENTFILTER_H

#include <QObject>
#include <QEvent>
class idleEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit idleEventFilter(QObject *parent = nullptr);

signals:

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};

#endif // IDLEEVENTFILTER_H
