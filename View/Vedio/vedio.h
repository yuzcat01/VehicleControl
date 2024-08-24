#ifndef VEDIO_H
#define VEDIO_H

#include <QWidget>

namespace Ui {
class Vedio;
}

class Vedio : public QWidget
{
    Q_OBJECT

public:
    explicit Vedio(QWidget *parent = nullptr);
    ~Vedio();

signals:
    void toHome();

private slots:
    void on_toHome_clicked();

private:
    Ui::Vedio *ui;
};

#endif // VEDIO_H
