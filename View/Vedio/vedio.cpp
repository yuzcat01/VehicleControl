#include "vedio.h"
#include "ui_vedio.h"

Vedio::Vedio(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Vedio)
{
    ui->setupUi(this);
}

Vedio::~Vedio()
{
    delete ui;
}

void Vedio::on_toHome_clicked()
{
     emit toHome();
}

