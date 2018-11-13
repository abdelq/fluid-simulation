#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Scenes/Scene.h"
#include <QMainWindow>

/* MainWindow is responsible for scene management.
 */

namespace Ui
{
    class MainWindow;
}

class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow();

    void onIdle();

private slots:
    void onSceneListItemClicked( QListWidgetItem* item );

private:
    void buildSceneList();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
