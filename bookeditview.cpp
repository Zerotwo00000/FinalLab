#include "bookeditview.h"
#include "ui_bookeditview.h"
#include "idatabase.h"
#include <QCloseEvent>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include <QDate>

BookEditView::BookEditView(QWidget *parent, int currentRow, BookEditView::Mode mode)
    : QWidget(parent)
    , ui(new Ui::BookEditView)
    , dataMapper(nullptr)
    , currentRow(currentRow)
    , editMode(mode)
{
    ui->setupUi(this);

    // 设置窗口标志，使其成为一个独立的窗口
    setWindowFlags(Qt::Window);

    // 根据模式设置窗口标题
    if (editMode == Add) {
        setWindowTitle("添加图书");
    } else {
        setWindowTitle("修改图书");
    }

    // 连接按钮信号
    connect(ui->BtnBookOK, &QPushButton::clicked, this, &BookEditView::on_BtnBookOK_clicked);
    connect(ui->BtnBookCancel, &QPushButton::clicked, this, &BookEditView::on_BtnBookCancel_clicked);

    // 初始化数据映射
    if (editMode == Edit && currentRow >= 0) {
        dataMapper = new QDataWidgetMapper(this);
        dataMapper->setModel(IDatabase::getInstance().bookTabModel);

        // 设置映射关系
        dataMapper->addMapping(ui->lineEditIsbn, 0);             // ISBN
        dataMapper->addMapping(ui->lineEditTitle, 1);            // 书名
        dataMapper->addMapping(ui->lineEditAuthor, 2);           // 作者
        dataMapper->addMapping(ui->lineEditPublisher, 3);        // 出版社
        dataMapper->addMapping(ui->lineEditYear, 4);             // 出版年份
        dataMapper->addMapping(ui->comboBoxCategory, 5);         // 分类
        dataMapper->addMapping(ui->spinBoxTotalCopies, 6);       // 总数量
        dataMapper->addMapping(ui->spinBoxAvailableCopies, 7);   // 可借数量
        dataMapper->addMapping(ui->lineEditLocation, 8);         // 位置

        // 设置当前行
        dataMapper->setCurrentIndex(currentRow);

        // 修改模式下，ISBN不允许编辑
        ui->lineEditIsbn->setReadOnly(true);
    } else {
        // 添加模式下，清空所有字段
        ui->lineEditIsbn->clear();
        ui->lineEditTitle->clear();
        ui->lineEditAuthor->clear();
        ui->lineEditPublisher->clear();

        // 设置当前年份为默认值
        int currentYear = QDate::currentDate().year();
        ui->lineEditYear->setText(QString::number(currentYear));

        ui->comboBoxCategory->setCurrentIndex(0);
        ui->spinBoxTotalCopies->setValue(1);
        ui->spinBoxAvailableCopies->setValue(1);
        ui->lineEditLocation->clear();
    }
}

BookEditView::~BookEditView()
{
    delete ui;
    if (dataMapper) {
        delete dataMapper;
    }
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
    QString isbn = ui->lineEditIsbn->text().trimmed();
    QString title = ui->lineEditTitle->text().trimmed();
    QString author = ui->lineEditAuthor->text().trimmed();
    QString publisher = ui->lineEditPublisher->text().trimmed();
    QString publishYearStr = ui->lineEditYear->text().trimmed();
    QString category = ui->comboBoxCategory->currentText();
    int totalCopies = ui->spinBoxTotalCopies->value();
    int availableCopies = ui->spinBoxAvailableCopies->value();
    QString location = ui->lineEditLocation->text().trimmed();

    // 简单的表单验证
    if (isbn.isEmpty() || title.isEmpty() || author.isEmpty()) {
        // 可以设置输入框样式提示错误，但不弹窗
        return;
    }

    // 检查可借数量是否大于总数量
    if (availableCopies > totalCopies) {
        // 不弹窗提示，直接设置可借数量为总数量
        availableCopies = totalCopies;
        ui->spinBoxAvailableCopies->setValue(availableCopies);
    }

    // 检查出版年份是否合理
    bool ok;
    int publishYear = publishYearStr.toInt(&ok);
    int currentYear = QDate::currentDate().year();

    if (!ok || publishYear < 0 || publishYear > currentYear + 1) {
        // 年份不合理，设置默认值
        publishYear = currentYear;
        ui->lineEditYear->setText(QString::number(publishYear));
    }

    if (editMode == Add) {
        // 检查ISBN是否已存在
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM books WHERE isbn = ?");
        checkQuery.addBindValue(isbn);

        if (checkQuery.exec() && checkQuery.next()) {
            int count = checkQuery.value(0).toInt();
            if (count > 0) {
                // ISBN已存在，不弹窗，直接返回
                return;
            }
        }

        // 添加新记录
        QSqlQuery insertQuery;
        insertQuery.prepare(
            "INSERT INTO books (isbn, title, author, publisher, publish_year, "
            "category, total_copies, available_copies, location, add_date) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, DATE('now'))"
            );

        insertQuery.addBindValue(isbn);
        insertQuery.addBindValue(title);
        insertQuery.addBindValue(author);
        insertQuery.addBindValue(publisher);
        insertQuery.addBindValue(publishYear);
        insertQuery.addBindValue(category);
        insertQuery.addBindValue(totalCopies);
        insertQuery.addBindValue(availableCopies);
        insertQuery.addBindValue(location);

        if (insertQuery.exec()) {
            qDebug() << "添加图书成功，ISBN:" << isbn;
            // 发射数据更新信号
            emit dataUpdated();
        } else {
            qDebug() << "添加图书失败:" << insertQuery.lastError().text();
        }

    } else if (editMode == Edit) {
        // 修改现有记录
        if (dataMapper) {
            // 通过数据映射器提交修改
            if (dataMapper->submit()) {
                // 提交到数据库
                if (IDatabase::getInstance().bookTabModel->submitAll()) {
                    qDebug() << "修改图书成功，ISBN:" << isbn;
                    // 发射数据更新信号
                    emit dataUpdated();
                } else {
                    qDebug() << "保存修改失败:" << IDatabase::getInstance().bookTabModel->lastError().text();
                    IDatabase::getInstance().bookTabModel->revertAll();
                }
            } else {
                qDebug() << "修改图书失败:" << IDatabase::getInstance().bookTabModel->lastError().text();
            }
        }
    }

    // 关闭窗口
    close();
}

// 取消按钮点击
void BookEditView::on_BtnBookCancel_clicked()
{
    if (editMode == Edit && dataMapper) {
        // 取消修改
        dataMapper->revert();
    }

    close();
}

