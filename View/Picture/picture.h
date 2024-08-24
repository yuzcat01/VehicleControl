#ifndef PICTURE_H
#define PICTURE_H

#include <QWidget>

namespace Ui {
class Picture;
}

class Picture : public QWidget
{
    Q_OBJECT

public:
    explicit Picture(QWidget *parent = nullptr);
    ~Picture();

signals:
    void toHome();

private slots:
    void on_toHome_clicked();

private:
    Ui::Picture *ui;
};

#endif // PICTURE_H
