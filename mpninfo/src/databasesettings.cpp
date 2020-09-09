#include "databasesettings.h"

#include <QSqlQuery>

void DatabaseSettings::setValue(const QString &key, const QVariant &value)
{
    QVariant currentValue = this->value(key);
    if (currentValue == value)
        return;

    QString result;
    if (value.type() == QVariant::ByteArray) {
        QByteArray a = value.toByteArray();
        result = QLatin1String("@ByteArray(");
        result += QString::fromLatin1(a.constData(), a.size());
        result += QLatin1Char(')');
    }
    else if (value.type() == QVariant::Double)
        result = QString("%1").arg(value.toDouble(), 0, 'f', 2);
    else
        result = value.toString();

    QString sql;
    if (currentValue.isNull())
        sql = QString("INSERT INTO `settings` values('%1', '%2')").arg(key, result);
    else
        sql = QString("UPDATE `settings` SET `value`='%2' WHERE `key`='%1'").arg(key, result);

    mEngine->database()->exec(sql);
}

QVariant DatabaseSettings::value(const QString &key)
{
    DatabasePtr db = mEngine->database();
    db->exec(QString("SELECT `value` FROM `settings` WHERE `key`='%1'").arg(key));
    if (!db->next())
        return QVariant();

    QString s = db->value(0).toString();
    if (s.startsWith(QLatin1String("@ByteArray(")))
        return QByteArray(s.toLatin1().mid(11, s.size() - 12));
    else
        return db->value(0);
}

QVariant DatabaseSettings::remove(const QString &key)
{
    DatabasePtr db = mEngine->database();
    db->exec(QString("DELETE FROM `settings` WHERE `key`='%1'").arg(key));
    if (!db->next())
        return QVariant();

    QString s = db->value(0).toString();
    if (s.startsWith(QLatin1String("@ByteArray(")))
        return QByteArray(s.toLatin1().mid(11, s.size() - 12));
    else
        return db->value(0);
}
