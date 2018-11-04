#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Ffmpeg");
    // set the ui components
    inputPath = ui->inputGroupBox->findChild<QLineEdit *>("inputFilePath");
    inputName = ui->inputGroupBox->findChild<QLineEdit *>("inputFileName");
    outputPath = ui->outputGroupBox->findChild<QLineEdit *>("outputFilePath");
    outputName = ui->outputGroupBox->findChild<QLineEdit *>("outputFileName");
    outputOpenButton = ui->outputGroupBox->findChild<QPushButton *>("outputOpenButton");
    startTime = ui->outputGroupBox->findChild<QLineEdit *>("startTime");
    stopTime = ui->outputGroupBox->findChild<QLineEdit *>("stopTime");

    // disable all outputGroupBox clickable components
    outputPath->setDisabled(true);
    outputName->setDisabled(true);
    outputOpenButton->setDisabled(true);
    startTime->setDisabled(true);
    stopTime->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    openFile();
}

void MainWindow::openFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Choose an input video");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    }
    setWindowTitle(filename);
    inputPath->setText(filename);
    QFileInfo fileInfo(file.fileName());
    inputName->setText(fileInfo.fileName());
}

void MainWindow::on_inputOpenButton_clicked()
{
    openFile();
}
