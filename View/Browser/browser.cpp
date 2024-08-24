#include "browser.h"
#include "ui_browser.h"

Browser::Browser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Browser)
{
    ui->setupUi(this);
}

Browser::~Browser()
{
    delete ui;
}

void Browser::on_toHome_clicked()
{
    emit toHome();
}

