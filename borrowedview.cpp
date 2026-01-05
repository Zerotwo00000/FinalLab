#include "borrowedview.h"
#include "ui_borrowedview.h"
#include <QCloseEvent>
#include <QDate>

BorrowedView::BorrowedView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowedView)
{
    ui->setupUi(this);

    // 设置窗口标志，使其成为一个独立的窗口
    setWindowFlags(Qt::Window);

    // 设置初始日期为当前日期
    QDate currentDate = QDate::currentDate();
    ui->dateEditBorrowDate->setDate(currentDate);
    ui->dateEditDueDate->setDate(currentDate.addDays(30));  // 默认借阅30天

    // 连接按钮信号
    connect(ui->btnBorrowedOK, &QPushButton::clicked, this, &BorrowedView::on_btnBorrowedOK_clicked);
    connect(ui->btnBorrowedCancel, &QPushButton::clicked, this, &BorrowedView::on_btnBorrowedCancel_clicked);
}

BorrowedView::~BorrowedView()
{
    delete ui;
}

void BorrowedView::closeEvent(QCloseEvent *event)
{
    // 关闭窗口时发出destroyed信号
    QWidget::closeEvent(event);
}

// 确定按钮点击
void BorrowedView::on_btnBorrowedOK_clicked()
{
    // 获取表单数据
    QString readerNo = ui->lineEditBorrowedReaderNo->text();
    QString bookId = ui->lineEditBorrowedBookId->text();

    // 简单的表单验证
    if (readerNo.isEmpty() || bookId.isEmpty()) {
        // 可以设置输入框样式提示错误，但不弹窗
        return;
    }

    // 检查借阅日期是否小于等于应还日期
    QDate borrowDate = ui->dateEditBorrowDate->date();
    QDate dueDate = ui->dateEditDueDate->date();

    if (borrowDate > dueDate) {
        // 借阅日期不能晚于应还日期
        return;
    }

    // 保存成功，不显示提示
    close();
}

// 取消按钮点击
void BorrowedView::on_btnBorrowedCancel_clicked()
{
    close();
}
