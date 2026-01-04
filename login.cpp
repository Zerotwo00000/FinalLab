#include "login.h"
#include "ui_login.h"
#include "masterview.h"  // 包含主界面头文件
#include <QMessageBox>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("图书借阅管理系统 - 登录");

    // 连接登录按钮点击信号
    connect(ui->pushButtonLogin, &QPushButton::clicked,
            this, &Login::on_pushButtonLogin_clicked);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButtonLogin_clicked()
{
    QString username = ui->lineEditUsername->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();

    // 简单的登录验证
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "用户名和密码不能为空！");
        return;
    }

    // 验证用户名和密码（这里使用简单验证，实际项目中应该连接数据库）
    if (username == "admin" && password == "admin") {
        // 发射登录成功信号
        emit loginSuccess();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}
