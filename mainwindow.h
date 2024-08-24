#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./View/Home/home.h"
#include "./View/Weather/weather.h"
#include "./View/Music/musicpage.h"
#include "./View/Map/map.h"
#include "./View/Communicate/communicate.h"
#include "./View/Vedio/vedio.h"
#include "./View/Picture/picture.h"
#include "./View/Browser/browser.h"

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
    Map map;
    Communicate communicate;
    Vedio vedio;
    Picture picture;
    Browser browser;
    //to add
};
#endif // MAINWINDOW_H
