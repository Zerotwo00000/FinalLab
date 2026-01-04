#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class MasterView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MasterView(QWidget *parent = nullptr);
    ~MasterView();

private slots:
    void on_btnExitSystem_clicked();

private:
    Ui::MasterView *ui;

    void setupNavigation();
    void setupButtonGroups();
};

#endif // MASTERVIEW_H
