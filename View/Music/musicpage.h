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

private:
    Ui::MusicPage *ui;
};

#endif // MUSICPAGE_H
