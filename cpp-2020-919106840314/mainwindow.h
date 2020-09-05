#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <math.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void trans();
    void start_count();
    bool judge_line1();
    bool judge_line2();
    double decisi( int a , int b);
    void count();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
