#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <QWidget>

#include <QWebSocket>
#include <QNetworkAccessManager>
#include <QString>


namespace Ui {
class Communicate;
}

class Communicate : public QWidget
{
    Q_OBJECT

public:
    explicit Communicate(QWidget *parent = nullptr);
    ~Communicate();

signals:
    void toHome();

private slots:
    void on_toHome_clicked();
    void on_sendButton_clicked();
    void onTextMessageReceived(QString message);  // 接收消息时调用
    void on_loginButton_clicked();    // Login button
    void on_registerButton_clicked(); // Register button
    void on_logoutButton_clicked();   // Logout button

    void on_toRegister_clicked();

    void on_toLogin_clicked();


    void on_hideandshow_clicked();

    void on_hideandshow_2_clicked();

private:
    Ui::Communicate *ui;
    QString getUsername() const;
    QString getPassword() const;
    QString getPassword_2() const;
    QWebSocket *webSocket;          // WebSocket对象
    QNetworkAccessManager *networkManager;
    QString token;

    void connectWebSocket();         // Function to connect WebSocket
    void logout();                   // Function to logout
    void appendMessage(const QString &message, const QString &sender);
    QString SHA256Util(QString rawPassword);
};

#endif // COMMUNICATE_H
