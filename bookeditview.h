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
    // 添加模式参数
    enum Mode { Add, Edit };

    explicit BookEditView(QWidget *parent = nullptr, int currentRow = -1, BookEditView::Mode mode = Add);
    ~BookEditView();

signals:
    void dataUpdated();  // 数据更新信号

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 确定按钮点击
    void on_BtnBookOK_clicked();

    // 取消按钮点击
    void on_BtnBookCancel_clicked();

private:
    Ui::BookEditView *ui;
    QDataWidgetMapper *dataMapper; // 数据映射
    int currentRow; // 当前行号
    Mode editMode;  // 编辑模式
};

#endif // BOOKEDITVIEW_H
