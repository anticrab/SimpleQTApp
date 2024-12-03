#pragma once

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class DataModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit DataModel(QObject* parent = nullptr);

    // Overridden methods from QAbstractTableModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool loadFromFile(const QString& filePath);
    void saveToFile() const;
    void setFilePath(const QString& filePath);

    // Methods for editing structure
    void addRow();

    // Locking updates
    void lockUpdates(bool lock);

    signals:
        void fileChanged();

private:
    QJsonArray m_data;
    QStringList m_headers;
    QString m_filePath;
    bool m_updatesLocked = false;
    bool m_updatesLockedFile = false;

    void loadData();
};
