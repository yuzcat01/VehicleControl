#ifndef MAP_H
#define MAP_H

#include <QWidget>

namespace Ui {
class Map;
}

class Map : public QWidget
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = nullptr);
    ~Map();

signals:
    void toHome();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Map *ui;
};

#endif // MAP_H
