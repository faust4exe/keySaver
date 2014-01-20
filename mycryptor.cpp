#include "mycryptor.h"

#include <QDebug>
#include <QSettings>
#include <QStringList>
#include <QMessageBox>

#include <math.h>

MyCryptor::MyCryptor(QObject *parent)
    : QObject(parent)
{
    m_useMove = true;
    m_useXOR = true;
    m_debug = true;

    // self testing
    qDebug() << "Testing XOR";
    char a = (char)(128 + 32 + 8 + 2); // 170
    char b = (char)(64 + 16 + 4 + 1); // 85
    char c = cyclicXOR(a, b);
    qDebug() << "c must be 255, but is " << (quint8)c << "  it is " << (c == a+b);
    char d = cyclicXOR(c, b);
    qDebug() << "d must be 170, but is " << (quint8)d << "  it is " << (d == a);

    qDebug() << "Testing MOVE";
    char e  = moveUp(a, 1);
    qDebug() << "e must be 85, but is " << (quint8)e << "  it is " << (e == b);

    char f  = moveUp(a, 2);
    qDebug() << "f must be 170, but is " << (quint8)f << "  it is " << (f == a);

    char g = 1;
    char h  = moveUp(g, 1);
    qDebug() << "h must be 2, but is " << (quint8)h << "  it is " << (h == 2);

    char i  = moveUp((char)2, 8);
    qDebug() << "i must be 2, but is " << (quint8)i << "  it is " << (i == 2);

    char j  = moveUp((char)2, 3);
    qDebug() << "i must be 16, but is " << (quint8)j << "  it is " << (j == 16);

    char k  = moveDown((char)16, 8);
    qDebug() << "k must be 16, but is " << (quint8)k << "  it is " << (k == 16);

    char l  = moveDown((char)16, 2);
    qDebug() << "l must be 4, but is " << (quint8)l << "  it is " << (l == 4);

}

void MyCryptor::loadItems()
{
    QSettings settings("fabulaGames", "KeySaver");

    if (m_debug)
        qDebug() << "Loading groups : " << settings.childGroups();

    foreach(QString index, settings.childGroups()) {
        Item *item = new Item();

        settings.beginGroup(index);
        item->login = settings.value("login", "").toString();
        item->description = settings.value("description", "").toString();
        item->password = settings.value("password", "").toString();
        settings.endGroup();

        m_items.append(item);

        emitSignalForItem(item);
    }
}

void MyCryptor::loadItemsWithKey(QString thekey)
{
    setKey(thekey);
    QSettings settings("fabulaGames", "KeySaver");

    if (m_debug)
        qDebug() << "On load we have groups : " << settings.childGroups();

    foreach(QString index, settings.childGroups()) {
        Item *item = new Item();

        settings.beginGroup(index);
        item->login = decryptValue(settings.value("login", "").toString(), key());
        item->description = decryptValue(settings.value("description", "").toString(), key());
        item->password = decryptValue(settings.value("password", "").toString(), key());
        settings.endGroup();

        m_items.append(item);

        emitSignalForItem(item);
    }
}

void MyCryptor::addPair(QString login, QString password, QString description)
{
    emit pairDecripted(login, password, description);

    QSettings settings("fabulaGames", "KeySaver");

    // need to crypt value
    QString testCrypt = cryptValue(login, key());
    QString testDesrypt = decryptValue(testCrypt, key());
    if (testCrypt != testDesrypt)
        qDebug() << "### Warning!!! The code-decode is WRONG !!!";

    settings.beginGroup(QString::number(m_items.count()+1));
    settings.setValue("login", cryptValue(login, key()));
    settings.setValue("password", cryptValue(password, key()));
    settings.setValue("description", cryptValue(description, key()));
    settings.endGroup();

    Item *item = new Item();

    item->login = login;
    item->description = description;
    item->password = password;

    m_items.append(item);

    if (m_debug)
        qDebug() << "After Add we have groups : " << settings.childGroups();
}

void MyCryptor::saveItems()
{
    QSettings settings("fabulaGames", "KeySaver");

    int index = 1;
    foreach(Item *item, m_items) {
        settings.beginGroup(QString::number(index));
        settings.setValue("login", item->login);
        settings.setValue("password", item->password);
        settings.setValue("description", item->description);
        settings.endGroup();
        index++;
    }
}

void MyCryptor::saveItemsWithKey(QString key)
{
    setKey(key);

    QSettings settings("fabulaGames", "KeySaver");

    int index = 1;
    foreach(Item *item, m_items) {
        settings.beginGroup(QString::number(index));
        settings.setValue("login", item->login);
        settings.setValue("password", item->password);
        settings.setValue("description", item->description);
        settings.endGroup();
        index++;
    }
}

void MyCryptor::clearAllPairs()
{
    QSettings settings("fabulaGames", "KeySaver");

    settings.clear();
}

void MyCryptor::emitSignalForItem(Item *item)
{
    emit pairDecripted(item->login, item->password, item->description);
}

