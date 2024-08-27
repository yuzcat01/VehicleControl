#include "home.h"
#include "ui_home.h"

#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QtMath>

Home::Home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);
    init();


}

Home::~Home()
{
    delete ui;
}

void Home::init()
{
    // 定时更新时间显示
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Home::updateTime);
    timer->start(1000); // 每秒更新一次时间
    QString imagePath = ":/new/prefix3/Resource/img/home/background.jpg"; // 图片路径
    this->setStyleSheet(QString("background-image: url(%1);").arg(imagePath));

}

//定时器超时调用的更新时间函数
void Home::updateTime()
{
    // 获取当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // 将日期转换为字符串
    QString currentDate = currentDateTime.toString("yyyy-MM-dd");

    // 将时间转换为字符串
    QString currentTime = currentDateTime.toString("HH:mm:ss");

    // 更新日期的QLabel文本
    ui->dateLabel->setText(currentDate);

    // 更新时间的QLabel文本
    ui->timeLabel->setText(currentTime);
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

