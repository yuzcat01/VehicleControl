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

private slots:
    void on_pushButton_clicked();
    void on_toMusicPage_clicked();

private:
    Ui::Home *ui;
};

#endif // HOME_H
