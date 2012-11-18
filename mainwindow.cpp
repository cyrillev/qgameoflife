#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringList>
#include <QStringListModel>
#include <QIcon>

#include "model/gameoflifepatternmodel.h"
#include "view/gameoflifescene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QAbstractListModel * model = new GameOfLifePatternModel(this);
    ui->treeView->setModel(model);

    GameOfLifeScene * scene = new GameOfLifeScene(this);
    ui->graphicsView->setScene(scene);

    connect (ui->treeView, SIGNAL(selectedPatternChanged(const GenGolPatternPtr&)),
             scene, SLOT(patternSelected(const GenGolPatternPtr&)) );

    ui->actionZoom_In->setIcon( QIcon::fromTheme("zoom-in" ) );

    connect( ui->actionZoom_In, SIGNAL( triggered()),
             ui->graphicsView , SLOT(ZoomIn()) );
    connect( ui->actionZoom_Out, SIGNAL( triggered()),
             ui->graphicsView , SLOT(ZoomOut()) );
    connect( ui->actionZoom_Original, SIGNAL( triggered()),
             ui->graphicsView , SLOT(ZoomOriginal()) );
    connect( ui->actionZoom_Fit, SIGNAL( triggered()),
             ui->graphicsView , SLOT(ZoomFit()) );

}

MainWindow::~MainWindow()
{
    delete ui;
}
