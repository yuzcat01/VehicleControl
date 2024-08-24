#ifndef HOME_H
#define HOME_H

#include <QWidget>

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

signals:
    void toWeather();
    void toMusicPage();
    void toMap();
    void toCommu();
    void toVedio();
    void toPicture();
    void toBrowser();

private slots:
    void on_pushButton_clicked();
    void on_toMusicPage_clicked();
    void on_pushButton_2_clicked();

    void on_toCommu_clicked();

    void on_toVedio_clicked();

    void on_toPic_clicked();

    void on_toBrow_clicked();

private:
    Ui::Home *ui;
};

#endif // HOME_H
