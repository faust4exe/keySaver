#ifndef MYCRYPTOR_H
#define MYCRYPTOR_H

#include <QObject>

class Item{
public:
    QString login;
    QString password;
    QString description;
};

class MyCryptor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString key READ key WRITE setKey)

public:
    MyCryptor(QObject *parent = 0);

    Q_INVOKABLE void loadItems();

    Q_INVOKABLE void loadItemsWithKey(QString key);

    Q_INVOKABLE void addPair(QString login, QString password, QString description);

    Q_INVOKABLE void saveItems();

    Q_INVOKABLE void saveItemsWithKey(QString key);

    Q_INVOKABLE void clearAllPairs();

    QString key() const
    {
        return m_key;
    }

public slots:
    void setKey(QString arg)
    {
        m_key = arg;
    }

signals:
    void pairDecripted(QString login, QString password, QString description);

private:
    void emitSignalForItem(Item *item);

    QString cryptValue (QString value, QString key);
    QString decryptValue (QString value, QString key);

    char cyclicXOR(char data, char key);

    char moveUp(quint8 data, int key);
    char moveDown(quint8 data, int key);

    int numberOfBitsInChar(char data);

    int keyLenght() const;

private:
    QList<Item*> m_items;

    QString m_key;
    int m_rows;

    bool m_useMove;
    bool m_useXOR;
    bool m_debug;
};

#endif // MYCRYPTOR_H
