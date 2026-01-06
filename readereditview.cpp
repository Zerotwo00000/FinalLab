#include "readereditview.h"
#include "ui_readereditview.h"
#include "idatabase.h"
#include <QCloseEvent>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>

ReaderEditView::ReaderEditView(QWidget *parent, int currentRow, ReaderEditView::Mode mode)
    : QWidget(parent)
    , ui(new Ui::ReaderEditView)
    , dataMapper(nullptr)
    , currentRow(currentRow)
    , editMode(mode)
{
    ui->setupUi(this);

    // 设置窗口标志，使其成为一个独立的窗口
    setWindowFlags(Qt::Window);

    // 重要：设置关闭时自动删除
    setAttribute(Qt::WA_DeleteOnClose);

    // 根据模式设置窗口标题
    if (editMode == Add) {
        setWindowTitle("添加读者");
    } else {
        setWindowTitle("修改读者");
    }

    // 根据UI中的按钮名称修正连接
    // 注意：确保UI文件中的对象名称是 BtnReaderOK 而不是 BtnReaderOk
    if (ui->BtnReaderOk) {
        connect(ui->BtnReaderOk, &QPushButton::clicked, this, &ReaderEditView::on_BtnReaderOK_clicked);
    }

    if (ui->BtnReaderCancel) {
        connect(ui->BtnReaderCancel, &QPushButton::clicked, this, &ReaderEditView::on_BtnReaderCancel_clicked);
    }

    // 调试输出UI对象名称
    qDebug() << "ReaderEditView UI objects:";
    qDebug() << "  BtnReaderOK exists:" << (ui->BtnReaderOk != nullptr);
    qDebug() << "  BtnReaderCancel exists:" << (ui->BtnReaderCancel != nullptr);
    qDebug() << "  lineEditReaderNo exists:" << (ui->lineEditReaderNo != nullptr);
    qDebug() << "  lineEditName exists:" << (ui->lineEditName != nullptr);
    qDebug() << "  lineEditPhone exists:" << (ui->lineEditPhone != nullptr);
    qDebug() << "  lineEditEmail exists:" << (ui->lineEditEmail != nullptr);
    qDebug() << "  lineEditIdCard exists:" << (ui->lineEditIdCard != nullptr);
    qDebug() << "  comboBoxStatus exists:" << (ui->comboBoxStatus != nullptr);

    // 根据UI中的实际名称修正spinBox名称
    // 如果UI中spinBox名称不同，需要调整这里
    QSpinBox *maxBorrowSpinBox = nullptr;
    QSpinBox *currentBorrowSpinBox = nullptr;

    // 尝试查找UI中的spinBox控件
    foreach (QSpinBox *spinBox, this->findChildren<QSpinBox *>()) {
        QString objectName = spinBox->objectName();
        qDebug() << "Found spinBox:" << objectName;
        if (objectName.contains("MaxBorrow", Qt::CaseInsensitive)) {
            maxBorrowSpinBox = spinBox;
        } else if (objectName.contains("CurrentBorrow", Qt::CaseInsensitive)) {
            currentBorrowSpinBox = spinBox;
        }
    }

    // 调试输出spinBox状态
    qDebug() << "Max borrow spinBox found:" << (maxBorrowSpinBox != nullptr);
    qDebug() << "Current borrow spinBox found:" << (currentBorrowSpinBox != nullptr);

    // 如果UI中没有这些控件，我们需要创建动态的隐藏字段
    if (!maxBorrowSpinBox) {
        qDebug() << "没有找到最大借书数SpinBox，创建隐藏控件";
        // 创建隐藏的spinBox
        maxBorrowSpinBox = new QSpinBox(this);
        maxBorrowSpinBox->setObjectName("maxBorrowLimit");
        maxBorrowSpinBox->setVisible(false);
        maxBorrowSpinBox->setRange(1, 20);
        maxBorrowSpinBox->setValue(5);
    }

    if (!currentBorrowSpinBox) {
        qDebug() << "没有找到当前借书数SpinBox，创建隐藏控件";
        // 创建隐藏的spinBox
        currentBorrowSpinBox = new QSpinBox(this);
        currentBorrowSpinBox->setObjectName("currentBorrowed");
        currentBorrowSpinBox->setVisible(false);
        currentBorrowSpinBox->setRange(0, 20);
        currentBorrowSpinBox->setValue(0);
    }

    // 初始化数据映射
    if (editMode == Edit && currentRow >= 0) {
        dataMapper = new QDataWidgetMapper(this);
        dataMapper->setModel(IDatabase::getInstance().readerTabModel);

        // 设置映射关系 - 根据数据库表中的字段顺序
        dataMapper->addMapping(ui->lineEditReaderNo, 0);        // 读者编号
        dataMapper->addMapping(ui->lineEditName, 1);            // 姓名
        dataMapper->addMapping(ui->lineEditPhone, 2);           // 电话
        dataMapper->addMapping(ui->lineEditEmail, 3);           // 邮箱
        dataMapper->addMapping(ui->lineEditIdCard, 4);          // 身份证号
        dataMapper->addMapping(maxBorrowSpinBox, 5);           // 最大借书数
        dataMapper->addMapping(currentBorrowSpinBox, 6);       // 当前借书数
        dataMapper->addMapping(ui->comboBoxStatus, 7);         // 状态
        // 第8列是register_date，但由系统自动生成，不需要映射

        // 设置当前行
        dataMapper->setCurrentIndex(currentRow);

        // 修改模式下，读者编号不允许编辑
        ui->lineEditReaderNo->setReadOnly(true);

        // 修改模式下，当前借书数应该从数据库中读取，不允许修改
        currentBorrowSpinBox->setReadOnly(true);
    } else {
        // 添加模式下，清空所有字段
        ui->lineEditReaderNo->clear();
        ui->lineEditName->clear();
        ui->lineEditPhone->clear();
        ui->lineEditEmail->clear();
        ui->lineEditIdCard->clear();

        // 设置默认值
        if (maxBorrowSpinBox) {
            maxBorrowSpinBox->setValue(5);  // 默认最大借5本
        }

        if (currentBorrowSpinBox) {
            currentBorrowSpinBox->setValue(0);  // 当前借书数为0
        }

        ui->comboBoxStatus->setCurrentIndex(0);  // 默认状态为正常

        // 添加模式下，当前借书数不允许编辑
        if (currentBorrowSpinBox) {
            currentBorrowSpinBox->setReadOnly(true);
        }
    }
}

