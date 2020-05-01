#include "treeitem.h"
#include "treemodel.h"

TreeModel::TreeModel(const QStringList &headers, QJsonDocument &data, QObject *parent)
    : QAbstractItemModel(parent) {

    QVector <QVariant> rootData;
    rootData << "Отчет" << "выполнение" << "срок"; //В модели будет 3 столбца

    rootItem = new TreeItem(rootData);
     //Создали корневой элемент

    setupModelData(data, rootItem);
      //Данные о строках модели разделены переводом строки
}



TreeModel::~TreeModel() { delete rootItem; }



int TreeModel::columnCount(const QModelIndex &parent) const {
 return rootItem->columnCount();

}



QVariant TreeModel::data (const QModelIndex &index, int role) const {
 if (!index.isValid()) return QVariant();
 if (role != Qt::DisplayRole  && role != Qt::EditRole) return QVariant();
 TreeItem *item = getItem(index);
 return item->data(index.column());
}



TreeItem *TreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}



Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
 if (!index.isValid()) return 0;
 return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}



QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
 if (orientation == Qt::Horizontal && role == Qt::DisplayRole) return rootItem->data(section);
 return QVariant();
}



QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
 if (parent.isValid() && parent.column() != 0) return QModelIndex();
 TreeItem *parentItem = getItem(parent);
 TreeItem *childItem = parentItem->child(row);
 if (childItem) return createIndex(row, column, childItem);
 else return QModelIndex();
}



QModelIndex TreeModel::parent(const QModelIndex &index) const {
 if (!index.isValid()) return QModelIndex();
 TreeItem *childItem = getItem(index);
 TreeItem *parentItem = childItem->parentItem();
 if (parentItem == rootItem) return QModelIndex();
 return createIndex(parentItem->childNumber(), 0, parentItem);
}



int TreeModel::rowCount(const QModelIndex &parent) const {
 TreeItem *parentItem = getItem(parent);
 return parentItem->childCount();
}



void TreeModel::setupModelData(const QJsonDocument &lines, TreeItem *parent) {
   // file1.setFileName(QFileDialog::getOpenFileName(nullptr, "", "E:\\qt_creator\\projects\\123\\123", ".json"));

    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;
    int number = 0;

    docAr = QJsonValue(lines.object().value("groups")).toArray();
  //  qDebug() << docAr.count();
    for(int i = 0; i < docAr.count(); i++){                                                                    //группы
        QVariant group_names =  docAr.at(i).toObject().value("group_name").toString();

        QVector <QVariant> columnData_gr;
        QString s = group_names.toString();

        group_names = s;
        columnData_gr << group_names;

        parents.last()->appendChild(new TreeItem(columnData_gr, parents.last()));


        QJsonArray cur_employee = docAr.at(i).toObject().value("employee").toArray();
        if (parents.last()->childCount() > 0) {
             parents << parents.last()->child(parents.last()->childCount()-1);
        }
        for(int j = 0; j < cur_employee.count(); j++){                                                       //работники

            QVector<QVariant> emp;

            emp << cur_employee.at(j).toObject().value("name").toString() << "" << "";

            parents.last()->appendChild(new TreeItem(emp, parents.last()));
         //   qDebug() << cur_employee.at(j).toObject().value("name").toString();


            QJsonArray task = cur_employee.at(j).toObject().value("tasks").toArray();

            if (parents.last()->childCount() > 0) {
                 parents << parents.last()->child(parents.last()->childCount()-1);
            }
            for(int k = 0; k < task.count(); k++){                                                      // задания

                QVector<QVariant> tsk;

                QVariant nm = task.at(k).toObject().value("task_name").toString();
                QVariant ready = task.at(k).toObject().value("readiness").toString();
                QVariant deadl = task.at(k).toObject().value("deadline").toString();

                tsk << nm << ready << deadl;

                parents.last()->appendChild(new TreeItem(tsk, parents.last()));

            }

            parents.pop_back();

        }
        parents.pop_back();


    }

}

QByteArray TreeModel::createFile(QString path_name) const{
    /*открыть файл */

    TreeItem *root = rootItem;

    //пробежим по всем группам
    QJsonArray group_array;
    for(int i = 0; i < root->childCount(); i++){
        TreeItem *cur_group = root->child(i); //возьмем i-ю группу
        QString group_name = cur_group->data(0).toString();


        //пробежим по все работникам
        QJsonArray employee_array;
        for(int j = 0; j < cur_group->childCount(); j++){
            TreeItem *cur_employee = cur_group->child(j);
            QString employee_name = cur_employee->data(0).toString();

            //пробежим по всем таскам
            QJsonArray task_array;
            for(int k = 0; k < cur_employee->childCount(); k++){
                TreeItem *cur_task = cur_employee->child(k);

                QString task_name_get = cur_task->data(0).toString();
                QString task_readiness_get = cur_task->data(1).toString();
                QString task_deadline_get = cur_task->data(2).toString();

                QVariantMap map;

                map.insert("task_name", task_name_get);
                map.insert("readiness", task_readiness_get);
                map.insert("deadline", task_deadline_get);

                QJsonObject json_task = QJsonObject::fromVariantMap(map);

                task_array.append(json_task);

            }
            QVariantMap employee_map;

            employee_map.insert("name", employee_name);
            employee_map.insert("tasks", task_array);
            QJsonObject json_employee = QJsonObject::fromVariantMap(employee_map);
            employee_array.append(json_employee);

        }
        QVariantMap group_map;
        group_map.insert("group_name", group_name);
        group_map.insert("employee", employee_array);
        QJsonObject json_group = QJsonObject::fromVariantMap(group_map);
        group_array.append(json_group);
    }
    QFile file_to_save;

    file_to_save.setFileName(path_name);
    file_to_save.open(QIODevice::WriteOnly|QFile::Text);
    QJsonDocument doc_to_write;
    doc_to_write.setArray(group_array);
    file_to_save.write("{\n\t\"groups\":"+doc_to_write.toJson()+ "}");

    file_to_save.close();

    return ("{\n\t\"groups\":"+doc_to_write.toJson()+ "}");
}



bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
 if (role != Qt::EditRole) return false;
 TreeItem *item = getItem(index);
 bool result = item->setData(index.column(), value);
 if (result) {
  emit dataChanged(index, index);
 }
 return result;
}



bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role) {
 if (role != Qt::EditRole || orientation != Qt::Horizontal) return false;
 bool result = rootItem->setData(section, value);
 if (result) {
  emit headerDataChanged(orientation, section, section);
 }
 return result;
}



bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent) {
 bool success;
 beginInsertColumns(parent, position, position + columns - 1);
 success = rootItem->insertColumns(position, columns);
 endInsertColumns();
 return success;
}



bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent) {
 TreeItem *parentItem = getItem(parent);
 bool success;
 beginInsertRows(parent, position, position + rows - 1);
 success = parentItem->insertChildren(position, rows, rootItem->columnCount());
 endInsertRows();
 return success;
}



bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent) {
 bool success;
 beginRemoveColumns(parent, position, position + columns - 1);
 success = rootItem->removeColumns(position, columns);
 endRemoveColumns();
 if (rootItem->columnCount() == 0) removeRows(0, rowCount());
 return success;
}



bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent) {
 TreeItem *parentItem = getItem(parent);
 bool success = true;
 beginRemoveRows(parent, position, position + rows - 1);
 success = parentItem->removeChildren(position, rows);
 endRemoveRows();
 return success;
}


