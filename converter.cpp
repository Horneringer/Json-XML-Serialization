#include "converter.h"

Converter::Converter(QObject *parent) : QObject(parent)
{

}

void Converter::writeJson(Test *obj, QString path)
{
    QVariantMap map;
    QVariantMap items;

    //устанавливаем имя с помощью существующей функции у объекта класса Test
    map["name"] = obj->name();

    //заполнение QVariantMap парами и значениями с помощью функции map класса Test
    foreach(QString key, obj->map().keys()){

        items.insert(key, QVariant(obj->map().value(key)));
    }

    //ключ, значение для которого это словарь items
    map["items"] = QVariant(items);

    //создаём документ json; преобразование из QVariant в Json
    QJsonDocument document = QJsonDocument::fromVariant(map);

    //открываем файл на чтение
    QFile file(path);

    if(!file.open(QIODevice::WriteOnly)){

         qCritical() << "Can't write!";
         qCritical() << file.errorString();
         return;
    }

    //запись в файл документа с преобразованием utf-8 для QFile
    file.write(document.toJson());
    file.close();

}

Test* Converter::readJson(QString path)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){

         qCritical() << "Can't write!";
         qCritical() << file.errorString();

         //возврат нулевого указателя
         return nullptr;
    }

    //одним блоком считываем файл в массив байт
    QByteArray data = file.readAll();
    file.close();

    //преобраование массива байт в json документ
    QJsonDocument document = QJsonDocument::fromJson(data);

    //динамически создаём объект класса
    Test* t = new Test();

    //устанавливаем значение имени для ключа name
    //по сути установка имени объекта
    t->setName(document["name"].toString());

    //закидываем по ключу items нужное знаение, которое является словарём
    //преобразуем в QVariantMap
    QVariantMap vmap = qvariant_cast<QVariantMap>(document["items"]);

    QMap<QString,QString> map;

    //формирование словаря ключей и значений из QVariantMap
    foreach(QString key, vmap.keys()){

        map.insert(key, vmap[key].toString());

    }

    //создание словаря для объекта
    t->setMap(map);

    //возврат объекта
    return t;

}

//запись xml файла
void Converter::writeXML(Test *obj, QString path)
{
    QFile file(path);

    if(!file.open(QIODevice::WriteOnly)){

         qCritical() << "Can't write!";
         qCritical() << file.errorString();

         //возврат нулевого указателя
         return;
    }

    //создание объекта и передача файла в конструктор
    QXmlStreamWriter stream(&file);

    //запись начала докумена -  версии xml
    stream.writeStartDocument();

    //установка автоформатирования
    stream.setAutoFormatting(true);

    //запись начального элемента с именем test
    stream.writeStartElement("test");

    //добавление аттрибута к начальному элементу, записывается в поле name объекта
    stream.writeAttribute("name", obj->name());

    //запись ещё одного элемента внутри test
    stream.writeStartElement("items");

    //цикл для создания внутри новых элементов
    foreach(QString key, obj->map().keys()){

        stream.writeStartElement("item");

        //первым аттрибутом будет ключ map
        stream.writeAttribute("key", key);

        //вторым - соответствующее значение
        stream.writeAttribute("value", obj->map().value(key));

        //конец элемента item
        stream.writeEndElement();

    }

    //конец элемента items
    stream.writeEndDocument();

    //конец элемента test
    stream.writeEndDocument();

    file.close();

}

//чтение xml файла
Test *Converter::readXML(QString path)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){

         qCritical() << "Can't read!";
         qCritical() << file.errorString();

         //возврат нулевого указателя
         return nullptr;
    }

    //чтение файла в массив
    QByteArray data = file.readAll();

    file.close();

    //создание объекта и передача массива в конструктор
    QXmlStreamReader stream(data);

    //динамическое создание объекта
    Test* t = new Test();

    //map для заполнения
    QMap<QString,QString> map;

    //чтение данных из потока
    while(!stream.atEnd()){

        //указывает тип токена, который только что был прочитан
        QXmlStreamReader::TokenType token = stream.readNext();

        //конструкция switch-case для парсинга файла
        switch (token) {

        //типы токенов, которые игнорируются при чтении
               case QXmlStreamReader::Comment:
                   break;
               case QXmlStreamReader::DTD:
                   break;
               case QXmlStreamReader::Characters:
                   break;
               case QXmlStreamReader::ProcessingInstruction:
                   break;
               case QXmlStreamReader::EntityReference:
                   break;
               case QXmlStreamReader::NoToken:
                   break;
               case QXmlStreamReader::Invalid:
                   break;
               case QXmlStreamReader::StartDocument:
                   break;

            //если достигли конца документа, передаём в объект то, что хранится в map
               case QXmlStreamReader::EndDocument:
                   t->setMap(map);
                   break;

                   //если найден начальный элемент test
               case QXmlStreamReader::StartElement:
                   if(stream.name().toString() == "test")
                   {


                       //чтение имени аттрибута и передача объекту в свойство name
                       QString name = stream.attributes().value("name").toString();
                       t->setName(name);
                   }

                   //если найден начальный элемент item
                   if(stream.name().toString() == "item")
                   {
                       //чтение ключа и значения из аттрибутов
                       QString key = stream.attributes().value("key").toString();
                       QString value = stream.attributes().value("value").toString();

                       //вставка в map
                       map.insert(key,value);
                   }

                   break;

                   //конец элемента игнорируется
               case QXmlStreamReader::EndElement:
                   break;
               }

    }

    return t;


}