ReaderEditView::~ReaderEditView()
{
    qDebug() << "读者编辑对话框被销毁";
    delete ui;
    if (dataMapper) {
        delete dataMapper;
    }
}

void ReaderEditView::closeEvent(QCloseEvent *event)
{
    // 发射对话框关闭信号
    emit dialogClosed();

    // 关闭窗口
    QWidget::closeEvent(event);
}

// 确定按钮点击
void ReaderEditView::on_BtnReaderOK_clicked()
{
    // 获取表单数据
    QString readerNo = ui->lineEditReaderNo->text().trimmed();
    QString name = ui->lineEditName->text().trimmed();
    QString phone = ui->lineEditPhone->text().trimmed();
    QString email = ui->lineEditEmail->text().trimmed();
    QString idCard = ui->lineEditIdCard->text().trimmed();

    // 查找spinBox控件
    QSpinBox *maxBorrowSpinBox = this->findChild<QSpinBox *>("maxBorrowLimit");
    QSpinBox *currentBorrowSpinBox = this->findChild<QSpinBox *>("currentBorrowed");

    int maxBorrowLimit = 5;  // 默认值
    int currentBorrowed = 0;  // 默认值

    if (maxBorrowSpinBox) {
        maxBorrowLimit = maxBorrowSpinBox->value();
    }

    if (currentBorrowSpinBox) {
        currentBorrowed = currentBorrowSpinBox->value();
    }

    QString status = ui->comboBoxStatus->currentText();

    // 注册日期由系统自动生成
    QString registerDate = QDate::currentDate().toString("yyyy-MM-dd");

    // 简单的表单验证
    if (readerNo.isEmpty() || name.isEmpty()) {
        qDebug() << "读者编号和姓名是必填项";
        return;
    }

    // 验证读者编号格式
    if (readerNo.length() < 1 || readerNo.length() > 20) {
        qDebug() << "读者编号长度应为1-20个字符";
        return;
    }

    // 验证姓名长度
    if (name.length() < 1 || name.length() > 50) {
        qDebug() << "姓名长度应为1-50个字符";
        return;
    }

    // 验证电话格式
    if (!phone.isEmpty() && phone.length() != 11) {
        qDebug() << "电话号码应为11位";
        return;
    }

    // 验证身份证号格式
    if (!idCard.isEmpty() && (idCard.length() != 15 && idCard.length() != 18)) {
        qDebug() << "身份证号应为15位或18位";
        return;
    }

    // 验证最大借书数
    if (maxBorrowLimit < 1 || maxBorrowLimit > 20) {
        qDebug() << "最大借书数应为1-20";
        return;
    }

    // 验证当前借书数
    if (currentBorrowed < 0 || currentBorrowed > maxBorrowLimit) {
        qDebug() << "当前借书数不能超过最大借书数，且不能为负数";
        return;
    }

    if (editMode == Add) {
        // 检查读者编号是否已存在
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM readers WHERE reader_no = ?");
        checkQuery.addBindValue(readerNo);

        if (checkQuery.exec() && checkQuery.next()) {
            int count = checkQuery.value(0).toInt();
            if (count > 0) {
                qDebug() << "读者编号已存在:" << readerNo;
                return;
            }
        } else {
            qDebug() << "检查读者编号失败:" << checkQuery.lastError().text();
        }

        // 检查身份证号是否已存在
        if (!idCard.isEmpty()) {
            checkQuery.prepare("SELECT COUNT(*) FROM readers WHERE id_card = ?");
            checkQuery.addBindValue(idCard);

            if (checkQuery.exec() && checkQuery.next()) {
                int count = checkQuery.value(0).toInt();
                if (count > 0) {
                    qDebug() << "身份证号已存在:" << idCard;
                    return;
                }
            } else {
                qDebug() << "检查身份证号失败:" << checkQuery.lastError().text();
            }
        }

        // 添加新记录
        QSqlQuery insertQuery;
        insertQuery.prepare(
            "INSERT INTO readers (reader_no, name, phone, email, id_card, "
            "max_borrow_limit, current_borrowed, status, register_date) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
            );

        insertQuery.addBindValue(readerNo);
        insertQuery.addBindValue(name);
        insertQuery.addBindValue(phone);
        insertQuery.addBindValue(email);
        insertQuery.addBindValue(idCard);
        insertQuery.addBindValue(maxBorrowLimit);
        insertQuery.addBindValue(currentBorrowed);
        insertQuery.addBindValue(status);
        insertQuery.addBindValue(registerDate);

        if (insertQuery.exec()) {
            qDebug() << "添加读者成功，读者编号:" << readerNo;
            // 发射数据更新信号
            emit dataUpdated();
        } else {
            qDebug() << "添加读者失败:" << insertQuery.lastError().text();
        }

    } else if (editMode == Edit) {
        // 修改现有记录
        if (dataMapper) {
            // 通过数据映射器提交修改
            if (dataMapper->submit()) {
                // 提交到数据库
                if (IDatabase::getInstance().readerTabModel->submitAll()) {
                    qDebug() << "修改读者成功，读者编号:" << readerNo;
                    // 发射数据更新信号
                    emit dataUpdated();
                } else {
                    qDebug() << "保存修改失败:" << IDatabase::getInstance().readerTabModel->lastError().text();
                    IDatabase::getInstance().readerTabModel->revertAll();
                }
            } else {
                qDebug() << "修改读者失败:" << IDatabase::getInstance().readerTabModel->lastError().text();
            }
        } else {
            // 如果没有dataMapper，直接通过SQL更新
            QSqlQuery updateQuery;
            updateQuery.prepare(
                "UPDATE readers SET "
                "name = ?, phone = ?, email = ?, id_card = ?, "
                "max_borrow_limit = ?, current_borrowed = ?, status = ? "
                "WHERE reader_no = ?"
                );

            updateQuery.addBindValue(name);
            updateQuery.addBindValue(phone);
            updateQuery.addBindValue(email);
            updateQuery.addBindValue(idCard);
            updateQuery.addBindValue(maxBorrowLimit);
            updateQuery.addBindValue(currentBorrowed);
            updateQuery.addBindValue(status);
            updateQuery.addBindValue(readerNo);

            if (updateQuery.exec()) {
                qDebug() << "修改读者成功，读者编号:" << readerNo;
                IDatabase::getInstance().readerTabModel->select();
                emit dataUpdated();
            } else {
                qDebug() << "修改读者失败:" << updateQuery.lastError().text();
            }
        }
    }

    // 关闭窗口
    close();
}

// 取消按钮点击
void ReaderEditView::on_BtnReaderCancel_clicked()
{
    if (editMode == Edit && dataMapper) {
        // 取消修改
        dataMapper->revert();
    }

    close();
}
