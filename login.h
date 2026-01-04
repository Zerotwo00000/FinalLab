#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void loginSuccess();  // 添加登录成功信号

private slots:
    void on_pushButtonLogin_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
