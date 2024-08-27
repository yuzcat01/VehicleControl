#include "picture.h"
#include "ui_picture.h"

#include <QMessageBox> // For displaying prompts
#include "customlabel.h"

Picture::Picture(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Picture)
    , currentIndex(-1)
{
    ui->setupUi(this);
}

Picture::~Picture()
{
    delete ui;
}

void Picture::on_toHome_clicked()
{
     emit toHome();
}


void Picture::on_selectButton_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Images", QString(),
                                                      "Images (*.png *.jpg *.bmp *.gif *.tiff *.xpm *.jpeg)");
    if (!files.isEmpty()) {
        imagePaths.append(files);
        updateThumbnailList();

        showImage(0);

    }
}

void Picture::showImage(int index) {
    if (index >= 0 && index < imagePaths.size()) {
        // 获取 scrollArea 中的所有子部件（即 CustomLabel）
        QList<CustomLabel*> labels = ui->previewArea->widget()->findChildren<CustomLabel*>();

        // 遍历所有的 CustomLabel，恢复默认边框颜色
        for (int i = 0; i < labels.size(); ++i) {
            CustomLabel *label = labels.at(i);
            if (label) {
                if (i == index) {
                    // 当前选中的图片设置为红色边框
                    label->setStyleSheet("border: 2px solid red;");
                } else {
                    // 其他图片恢复为默认灰色边框
                    label->setStyleSheet("border: 2px solid gray;");
                }
            }
        }

        // 更新当前索引并显示选中的图片
        currentIndex = index;
        QPixmap pixmap(imagePaths[index]);
        ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));
    }
}




void Picture::on_prevButton_clicked()
{
    if (currentIndex > 0) {
        showImage(--currentIndex);
    } else {
        showImage(imagePaths.size() - 1); // Go to the last image
        QMessageBox::information(this, "列表起始", "已查看过第一张图片，自动回到最后一张");
    }
}


void Picture::on_nextButton_clicked()
{
    if (currentIndex < imagePaths.size() - 1) {
        showImage(++currentIndex);
    } else {
        showImage(0); // Go back to the first image
        QMessageBox::information(this, "列表结束", "已查看过最后张图片，自动回到第一张");
    }
}



void Picture::on_removeButton_clicked()
{
    if (!imagePaths.isEmpty() && currentIndex >= 0 && currentIndex < imagePaths.size()) {
        imagePaths.removeAt(currentIndex);
        updateThumbnailList();
        if (currentIndex >= imagePaths.size()) {
            currentIndex = imagePaths.size() - 1;
        }
        showImage(currentIndex);
    }
}

void Picture::updateThumbnailList() {
    // 创建一个新的 QWidget 作为 ScrollArea 的内容
    QWidget *container = new QWidget(this);
    QBoxLayout *layout = new QHBoxLayout(container);

    // 遍历所有图像路径，并为每个图像创建一个 CustomLabel
    for (int i = 0; i < imagePaths.size(); ++i) {
        QPixmap pixmap(imagePaths[i]);
        CustomLabel *thumbnailLabel = new CustomLabel(this);  // 使用 CustomLabel
        thumbnailLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio)); // 调整缩略图大小
        thumbnailLabel->setFixedSize(100, 100); // 确保标签有固定大小
        thumbnailLabel->setFrameStyle(QFrame::Box);
        thumbnailLabel->setStyleSheet("border: 2px solid gray;");
        connect(thumbnailLabel, &CustomLabel::clicked, [=]() {
            showImage(i);
        });

        // 将缩略图添加到 layout 中
        layout->addWidget(thumbnailLabel);
    }

    // 设置容器的布局
    container->setLayout(layout);
    // 将容器设置为 ScrollArea 的内容
    ui->previewArea->setWidget(container);
}
