#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication application( argc, argv );
    MainWindow mainWindow;
    mainWindow.show();

    // Realtime loop (maybe not the best possible way to be implemented in Qt)
    while( !mainWindow.isHidden() )
    {
        if ( application.hasPendingEvents() )
            application.processEvents();

        mainWindow.onIdle();
    }

    return 0;
}
