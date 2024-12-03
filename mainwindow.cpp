#include "mainwindow.hpp"
#include <QInputDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_tableView(new QTableView(this)), m_model(new DataModel(this)), m_watcher(new QFileSystemWatcher(this)) {
    setupUi();

    const QString jsonFilePath = "data.json";
    m_model->setFilePath(jsonFilePath);
    setupFileWatcher(jsonFilePath);

    m_tableView->setModel(m_model);

    connect(m_addRowButton, &QPushButton::clicked, m_model, &DataModel::addRow);
    connect(m_saveButton, &QPushButton::clicked, m_model, &DataModel::saveToFile);
}

void MainWindow::setupFileWatcher(const QString& filePath) {
    if (!m_watcher->addPath(filePath)) {
        qWarning() << "Failed to watch the file:" << filePath;
        return;
    }

    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this, filePath]() {
        qDebug() << "File changed. Reloading data.";
        m_model->lockUpdates(true);  // Блокируем пользовательские изменения.
        m_model->loadFromFile(filePath);
        m_model->lockUpdates(false); // Разблокируем после обновления.
        m_watcher->addPath(filePath); // Повторно отслеживаем файл.
    });
}

void MainWindow::setupUi() {
    auto* centralWidget = new QWidget(this);
    auto* layout = new QVBoxLayout(centralWidget);

    m_addRowButton = new QPushButton("Add Row", this);
    m_saveButton = new QPushButton("Save Changes", this);

    layout->addWidget(m_tableView);
    layout->addWidget(m_addRowButton);
    layout->addWidget(m_saveButton);

    setCentralWidget(centralWidget);
}