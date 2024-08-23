#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    showHome();
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
        ui->stackedWidget->setCurrentWidget(&weather);
    });
    connect(&weather,&Weather::toHome,this,[=]{
        ui->stackedWidget->setCurrentWidget(&home);
    });
    // To add
}

void MainWindow::showHome()
{
    ui->stackedWidget->setCurrentWidget(&home);
}
