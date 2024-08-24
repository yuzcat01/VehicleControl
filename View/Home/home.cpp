#include "home.h"
#include "ui_home.h"

Home::Home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);
}

Home::~Home()
{
    delete ui;
}

void Home::on_pushButton_clicked()
{
    emit toWeather();
}

void Home::on_toMusicPage_clicked()
{
    emit toMusicPage();
}

void Home::on_pushButton_2_clicked()
{
    emit toMap();
}


void Home::on_toCommu_clicked()
{
     emit toCommu();
}


void Home::on_toVedio_clicked()
{
     emit toVedio();
}


void Home::on_toPic_clicked()
{
     emit toPicture();
}


void Home::on_toBrow_clicked()
{
     emit toBrowser();
}

