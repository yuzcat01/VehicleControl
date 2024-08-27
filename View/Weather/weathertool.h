#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H
#include<QString>
#include<QMap>
#include<QFile>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
#include<QJsonValue>

class WeatherTool {

private:
    static QMap<QString, QString> mCityMap;

    //初始化CityMap；
    static void initCityMap() {
        //1.读取文件
        QString filePath = ":/Resource/citycode-2019-08-23.json";
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray json = file.readAll();
        file.close();

        //2.解析并写入到map
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(json,&err);
        if(err.error != QJsonParseError::NoError) {
            return;
        }

        if(!doc.isArray()) {
            return;
        }

        QJsonArray cities = doc.array();
        for(int i=0; i<cities.size(); i++) {
            QString city = cities[i].toObject().value("city_name").toString();
            QString code = cities[i].toObject().value("city_code").toString();
            if(code.size() > 0) {
                mCityMap.insert(city, code);
            }
        }
    }
public:
    static QString getCityCode(QString cityName) {

        if(mCityMap.isEmpty()) {
            initCityMap();
        }

        QMap<QString, QString>::iterator it = mCityMap.find(cityName);
        if(it == mCityMap.end()) {
            it = mCityMap.find(cityName + "市");
        }

        if(it != mCityMap.end()) {
            return it.value();
        }

        return "";
    }
};

QMap<QString, QString> WeatherTool::mCityMap = {};

#endif // WEATHERTOOL_H
