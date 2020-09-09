#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <QObject>
#include <QRunnable>

class Runnable : public QRunnable, public QObject
{
public:
    virtual void run() = 0;
};

#endif // RUNNABLE_H
