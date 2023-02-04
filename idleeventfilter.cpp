#include "idleeventfilter.h"

idleEventFilter::idleEventFilter(QObject *parent) : QObject(parent)
{

}

bool idleEventFilter::  eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::KeyPress)
    {

    }

    return QObject::eventFilter(obj, ev);
}
