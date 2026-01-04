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

private:
    Ui::ReaderEditView *ui;
};

#endif // READEREDITVIEW_H
