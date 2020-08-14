#ifndef WINDOW_H
#define WINDOW_H

#include <QString>

class Page;
class Window
{
public:
    virtual void addPage(Page *, const QString &title) = 0;
};

#endif // WINDOW_H
