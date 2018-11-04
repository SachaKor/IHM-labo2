#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

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
    }
    setWindowTitle(filename);
    inputPath->setText(filename);
    QFileInfo fileInfo(file.fileName());
    inputName->setText(fileInfo.fileName());
    setOutputFileComponentsEnabled(true);
    inputName->setEnabled(true);
    QString commandStr = "ffprobe";
    QStringList params;
    params << "-version";
#ifdef __APPLE__
    setMacEnvironment();
#endif
    QProcess ffprobe;
    ffprobe.start(commandStr, params);
    ffprobe.waitForFinished(-1);
    QString out = ffprobe.readAllStandardOutput();
    QString err = ffprobe.readAllStandardError();
    vidProps->setText(out);

    QString fullCommand = commandStr + " ";
    for(QString param : params) {
        fullCommand += param + " ";
    }
    commandLine->append("> " + fullCommand);

//    commandLine->append("> " + fullCommand);
//    QProcess sh;
//    sh.start("ls", QStringList() << "-c" << "-a");
//    sh.waitForFinished(-1);
//    QString out = sh.readAllStandardOutput();
//    vidProps->setText(out);

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
    QStringList pathVector = path->text().split("/");
    pathVector.pop_back();
    QString folderPath = "";
    for(QString p : pathVector) {
        folderPath += p;
        folderPath += "/";
    }
    path->setText(folderPath + filename->text());
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

void MainWindow::setMacEnvironment() {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QStringList envlist = env.toStringList();

    for(int i=0; i < envlist.size(); i++)
    {
        QString entry = envlist[i];

        if(entry.startsWith("PATH="))
        {
            int index = entry.indexOf("=");

            if(index != -1)
            {
                QString value = entry.right(entry.length() - (index+1));
                value += ":/usr/texbin:/usr/local/bin";

                setenv("PATH", value.toLatin1().constData(), true);
            }

            break;
        }
    }
}
