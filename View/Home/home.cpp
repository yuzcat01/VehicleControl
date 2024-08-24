#include "home.h"
#include "ui_home.h"

#include <QPainter>
#include <QTime>
#include <QTimer>

Home::Home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);
    init();
    lookTime();

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
}

void Home::lookTime()
{
    // 获取当前本地时间
    QDateTime now = QDateTime::currentDateTime();

    // 转换为字符串
    // 使用 Qt::ISODate 格式，你也可以使用其他格式，如 Qt::RFC2822Date, Qt::SystemLocaleDate 等
    QString dateTimeString = now.toString(Qt::ISODate);


    // 使用自定义格式
    QString customFormat = now.toString("yyyy-MM-dd HH:mm:ss");

    ui->timeLabel->setText(customFormat);

}

//定时器超时调用的更新时间函数
void Home::updateTime()
{
    // 获取当前时间并转换为字符串
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    //显示时间的QLabel更新其文本
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

