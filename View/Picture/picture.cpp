#include "picture.h"
#include "ui_picture.h"

Picture::Picture(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Picture)
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

