#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class IDatabase : public QObject
{
    Q_OBJECT
public:

    static IDatabase &getInstance()
    {
        static IDatabase instance;
        return instance;
    }

private:
    explicit IDatabase(QObject *parent = nullptr);
    IDatabase(IDatabase const&)               = delete;
    void operator=(IDatabase const&)  = delete;

    QSqlDatabase database;
    void ininDatabase();

signals:

public:
    // 图书模型
    bool initBookModel();
    bool queryBook(QString Bookfilter);
    bool deleteCurrentBook();
    bool submitBookEdit();
    void revertBookEdit();
    QSqlTableModel *bookTabModel;
    QItemSelectionModel *theBookSelection;

    // 读者模型
    bool initReaderModel();
    bool queryReader(QString Readerfilter);
    bool deleteCurrentReader();
    bool submitReaderEdit();
    void revertReaderEdit();
    QSqlTableModel *readerTabModel;
    QItemSelectionModel *theReaderSelection;

    // 借阅模型
    bool initBorrowModel();
    QSqlTableModel *borrowTabModel;
    QItemSelectionModel *theBorrowSelection;
};

#endif // IDATABASE_H
