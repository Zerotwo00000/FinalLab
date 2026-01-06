#ifndef READEREDITVIEW_H
#define READEREDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class ReaderEditView;
}

class ReaderEditView : public QWidget
{
    Q_OBJECT

public:
    // 添加模式参数枚举
    enum Mode { Add, Edit };

    explicit ReaderEditView(QWidget *parent = nullptr, int currentRow = -1, ReaderEditView::Mode mode = Add);
    ~ReaderEditView();

signals:
    void dataUpdated();  // 数据更新信号
    void dialogClosed();  // 对话框关闭信号

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 确定按钮点击
    void on_BtnReaderOK_clicked();

    // 取消按钮点击
    void on_BtnReaderCancel_clicked();

private:
    Ui::ReaderEditView *ui;
    QDataWidgetMapper *dataMapper; // 数据映射
    int currentRow; // 当前行号
    Mode editMode;  // 编辑模式
};

#endif // READEREDITVIEW_H
