#include "test.h"

Test::Test(QObject *parent) : QObject(parent)
{

}

void Test::fill()
{
    m_name = "test obj";
    for(int i=0; i<10;i++){

        //номер итеорации в строку
        QString num = QString::number(i);

        //делаем его ключом
        QString key = "Key " + num;

        //и значением
        QString value = "Item number " + num;

        //формируем пару в map
        m_map.insert(key,value);
    }

}

QString Test::name()
{
    return m_name;
}

void Test::setName(QString value)
{
    m_name = value;

}

QMap<QString, QString> Test::map()
{

    return m_map;

}

//заполнение map значениями
void Test::setMap(QMap<QString, QString> newMap)
{
    m_map = newMap;
}
