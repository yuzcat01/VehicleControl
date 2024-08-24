#ifndef MUSICPAGE_H
#define MUSICPAGE_H

#include <QWidget>

namespace Ui {
class MusicPage;
}

class MusicPage : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPage(QWidget *parent = nullptr);
    ~MusicPage();

signals:
    void toHome();

private slots:
    void on_backToHome_clicked();

private:
    Ui::MusicPage *ui;
};

#endif // MUSICPAGE_H
