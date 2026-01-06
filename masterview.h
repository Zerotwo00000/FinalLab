#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class BookEditView;
class ReaderEditView;
class BorrowedView;

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

    // 读者管理按钮
    void on_btnReaderAdd_clicked();
    void on_btnReaderModify_clicked();
    void on_btnReaderQuery_clicked();
    void on_btnReaderDelete_clicked();

    // 借阅管理按钮
    void on_btnBorrowed_clicked();  // 借阅按钮
    void on_btnBorrowQuery_clicked();  // 查询记录按钮
    void on_btnBorrowDelete_clicked();  // 删除按钮
    void on_btnBorrowReturn_clicked();  // 归还按钮

    // 退出系统
    void on_btnExitSystem_clicked();

private:
    Ui::MasterView *ui;

    // 对话框指针
    BookEditView *bookEditDialog = nullptr;
    ReaderEditView *readerEditDialog = nullptr;
    BorrowedView *borrowedDialog = nullptr;

    void setupNavigation();    // 设置导航
    void setupConnections();   // 设置信号槽连接
    void setupTableViews();    // 设置表格视图属性
    void setupDatabaseModels(); // 设置数据库模型
};

#endif // MASTERVIEW_H
