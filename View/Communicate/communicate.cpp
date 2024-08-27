#include "communicate.h"
#include "ui_communicate.h"
#include <QTimer>

Communicate::Communicate(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Communicate)
{
    ui->setupUi(this);
}

Communicate::~Communicate()
{
    delete ui;
}

void Communicate::on_toHome_clicked()
{
     emit toHome();
}

void Communicate::on_sendButton_clicked()
{
    // 获取输入框中的消息
    QString message = ui->messageInput->text();

    if(!message.isEmpty()) {
        // 显示消息
        appendMessage(message, "Me");

        // 清空输入框
        ui->messageInput->clear();

        // 模拟回复
        QTimer::singleShot(1000, [this](){
            appendMessage("This is a reply.", "Chatbot");
        });
    }
}

void Communicate::appendMessage(const QString &message, const QString &sender)
{
    QString formattedMessage = QString("<b>%1:</b> %2").arg(sender, message);
    ui->chatDisplay->append(formattedMessage);
}
