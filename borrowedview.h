#ifndef BORROWEDVIEW_H
#define BORROWEDVIEW_H

#include <QWidget>
#include <QSqlQuery>

namespace Ui {
class BorrowedView;
}

class BorrowedView : public QWidget
{
    Q_OBJECT

public:
    explicit BorrowedView(QWidget *parent = nullptr);
    ~BorrowedView();

signals:
    void dataUpdated();  // 数据更新信号
    void dialogClosed();  // 对话框关闭信号

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 确定按钮点击
    void on_btnBorrowedOK_clicked();

    // 取消按钮点击
    void on_btnBorrowedCancel_clicked();

    // 书籍选择变化
    void on_comboBoxBorrow_currentIndexChanged(int index);

private:
    Ui::BorrowedView *ui;

    // 初始化下拉框
    void initComboBox();

    // 显示书籍信息
    void showBookInfo(const QString& isbn);

    // 验证借阅条件
    bool validateBorrowCondition(const QString& readerNo, const QString& isbn);

    // 当前选择的ISBN
    QString selectedIsbn;
};

#endif // BORROWEDVIEW_H
