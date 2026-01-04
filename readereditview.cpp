#include "readereditview.h"
#include "ui_readereditview.h"

ReaderEditView::ReaderEditView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReaderEditView)
{
    ui->setupUi(this);
}

ReaderEditView::~ReaderEditView()
{
    delete ui;
}
