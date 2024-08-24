#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <QWidget>

namespace Ui {
class Communicate;
}

class Communicate : public QWidget
{
    Q_OBJECT

public:
    explicit Communicate(QWidget *parent = nullptr);
    ~Communicate();

signals:
    void toHome();

private slots:
    void on_toHome_clicked();

private:
    Ui::Communicate *ui;
};

#endif // COMMUNICATE_H
