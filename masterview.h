#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MasterView; }
QT_END_NAMESPACE

class BookEditView;
class ReaderEditView;
class BorrowedView;

class MasterView : public QMainWindow
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

private slots:
    // 导航按钮
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
    void on_btnBorrowed_clicked();
    void on_btnBorrowQuery_clicked();
    void on_btnBorrowDelete_clicked();
    void on_btnBorrowReturn_clicked();

    // 报表管理按钮
    void on_btnReportQuery_clicked();  // 查询统计
    void exportDataClicked();          // 导出数据 - 修改了函数名

    // 退出系统
    void on_btnExitSystem_clicked();

private:
    Ui::MasterView *ui;

    // 对话框指针
    BookEditView *bookEditDialog = nullptr;
    ReaderEditView *readerEditDialog = nullptr;
    BorrowedView *borrowedDialog = nullptr;

    // 初始化函数
    void setupNavigation();              // 设置导航功能
    void setupConnections();             // 设置信号槽连接
    void setupTableViews();              // 设置表格视图属性
    void setupDatabaseModels();          // 设置数据库模型

    // 统计函数
    void generateReport();                // 生成统计报表
    QString getMostBorrowedBook();       // 获取借阅最多的图书
    QString getLeastBorrowedBook();       // 获取借阅最少的图书
    QString getMostActiveReader();        // 获取借书最多的读者
    QString getLeastActiveReader();       // 获取借书最少的读者

    // 导出函数
    void exportToExcel();                 // 导出到Excel
};

#endif // MASTERVIEW_H
