#include "login.h"
#include "masterview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建登录界面
    Login loginWindow;

    // 创建主界面指针（先不创建）
    MasterView *mainWindow = nullptr;

    // 连接登录成功信号
    QObject::connect(&loginWindow, &Login::loginSuccess,
                     [&loginWindow, &mainWindow]() {
                         // 隐藏登录窗口
                         loginWindow.hide();

                         // 创建主界面
                         if (!mainWindow) {
                             mainWindow = new MasterView();
                         }

                         // 显示主界面
                         mainWindow->show();

                         // 连接主界面关闭信号到应用程序退出
                         QObject::connect(mainWindow, &MasterView::destroyed,
                                          QApplication::instance(), &QApplication::quit);
                     });

    // 显示登录界面
    loginWindow.show();

    return a.exec();
}
