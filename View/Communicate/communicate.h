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
    QString getUsername() const;
    QString getPassword() const;

signals:
    void toHome();

private slots:
    void on_toHome_clicked();
    void on_sendButton_clicked();
    void onTextMessageReceived(QString message);  // 接收消息时调用
    void on_loginButton_clicked();    // Login button
    void on_registerButton_clicked(); // Register button
    void on_logoutButton_clicked();   // Logout button



private:
    Ui::Communicate *ui;
    QWebSocket *webSocket;          // WebSocket对象
    QNetworkAccessManager *networkManager;
    QString token;

    void connectWebSocket();         // Function to connect WebSocket
    void logout();                   // Function to logout
    void appendMessage(const QString &message, const QString &sender);
};

#endif // COMMUNICATE_H
