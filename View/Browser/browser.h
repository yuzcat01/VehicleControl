#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>

namespace Ui {
class Browser;
}

class Browser : public QWidget
{
    Q_OBJECT

public:
    explicit Browser(QWidget *parent = nullptr);
    ~Browser();

signals:
    void toHome();

private slots:
    void on_toHome_clicked();

private:
    Ui::Browser *ui;
};

#endif // BROWSER_H
