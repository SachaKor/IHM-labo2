#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // set the icon for the Open File buton
    QPixmap openButtonPixmap("qrc:/imgs/baseline-folder_open-24px.svg");
    QIcon openButtonIcon(openButtonPixmap);
    ui->pushButton->setIcon(openButtonIcon);
    ui->pushButton->setIconSize(openButtonPixmap.rect().size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    currentFile.clear();

}
