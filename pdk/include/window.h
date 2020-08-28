#ifndef WINDOW_H
#define WINDOW_H

#include "interface.h"

class Page;
class Window
{
public:
    virtual void addPage(PagePtr, const QString &title) = 0;
};

#endif // WINDOW_H
