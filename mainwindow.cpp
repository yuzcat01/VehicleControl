#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    showHome();
    //定时器
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    // HomePage
    ui->stackedWidget->addWidget(&home);
    // WeatherPage
    ui->stackedWidget->addWidget(&weather);
    connect(&home,&Home::toWeather,this,[=](){
        if(weather.isSearched == false){
            weather.getWeatherInfo("北京");
        }
        ui->stackedWidget->setCurrentWidget(&weather);
    });
    connect(&weather,&Weather::toHome,this,[=]{
        ui->stackedWidget->setCurrentWidget(&home);
    });
    // MusicPage
    ui->stackedWidget->addWidget(&musicPage);
    connect(&home,&Home::toMusicPage,this,[=](){
        ui->stackedWidget->setCurrentWidget(&musicPage);
    });
    connect(&musicPage,&MusicPage::toHome,this,[=]{
        ui->stackedWidget->setCurrentWidget(&home);
    });
    //MapPage
    ui->stackedWidget->addWidget(&map);
    connect(&home,&Home::toMap,this,[=](){
        ui->stackedWidget->setCurrentWidget(&map);
        map.getIp();
    });
    connect(&map,&Map::toHome,this,[=]{
        ui->stackedWidget->setCurrentWidget(&home);
    });
    //CommunicatePage
    ui->stackedWidget->addWidget(&communicate);
    connect(&home,&Home::toCommu,this,[=](){
        ui->stackedWidget->setCurrentWidget(&communicate);
    });
    connect(&communicate,&Communicate::toHome,this,[=]{
        ui->stackedWidget->setCurrentWidget(&home);
    });
    //Vediopage
    ui->stackedWidget->addWidget(&vedio);
    connect(&home,&Home::toVedio,this,[=](){
        ui->stackedWidget->setCurrentWidget(&vedio);
    });
    connect(&vedio,&Vedio::toHome,this,[=]{
        ui->stackedWidget->setCurrentWidget(&home);
    });
    //PicturePage
    ui->stackedWidget->addWidget(&picture);
    connect(&home,&Home::toPicture,this,[=](){
        ui->stackedWidget->setCurrentWidget(&picture);
    });
    connect(&picture,&Picture::toHome,this,[=]{
        ui->stackedWidget->setCurrentWidget(&home);
    });
}

void MainWindow::showHome()
{
    ui->stackedWidget->setCurrentWidget(&home);
}

