#include "datamodel.hpp"
#include <QDebug>

DataModel::DataModel(QObject* parent)
    : QAbstractTableModel(parent) {}

int DataModel::rowCount(const QModelIndex&) const {
    return m_data.size();
}

int DataModel::columnCount(const QModelIndex&) const {
    return m_headers.size();
}

QVariant DataModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_data.size() || index.column() >= m_headers.size())
        return {};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const QJsonObject obj = m_data[index.row()].toObject();
        return obj.value(m_headers[index.column()]).toVariant();
    }
    return {};
}

bool DataModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || index.row() >= m_data.size() || index.column() >= m_headers.size() || role != Qt::EditRole)
        return false;

    QJsonObject obj = m_data[index.row()].toObject();
    obj[m_headers[index.column()]] = QJsonValue::fromVariant(value);
    m_data[index.row()] = obj;

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags DataModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_headers.value(section);
    }
    return {};
}

bool DataModel::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isArray()) {
        qWarning() << "Invalid JSON format in file:" << filePath;
        return false;
    }

    QJsonArray newData = doc.array();

    beginResetModel();
    m_data = newData;
    if (!m_data.isEmpty()) {
        const QJsonObject obj = m_data.first().toObject();
        m_headers = obj.keys();
    }
    endResetModel();

    return true;
}

void DataModel::saveToFile() const {
    if (m_filePath.isEmpty())
        return;
    if (m_updatesLocked) {
        qWarning() << "Failed to save file:" << m_filePath;
        return;
    }
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to save file:" << m_filePath;
        return;
    }

    QJsonDocument doc(m_data);
    file.write(doc.toJson());
    file.close();
}

void DataModel::setFilePath(const QString& filePath) {
    m_filePath = filePath;
    loadData();
}

void DataModel::addRow() {
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());

    QJsonObject newRow;
    for (const QString& header : m_headers) {
        newRow[header] = ""; // Заполняем пустыми значениями
    }
    m_data.append(newRow);

    endInsertRows();
}

void DataModel::lockUpdates(bool lock) {
    m_updatesLocked = lock;
}

void DataModel::loadData() {
    if (!m_filePath.isEmpty()) {
        loadFromFile(m_filePath);
    }
}