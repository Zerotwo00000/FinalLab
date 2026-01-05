#include "borrowedview.h"
#include "ui_borrowedview.h"

BorrowedView::BorrowedView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowedView)
{
    ui->setupUi(this);
}

BorrowedView::~BorrowedView()
{
    delete ui;
}
