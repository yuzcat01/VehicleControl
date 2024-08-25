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
    , cityCode("101010100")
{
    ui->setupUi(this);
    init();


}

void Weather::init()
{
    ui->tabWidget->setTabText(0,"今日天气");
    ui->tabWidget->setTabText(1,"未来七天天气预报");
    ui->tabWidget->setCurrentIndex(0);
}
Weather::~Weather()
{
    delete ui;
}

void Weather::on_pushButton_clicked()
{
    emit toHome();
}

void Weather::getWeather()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);   //网络管理器
    QNetworkRequest request;   //请求
    request.setUrl(QUrl("http://t.weather.itboy.net/api/weather/city/" + cityCode));   //设置url
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

                    if(obj.contains("cityInfo"))
                    {
                        QJsonValue cityInfo = obj.value("cityInfo");
                        if(cityInfo.isObject())
                        {
                            QJsonObject cityInfoObj = cityInfo.toObject();
                            if(cityInfoObj.contains("city"))
                            {
                                QJsonValue city = cityInfoObj.value("city");
                                ui->label_2->setText("城市：" + city.toString());
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
                                            ui->label_5->setText("天气：" + type.toString());
                                            if(type.toString() == "晴")
                                                ui->img->setPixmap(QPixmap(":/img/prefix1/Resource/img/Weather/w0.png"));
                                            else if(type.toString() == "多云")
                                                ui->img->setPixmap(QPixmap(":/img/prefix1/Resource/img/Weather/w1.png"));
                                            else if(type.toString() == "阴")
                                                ui->img->setPixmap(QPixmap(":/img/prefix1/Resource/img/Weather/w2.png"));
                                            else if(type.toString().endsWith("雨"))
                                                ui->img->setPixmap(QPixmap(":/img/prefix1/Resource/img/Weather/w3.png"));

                                        }
                                    }
                                }
                            }

                            if(data.contains("wendu"))   //判断是否包含wendu
                            {
                                QJsonValue wendu = data.value("wendu");   //获取温度
                                ui->label_3->setText("温度：" + wendu.toString() + "℃");  //显示温度
                            }

                            if(data.contains("pm25"))
                            {
                                QJsonValue pm25 = data.value("pm25");
                                ui->label_4->setText("pm2.5：" + QString("%1").arg(pm25.toDouble()));
                            }
                        }
                    }
                }
            }
        }
        reply->deleteLater();
    });

}


void Weather::on_pushButton_2_clicked()
{
    getWeather();
}




void Weather::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0)
        this->cityCode = "101010100";
    else if(index == 1)
        this->cityCode = "101190101";
    else if(index == 2)
        this->cityCode = "101020100";
}