QString MyCryptor::cryptValue(QString value, QString thekey)
{
    // this can be done not in this function - so we can generally set a key and work with it
    setKey(thekey);

    if(keyLenght() == 0) {
        QMessageBox::warning(NULL, "Error", "Introduceti cuvintul cheie");
        return "";
    }

    QString text = value;

    // get the number of rows of crypt table
    m_rows = text.length() / keyLenght();

    // if there is some remaining add one more line to crypt table
    if(text.length() % keyLenght() !=0)
        m_rows++;

    // create the crypt table
    char *datas = new char[m_rows * keyLenght()];

    // fill the table with out information
    int index = 0;
    for(int i=0; i < m_rows; i++) {
        for(int j=0; j < keyLenght(); j++){
          if(index < text.length())
              datas[j + keyLenght() * i] = text.at(index++).toLatin1();
          else
              datas[j + keyLenght() * i] = ' ';
        }
    }


    // set the key value to tmp and sort its characters
    QString tmp = key();
    QList<int> indexes;
    for (int i=0; i < tmp.length(); i++)
        indexes.append(i);

    for(int i=0; i < tmp.length(); i++) {
        for(int j=0; j < tmp.length(); j++) {
            if(tmp[i] < tmp[j]) {
                char c = tmp.at(i).toLatin1();
                tmp[i] = tmp[j];
                tmp[j] = c;

                // sort indexes as well
                int z = indexes.at(i);//.toAscii();
                indexes[i] = indexes[j];
                indexes[j] = z;
            }
        }
    }

    if (m_debug)
        qDebug() << "#########################################";

    QString crypt;
    // for each char in key
    for(int i=0; i < tmp.length(); i++) {
        for(int j=0; j < m_rows; j++) {
            char charForGroup = tmp.at(i).toLatin1();
            char charOfData = datas[indexes.at(i) + j * keyLenght()];
            char result = charOfData;
            char prev = charOfData;

            if (m_useXOR)
                result = cyclicXOR(charOfData, charForGroup);

            if (m_debug)
                qDebug() << "------------------------------------------";

            if (m_debug)
                qDebug() << "XOR in = " << prev << " with " << charForGroup << "  out = " << result;

            prev = result;
            if (m_useMove)
                result = moveUp(result, numberOfBitsInChar(charForGroup));

            if (m_debug)
                qDebug() << "Moved << of " << prev << "  by " << numberOfBitsInChar(charForGroup) <<  " = " << result;

            crypt.append(result);
        }
    }

    if (m_debug)
        qDebug() << "Crypted : " << value << "  to : " << crypt << "    with key : " << key();

    return crypt;
}

QString MyCryptor::decryptValue(QString value, QString thekey)
{
    setKey(thekey);
    // this is crypted text
    QString text = value;//Memo2->Text;

    // get the number of rows of crypt table
    m_rows = text.length() / keyLenght();

    // if there is some remaining add one more line to crypt table
    if(text.length() % keyLenght() !=0)
        m_rows++;

    QString tmp = key();//Edit7->Text; // this is the key
    QList<int> indexes;
    for (int i=0; i < tmp.length(); i++)
        indexes.append(i);

    // sort the characters of the key
    for(int i=0; i < tmp.length(); i++) {
        for(int j=0; j < tmp.length(); j++) {
            if(tmp[i] < tmp[j])
            {
                char c = tmp.at(i).toLatin1();
                tmp[i] = tmp[j];
                tmp[j] = c;

                // sort indexes as well
                int z = indexes.at(i);//.toAscii();
                indexes[i] = indexes[j];
                indexes[j] = z;
            }
        }
    }

    // this will be decripted info
    QString crypt;
    crypt.fill(' ', keyLenght() * m_rows);
    int index=0;

    if (m_debug)
        qDebug() << "#########################################";

    for(int i=0; i < tmp.length(); i++) {
        for(int j=0; j< m_rows; j++) {
            if (index >= value.length())
                continue;

            char charForGroup = tmp.at(i).toLatin1();
            char charOfData = value.at(index).toLatin1();
            char result = charOfData;
            char prev = charOfData;

            if (m_debug)
                qDebug() << "------------------------------------------";


            if (m_useMove)
                result = moveDown(charOfData, numberOfBitsInChar(charForGroup));

            if (m_debug)
                qDebug() << "Moved >> of " << prev << "  by " << numberOfBitsInChar(charForGroup) << " = " << result;

            prev = result;
            if (m_useXOR)
                result = cyclicXOR(result, charForGroup);

            // here is test
            if (m_debug)
                qDebug() << "XOR in = " << prev << "  with " << charForGroup << "  out = " << result;


            crypt[indexes.at(i) + j * keyLenght()] = result;
            index++;
        }
    }

    if (m_debug)
        qDebug() << "Decrypted : " << value << "  to : " << crypt << "    with key : " << key();

    return crypt;
}

char MyCryptor::cyclicXOR(char data, char key)
{
    char result = data;
    result ^= key;
    return result;
}

char MyCryptor::moveUp(quint8 data, int key)
{
    if (key == 0)
        return data;
    else {
        quint8 bit7 = data & 128;
        data *= 2;
        data += bit7 / 128;
        return moveUp(data, key-1);

    }
}

char MyCryptor::moveDown(quint8 data, int key)
{
    if (key == 0)
        return data;
    else {
        quint8 bit0 = data & 1;
        data /= 2;
        data += bit0 * 128;
        return moveDown(data, key-1);
    }
}

int MyCryptor::numberOfBitsInChar(char data)
{
    int result = 0;
    for (int i = 0; i < 8; i++) {
        if ((int)data & (int)pow(2, i))
            result ++;
    }

    if (m_debug)
        qDebug() << data << "  has " << result << " bits";

    return result;
}

int MyCryptor::keyLenght() const
{
    return m_key.length();
}


