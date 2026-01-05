#ifndef BOOKEDITVIEW_H
#define BOOKEDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class BookEditView;
}

class BookEditView : public QWidget
{
    Q_OBJECT

public:
    explicit BookEditView(QWidget *parent = nullptr);
    ~BookEditView();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 确定按钮点击
    void on_BtnBookOK_clicked();

    // 取消按钮点击
    void on_BtnBookCancel_clicked();

private:
    Ui::BookEditView *ui;
    QDataWidgetMapper *dataMapper; //数据映射
};

#endif // BOOKEDITVIEW_H
