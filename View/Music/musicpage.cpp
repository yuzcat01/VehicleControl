#include "musicpage.h"
#include "ui_musicpage.h"

MusicPage::MusicPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicPage)
{
    ui->setupUi(this);
}

MusicPage::~MusicPage()
{
    delete ui;
}
void MusicPage::on_backToHome_clicked()
{
    emit toHome();
}
