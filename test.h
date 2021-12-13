#ifndef TEST_H
#define TEST_H

#include <QObject>
#include<QDebug>
#include<QDataStream>
#include<QFile>
#include<QMap>


//класс для зашрузки и сохранения данных
class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);

    void fill();
    QString name();
    void setName(QString value);
    void setMap(QMap<QString, QString> newMap);
    QMap<QString,QString> map();

    //перегрузка оператора вывода будет дружественной функцией для класса Test
    //дружественная функция имеет доступ к приватным членам класса
    //в параметрах поток и объет, который передается
    friend QDataStream& operator <<(QDataStream &stream, const Test &t){

        qInfo() << "Operator <<";

        //порядок передачи данных в поток важен!

        //передача имени
        stream << t.m_name;

        //передача map
        stream << t.m_map;

        return  stream;
    }

    //аналогично с оператором чтения из потока
    //ссылка на объект уже не константная, так как происходит изменение параметров объекта
    friend QDataStream& operator >>(QDataStream &stream, Test &t){

        qInfo() << "Operator >>";

        //порядок передачи данных в поток важен!

        //передача имени
        stream >> t.m_name;

        //передача map
        stream >> t.m_map;

        return  stream;
    }




signals:

private:

    QString m_name;
    QMap<QString,QString> m_map;
};

#endif // TEST_H
