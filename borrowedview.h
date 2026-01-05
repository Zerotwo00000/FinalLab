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

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 确定按钮点击
    void on_btnBorrowedOK_clicked();

    // 取消按钮点击
    void on_btnBorrowedCancel_clicked();

private:
    Ui::BorrowedView *ui;
};

#endif // BORROWEDVIEW_H
