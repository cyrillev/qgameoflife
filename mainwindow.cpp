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

    scene = new GameOfLifeScene(0, 0, 10000, 10000, this);
    ui->graphicsView->setScene(scene);

    connect (ui->treeView, SIGNAL(selectedPatternChanged(const GenGolPatternPtr&)),
             scene, SLOT(patternSelected(const GenGolPatternPtr&)) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionStart_triggered()
{
    scene->Start();
}

void MainWindow::on_actionZoom_Fit_triggered()
{
    ui->graphicsView->ZoomFit();
}

void MainWindow::on_actionZoom_Original_triggered()
{
    ui->graphicsView->ZoomOriginal();
}

void MainWindow::on_actionZoom_Out_triggered()
{
    ui->graphicsView->ZoomOut();
}

void MainWindow::on_actionZoom_In_triggered()
{
    ui->graphicsView->ZoomIn();
}
