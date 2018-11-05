#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#ifdef __APPLE__
    Utils::setMacEnvironment();
#endif
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
    commandLine = ui->commandLineGroupBox->findChild<QTextEdit *>("commandLine");
    vidProps = ui->inputGroupBox->findChild<QTextEdit *>("vidProps");

    commandLine->setReadOnly(true);

    inputName->setEnabled(false);

    // disable all outputGroupBox clickable components
    setOutputFileComponentsEnabled(false);
}

void MainWindow::setOutputFileComponentsEnabled(bool enabled) {
    outputPath->setEnabled(enabled);
    outputName->setEnabled(enabled);
    outputOpenButton->setEnabled(enabled);
    startTime->setEnabled(enabled);
    stopTime->setEnabled(enabled);
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
        return;
    }
    inputPath->setText(filename);
    QFileInfo fileInfo(file.fileName());
    inputName->setText(fileInfo.fileName());
    setOutputFileComponentsEnabled(true);
    inputName->setEnabled(true);

    QString commandStr = "ffprobe";
    QStringList params;
    params << "-i" << inputPath->text() << "-show_format" << "-hide_banner";
    QProcess ffprobe;
    ffprobe.start(commandStr, params);
    ffprobe.waitForFinished(-1);
    QString out = ffprobe.readAllStandardOutput();
    QString err = ffprobe.readAllStandardError();
    vidProps->setText(out);

    // display command in the command line
    displayCommand(commandStr, params);

    // set the default output video file name
    QString folderName = Utils::getFolderName(inputPath->text());
    outputPath->setText(folderName + "untitled" + "."
                        + Utils::getFileFormat(filename));


}


void MainWindow::on_inputOpenButton_clicked()
{
    openFile();
}

void MainWindow::on_inputFilePath_textChanged(const QString &arg1)
{
    QFile file(arg1);
    QFileInfo fileInfo(file.fileName());
    inputName->setText(fileInfo.fileName());
}

void MainWindow::on_inputFileName_textChanged(const QString &arg1)
{
    linkFileNameToPath(inputPath, inputName);
}

void MainWindow::linkFileNameToPath(QLineEdit *path, QLineEdit *filename) {
    path->setText(Utils::getFolderName(path->text()) + filename->text());
}

void MainWindow::on_outputFilePath_textEdited(const QString &arg1)
{
    QFile file(arg1);
    QFileInfo fileInfo(file.fileName());
    outputName->setText(fileInfo.fileName());
}

void MainWindow::on_outputFileName_textEdited(const QString &arg1)
{
    linkFileNameToPath(outputPath, outputName);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_outputFilePath_textChanged(const QString &arg1)
{
    QFile file(arg1);
    QFileInfo fileInfo(file.fileName());
    outputName->setText(fileInfo.fileName());
}

void MainWindow::on_trimButton_clicked()
{
    int sTime = startTime->text().toInt();
    int eTime = stopTime->text().toInt();

    //TODO: check the start and stop time

    QString commandStr = "ffmpeg";
    QProcess ffmpeg;
    QStringList params;

    params << "-ss" << startTime->text() << "-i"  << inputPath->text() << "-c"
           << "copy"<< "-t" << QString::number(eTime-sTime)
           << outputPath->text();

    ffmpeg.start(commandStr, params);
    ffmpeg.waitForFinished(-1);
    QString out = ffmpeg.readAllStandardOutput();
    QString err = ffmpeg.readAllStandardError();

    displayCommand(commandStr, params);
}

void MainWindow::displayCommand(const QString& command, const QStringList& params) {
    QString fullCommand = command + " ";
    for(QString param : params) {
        fullCommand += param + " ";
    }
    commandLine->append("> " + fullCommand);

    // set the default output video file name
    QString folderName = Utils::getFolderName(inputPath->text());
}
