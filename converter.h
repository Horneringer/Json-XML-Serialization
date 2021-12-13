#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include<QDebug>
#include<QFile>
#include<QDir>

//для чтения XML
#include<QXmlStreamReader>

//для записи XML
#include<QXmlStreamWriter>

//действует как объединение для наиболее распространенного типа данных Qt
#include<QVariant>

//map пар для QVariant
#include<QVariantMap>

//для работы с объектами json
#include<QJsonObject>

//массив объектов json
#include<QJsonArray>

//обеспечивает способ чтения и записи документов JSON
#include<QJsonDocument>

#include<QMap>

#include"test.h"

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = nullptr);

    //функция записи объекта в json файл
    static void writeJson(Test* obj, QString path);

    //функция чтения из json файла возвращает объект
    //в параметрах путь к файлу
    static Test* readJson(QString path);


    //функция записи объекта в xml файл
    static void writeXML(Test* obj, QString path);

    //функция чтения из xml файла возвращает объект
    //в параметрах путь к файлу
    static Test* readXML(QString path);

signals:

};

#endif // CONVERTER_H
