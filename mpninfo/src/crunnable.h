#ifndef CRUNNABLE_H
#define CRUNNABLE_H

#include <QObject>
#include <QRunnable>

class CRunnable : public QRunnable
{
public:
    CRunnable(void (* fn)(void *, void *), void *dat = nullptr, void *res = nullptr) :
        foo(fn), data(dat), result(res), finish(false) {}

    bool finished() { return finish; }

    void run() {
        foo(data, result);
        finish = true;
    }

private:
    void (* foo)(void *, void *);
    void *data;
    void *result;
    bool finish;
};

#endif // CRUNNABLE_H
