/*
 * Copyright 2013 Canonical Ltd.
 *
 * This file is part of webbrowser-app.
 *
 * webbrowser-app is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * webbrowser-app is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HISTORY_MODEL_H__
#define __HISTORY_MODEL_H__

// Qt
#include <QtCore/QAbstractListModel>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtSql/QSqlDatabase>

class HistoryModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString databasePath READ databasePath WRITE setDatabasePath NOTIFY databasePathChanged)

    Q_ENUMS(Roles)

public:
    HistoryModel(QObject* parent=0);
    ~HistoryModel();

    enum Roles {
        Url = Qt::UserRole + 1,
        Title,
        Icon,
        Visits,
        LastVisit
    };

    // reimplemented from QAbstractListModel
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& parent=QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;

    const QString databasePath() const;
    void setDatabasePath(const QString& path);

    Q_INVOKABLE int add(const QUrl& url, const QString& title, const QUrl& icon);
    Q_INVOKABLE void clearAll();

Q_SIGNALS:
    void databasePathChanged() const;

private:
    QSqlDatabase m_database;

    struct HistoryEntry {
        QUrl url;
        QString title;
        QUrl icon;
        uint visits;
        QDateTime lastVisit;
    };
    QList<HistoryEntry> m_entries;
    int getEntryIndex(const QUrl& url) const;

    void resetDatabase(const QString& databaseName);
    void createDatabaseSchema();
    void populateFromDatabase();
    void insertNewEntryInDatabase(const HistoryEntry& entry);
    void updateExistingEntryInDatabase(const HistoryEntry& entry);
    void clearDatabase();
};

#endif // __HISTORY_MODEL_H__