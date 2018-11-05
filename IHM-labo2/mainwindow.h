#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_inputOpenButton_clicked();

    void on_inputFilePath_textChanged(const QString &arg1);

    void on_inputFileName_textChanged(const QString &arg1);

    void on_outputFilePath_textEdited(const QString &arg1);

    void on_outputFileName_textEdited(const QString &arg1);

    void on_actionExit_triggered();

    void on_outputFilePath_textChanged(const QString &arg1);

    void on_trimButton_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    void openFile();
    void setOutputFileComponentsEnabled(bool enabled);
    void linkFileNameToPath(QLineEdit *path, QLineEdit *filename);
    void displayCommand(const QString& command, const QStringList& params);
    bool startAndStopTimesOk();
    void renameInputFile();

    // constants
    const QString NA = "N/A";
    const QString DURATION = "duration";
    const QString START_TIME = "start_time";

    // video properties
    double durationValue = -1;
    double startTimeValue = -1;

    // file properties
    QString initialInputFilename = "";

    // checks
    bool validInputVideoFile = false;
    bool initialFileNameChanged = false;

    // ui components
    QLineEdit *inputPath;
    QLineEdit *inputName;
    QLineEdit *outputPath;
    QLineEdit *outputName;
    QPushButton *outputOpenButton;
    QLineEdit *startTime;
    QLineEdit *stopTime;
    QTextEdit *commandLine;
    QTextEdit *vidProps;

};

#endif // MAINWINDOW_H
