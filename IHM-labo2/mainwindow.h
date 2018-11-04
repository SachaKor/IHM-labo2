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

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    void openFile();
    void setOutputFileComponentsEnabled(bool enabled);
    void linkFileNameToPath(QLineEdit *path, QLineEdit *filename);

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
