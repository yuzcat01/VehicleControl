#ifndef WEATHERDATA_H
#define WEATHERDATA_H
#include <QString>

class Today {
public:
    Today()
    {
        city ="����";
        date ="2023-7-28";

        wendu = 0;
        type ="����";
        high = 30;
        low = 18;

        ganmao ="��ðָ��";

        shidu = "0%";
        pm25 = 0;
        fx ="�Ϸ�";
        fl ="2��";
        quality ="������";
    }

    QString city;
    QString date;

    double wendu;
    QString type;

    int high;
    int low;

    QString ganmao;

    QString shidu;
    int pm25;
    QString fx;
    QString fl;
    QString quality;
};

class Day{
public:
    Day()
    {
        week ="����";
        date ="2023-7-29";

        type ="����";

        aqi = 0; //����ָ������

        high = 0;
        low = 0;

        fx ="�Ϸ�";
        fl ="2��";
    }

    QString week;
    QString date;

    QString type;

    int aqi;

    int high;
    int low;

    QString fx;
    QString fl;
};
#endif // WEATHERDATA_H
