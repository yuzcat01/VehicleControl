#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>

namespace Ui {
class Weather;
}

class Weather : public QWidget
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    ~Weather();
    void getWeather();

signals:
    void toHome();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Weather *ui;
    QString cityCode;
};

#endif // WEATHER_H
