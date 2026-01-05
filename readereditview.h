#ifndef READEREDITVIEW_H
#define READEREDITVIEW_H

#include <QWidget>

namespace Ui {
class ReaderEditView;
}

class ReaderEditView : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderEditView(QWidget *parent = nullptr);
    ~ReaderEditView();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 确定按钮点击
    void on_BtnReaderOK_clicked();

    // 取消按钮点击
    void on_BtnReaderCancel_clicked();

private:
    Ui::ReaderEditView *ui;
};

#endif // READEREDITVIEW_H
