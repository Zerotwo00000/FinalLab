#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class BookEditView;  // 前向声明

class MasterView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MasterView(QWidget *parent = nullptr);
    ~MasterView();

private slots:
    // 导航按钮点击槽函数
    void on_btnBookManage_clicked();
    void on_btnReaderManage_clicked();
    void on_btnBorrowManage_clicked();
    void on_btnReportManage_clicked();

    // 图书管理按钮
    void on_btnBookAdd_clicked();
    void on_btnBookModify_clicked();
    void on_btnBookQuery_clicked();
    void on_btnBookDelete_clicked();

    // 退出系统
    void on_btnExitSystem_clicked();

private:
    Ui::MasterView *ui;
    BookEditView *bookEditDialog;  // 只保留一个对话框实例

    void setupNavigation();    // 设置导航
    void setupConnections();   // 设置信号槽连接
};

#endif // MASTERVIEW_H
