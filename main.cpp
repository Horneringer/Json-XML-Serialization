#include <QCoreApplication>
#include"test.h"
#include"converter.h"

bool saveFile(QString path){

    QFile file(path);

    if(!file.open(QIODevice::WriteOnly)){

        return false;
    }

    //захватываем файл потоком
    QDataStream out(&file);

    //установка версии Qt для сериализации файла
    out.setVersion(QDataStream::Qt_6_2);

    QString title = "Answer is 42!";

    qint64 num = 42;

    //закидываем в поток
    out << title;
    out << num;

    file.close();

    return true;
}

bool readFile(QString path){

    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){

        return false;
    }

    //захватываем файл потоком
    QDataStream in(&file);

    //проверка версии для корректной десериализации
    if(in.version() != QDataStream::Qt_6_2){

        qCritical() << "Wrong version!";
        file.close();
        return false;
    }

    //извлекаем данные из потока в переменные, в том же порядке, в каком записывали

    QString title;
    qint64 num;

    in >> title;
    in >> num;

    qInfo() << title;
    qInfo() << num;

    file.close();

    return true;
}


//функция сохранения для класса; кроме пути к файлу, в параметрах ещё указатель на объект класса
bool saveFile2(Test* t, QString path){

    QFile file(path);

    if(!file.open(QIODevice::WriteOnly)){

        return false;
    }

    QDataStream ds(&file);

    ds.setVersion(QDataStream::Qt_6_2);

    //захват потоком объекта
    ds << *t;

    file.close();

    return true;
}

//функция загрузки для класса
bool loadFile(QString path){

    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){

        return false;
    }

     QDataStream ds(&file);

     if(ds.version() != QDataStream::Qt_6_2){

         qCritical() << "Wrong version!";
         file.close();
         return false;
     }

     //создание объекта, для дальнейшей загрузки данных в него из потока
     Test t;

     //срабатывает перегрузка оператора и заполняются необходимые поля
     ds >> t;

     file.close();

     //отображение имени
     qInfo() << "Name " << t.name();

     //отображение значения для каждого ключа
     foreach(QString key, t.map().keys()){
         qInfo() << key << " : " << t.map().value(key);
     }

     return true;

}

void writeJson(QString path){

    //объект
    Test t;

    //заполение данными
    t.fill();

    //вызов метода записи из объекта в указанный файл
    Converter::writeJson(&t, path);


}

void readJson(QString path){

    Test* t;

    //чтение из json файла в объект
    t = Converter::readJson(path);

    //если объект не загрузился, выводим сообщение
    if(!t){

        qInfo() << "Obj not loaded";
        return;
    }

    //вывод имени объекта
    qInfo() << "Name " << t->name();

    //выводим словарь объекта
    foreach(QString key, t->map()){

        qInfo() << key << " : " << t->map().value(key);
    }

}

void writeXml(QString path){

    //объект
    Test t;

    //заполение данными
    t.fill();

    //вызов метода записи из объекта в указанный файл
    Converter::writeXML(&t, path);


}

void readXml(QString path){

    Test* t;

    //чтение из json файла в объект
    t = Converter::readXML(path);

    //если объект не загрузился, выводим сообщение
    if(!t){

        qInfo() << "Obj not loaded";
        return;
    }

    //вывод имени объекта
    qInfo() << "Name " << t->name();

    //выводим словарь объекта
    foreach(QString key, t->map().keys()){

        qInfo() << key << " : " << t->map().value(key);
    }

}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString file = "test.txt";

    if(saveFile(file)){
        qInfo() << "Saved!";
        readFile(file);
    }

    //=========================================

    QString file2 = "test2.txt";

    Test t;

    //заполение случайными данными
    t.fill();

    if(saveFile2(&t,file2)){
        loadFile(file2);

    }
//===============================================

    QString file3 = "test3.txt";

    // первым в файл записывается имя, всё что записывается далее, будет находится выше
    //writeJson(file3);
    //readJson(file3);

    //=====================================
    QString file4 = "test4.txt";

    writeXml(file4);
    readXml(file4);



    return a.exec();
}
