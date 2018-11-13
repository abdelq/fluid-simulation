#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Scenes/SceneCube.h"
#include "Scenes/SceneCylinder.h"
#include "Scenes/SceneSphere.h"
#include "Scenes/SceneSphereHighRes.h"
#include <QDesktopWidget>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi(this);
    buildSceneList();

    show();
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry() ) );

    connect( ui->sceneList, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( onSceneListItemClicked( QListWidgetItem* ) ) );
}

MainWindow::~MainWindow()
{
    for ( int i=0 ; i<ui->sceneList->count() ; ++i )
        delete (Scene*)ui->sceneList->item(i)->data(Qt::UserRole).value<void*>();

    delete ui;
}

void MainWindow::onIdle()
{
    ui->glWidget->onIdle();
}

void MainWindow::buildSceneList()
{
    QVector<QPair<QString,Scene*> > scenes;
    scenes.append( QPair<QString,Scene*>( "Sphere", new SceneSphere ) );
    scenes.append( QPair<QString,Scene*>( "Cube", new SceneCube ) );
    scenes.append( QPair<QString,Scene*>( "Cylinder", new SceneCylinder ) );
    scenes.append( QPair<QString,Scene*>( "Sphere - High resolution", new SceneSphereHighRes ) );

    for ( int i=0 ; i<scenes.size() ; ++i )
    {
        QListWidgetItem* item = new QListWidgetItem( scenes[i].first, ui->sceneList );
        item->setData( Qt::UserRole, QVariant::fromValue( (void*)scenes[i].second ) );
        ui->sceneList->addItem( item );
    }

    ui->sceneList->setCurrentRow( 0 );
    onSceneListItemClicked( ui->sceneList->currentItem() );
}

void MainWindow::onSceneListItemClicked( QListWidgetItem* item )
{
    ui->glWidget->setScene( (Scene*)item->data( Qt::UserRole ).value<void*>() );
}
