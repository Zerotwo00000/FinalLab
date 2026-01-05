#include "bookeditview.h"
#include "ui_bookeditview.h"
#include <QCloseEvent>

BookEditView::BookEditView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BookEditView)
{
    ui->setupUi(this);

    // 设置窗口标志，使其成为一个独立的窗口
    setWindowFlags(Qt::Window);

    // 连接按钮信号
    connect(ui->BtnBookOK, &QPushButton::clicked, this, &BookEditView::on_BtnBookOK_clicked);
    connect(ui->BtnBookCancel, &QPushButton::clicked, this, &BookEditView::on_BtnBookCancel_clicked);
}

BookEditView::~BookEditView()
{
    delete ui;
}

void BookEditView::closeEvent(QCloseEvent *event)
{
    // 关闭窗口时发出destroyed信号
    QWidget::closeEvent(event);
}

// 确定按钮点击
void BookEditView::on_BtnBookOK_clicked()
{
    // 获取表单数据
    QString isbn = ui->lineEditIsbn->text();
    QString title = ui->lineEditTitle->text();
    QString author = ui->lineEditAuthor->text();

    // 简单的表单验证
    if (isbn.isEmpty() || title.isEmpty() || author.isEmpty()) {
        // 可以设置输入框样式提示错误，但不弹窗
        return;
    }

    // 保存成功，不显示提示
    close();
}

// 取消按钮点击
void BookEditView::on_BtnBookCancel_clicked()
{
    close();
}
