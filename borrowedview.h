#ifndef BORROWEDVIEW_H
#define BORROWEDVIEW_H

#include <QWidget>

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
    void dataUpdated();
    void dialogClosed();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 确定按钮点击
    void on_btnBorrowedOK_clicked();

    // 取消按钮点击
    void on_btnBorrowedCancel_clicked();

    // 书籍选择变化
    void onBookSelected(int index);

private:
    Ui::BorrowedView *ui;

    // 加载可借阅书籍
    void loadAvailableBooks();

    // 检查读者状态
    bool checkReaderStatus(const QString& readerNo);

    // 检查是否今天已借阅过同一本书
    bool checkTodayBorrowed(const QString& readerNo, const QString& isbn, const QDate& borrowDate);
};

#endif // BORROWEDVIEW_H
