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

private:
    Ui::BorrowedView *ui;
};

#endif // BORROWEDVIEW_H
