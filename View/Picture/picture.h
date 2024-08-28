#ifndef PICTURE_H
#define PICTURE_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QList>
#include <QMouseEvent>

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

    void showImage(int index);

    void on_selectButton_clicked();

    void on_prevButton_clicked();

    void on_nextButton_clicked();

    void on_removeButton_clicked();

private:
    Ui::Picture *ui;
    QList<QString> imagePaths; // Stores the paths of selected images
    int currentIndex = 0;

    void updateThumbnailList();
};

#endif // PICTURE_H
