#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QMap>
#include <QMessageBox>
#include <QDoubleValidator>

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

    initialInputFilename = filename;

    // set the input path and filename text fields
    inputPath->setText(filename);
    QFileInfo fileInfo(file.fileName());
    inputName->setText(fileInfo.fileName());

    QString commandStr = "ffprobe";
    QStringList params;
    params << "-i" << inputPath->text() << "-show_format" << "-hide_banner";
    QProcess ffprobe;
    ffprobe.start(commandStr, params);
    ffprobe.waitForFinished(-1);
    QString out = ffprobe.readAllStandardOutput();
    QString err = ffprobe.readAllStandardError();

    QMap<QString, QString> mappedProps = Utils::mapProperties(out);
    QString parsedOptions;
    for(auto key : mappedProps.keys()) {
        parsedOptions += key + " = " + mappedProps.value(key) + "\n";
    }
    vidProps->setText(parsedOptions);

    QString durationProp = mappedProps.value(DURATION);

    if(durationProp == NA) {
        QMessageBox::warning(this, "warning", "Video file is not valid");
    } else {
        validInputVideoFile = true;
        durationValue = durationProp.toDouble();
        startTimeValue = mappedProps.value(START_TIME).toDouble();
    }

    if(validInputVideoFile) {
        setOutputFileComponentsEnabled(true);
        inputName->setEnabled(true);

        // display command in the command line
        displayCommand(commandStr, params);

        // set the default output video file name
        QString folderName = Utils::getFolderName(inputPath->text());
        outputPath->setText(folderName + "untitled" + "."
                            + Utils::getFileFormat(filename));

        // set start and stop time default values
        startTime->setText(QString::number(startTimeValue));
        stopTime->setText(QString::number(startTimeValue+durationValue));

        // set double validators for start and stop times
        startTime->setValidator(new QDoubleValidator(this));
        stopTime->setValidator(new QDoubleValidator(this));
    }
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
    if(initialInputFilename != inputPath->text()) {
        initialFileNameChanged = true;
    }
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
    if(!startAndStopTimesOk()) {
        QMessageBox::warning(this, "warning", "Invalid start or stop time");
        return;
    }

    if(Utils::fileExists(outputPath->text())) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Overwrite", "The file with name "
                                      + outputName->text()
                                      + " already exists.\n"
                                      + "Overwrite?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QFile file(outputPath->text());
            file.remove();
        } else {
          return;
        }
    }

    renameInputFile();

    double sTime = startTime->text().toDouble();
    double eTime = stopTime->text().toDouble();

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

bool MainWindow::startAndStopTimesOk() {
    double start = startTime->text().toDouble();
    double stop = stopTime->text().toDouble();

    if(start < 0 || stop < 0 || start >= stop
            || (stop > startTimeValue + durationValue)
            || (start > startTimeValue + durationValue)) {
        return false;
    }

    return true;
}

void MainWindow::renameInputFile() {
    if(initialFileNameChanged) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Rename", "The input file name "
                                      + inputName->text()
                                      + " was changed.\n"
                                      + "Rename?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QFile file(initialInputFilename);
            file.rename(inputPath->text());
        } else {
            inputPath->setText(initialInputFilename);
        }
    }
}
