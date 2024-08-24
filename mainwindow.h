#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./View/Home/home.h"
#include "./View/Weather/weather.h"
#include "./View/Music/musicpage.h"

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

    void init();
    void showHome();

private:
    Ui::MainWindow *ui;
    Home home;
    Weather weather;
    MusicPage musicPage;
    //to add
};
#endif // MAINWINDOW_H
