#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./View/Home/home.h"
#include "./View/Weather/weather.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //init and show home
    void init();
    void showHome();
    //to add

private:
    Ui::MainWindow *ui;
    Home home;
    Weather weather;
};
#endif // MAINWINDOW_H
