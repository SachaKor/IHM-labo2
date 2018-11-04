#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>

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

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    void openFile();

    // ui components
    QLineEdit *inputPath;
    QLineEdit *inputName;
    QLineEdit *outputPath;
    QLineEdit *outputName;
    QPushButton *outputOpenButton;
    QLineEdit *startTime;
    QLineEdit *stopTime;
};

#endif // MAINWINDOW_H
