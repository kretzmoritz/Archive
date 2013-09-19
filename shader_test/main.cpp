#include "window.h"

cWindow mainWindow;

int main(int ArgCount, char**Args)
{
    mainWindow.create(1000, 1000);

    return 0;
}
