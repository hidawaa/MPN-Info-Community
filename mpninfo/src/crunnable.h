#ifndef CRUNNABLE_H
#define CRUNNABLE_H

#include <QObject>
#include <QRunnable>

class CRunnable : public QRunnable
{
public:
    CRunnable(void (* fn)(void *, void *), void *dat = nullptr, void *res = nullptr) :
        foo(fn), data(dat), result(res), running(true) {}

    bool isRunning() {
        return running;
    }

    void run() {
        foo(data, result);
        running = false;
    }

private:
    void (* foo)(void *, void *);
    void *data;
    void *result;
    bool running;
};

#endif // CRUNNABLE_H
