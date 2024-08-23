#include "weather.h"
#include "ui_weather.h"

#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QUrl>

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
#include<QJsonValue>
#include<QJsonArray>
#include<QByteArray>


Weather::Weather(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Weather)
{
    ui->setupUi(this);
}

Weather::~Weather()
{
    delete ui;
}

void Weather::on_pushButton_clicked()
{
    emit toHome();
}


void Weather::on_pushButton_2_clicked()
{
    QString cityToSearch = ui->lineEdit->text();

    //根据URL(http://t.weather.itboy.net/api/weather/city/101010100)http请求查询温度信息
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);   //网络管理器
    QNetworkRequest request;   //请求
    request.setUrl(QUrl("http://t.weather.itboy.net/api/weather/city/101010100"));   //设置url
    QNetworkReply *reply = manager->get(request); //发送get请求
    connect(reply, &QNetworkReply::finished, [=](){   //请求完成后的处理
        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray data = reply->readAll();
            QJsonParseError json_error;
            QJsonDocument doc = QJsonDocument::fromJson(data, &json_error);  //解析json数据
            if(json_error.error == QJsonParseError::NoError)
            {
                if(doc.isObject())
                {
                    QJsonObject obj = doc.object();  //获取json对象
                    if(obj.contains("cityInfo")) //获取城市名称
                    {
                        QJsonValue cityin = obj.value("cityInfo");
                        if(cityin.isObject())
                        {
                            QJsonObject cityi = cityin.toObject();
                            if(cityi.contains("city"))
                            {
                                QJsonValue cityname = cityi.value("city");
                                ui->label_2->setText("城市是：" + cityname.toString());
                            }
                        }
                    }
                    if(obj.contains("data"))  //判断是否包含data
                    {
                        QJsonValue value = obj.value("data");  //获取data数据
                        if(value.isObject())
                        {
                            QJsonObject data = value.toObject();  //获取data对象
                            if(data.contains("forecast"))
                            {
                                QJsonValue forecast = data.value("forecast");
                                if(forecast.isArray())
                                {
                                    QJsonArray forecastArray = forecast.toArray();
                                    QJsonValue todayWeather = forecastArray[0];
                                    if(todayWeather.isObject())
                                    {
                                        QJsonObject todayWeatherObj = todayWeather.toObject();
                                        if(todayWeatherObj.contains("type"))
                                        {
                                            QJsonValue type = todayWeatherObj.value("type");
                                            ui->label_4->setText("天1气：" + type.toString());
                                        }
                                    }
                                }
                            }
                            if(data.contains("wendu"))   //判断是否包含wendu
                            {
                                QJsonValue wendu = data.value("wendu");   //获取温度
                                ui->label_3->setText("温度：" + wendu.toString());  //显示温度
                            }
                            if(data.contains("pm25"))   //判断是否包含pm25
                            {
                                QJsonValue pm25 = data.value("pm25");   //获取pm25
                                ui->label_5->setText("PM2.5：" + QString("%1").arg(pm25.toDouble()));  //显示
                            }
                        }
                    }
                }
            }
        }
        reply->deleteLater();  //释放资源  这个很重要！！！
    });
}

