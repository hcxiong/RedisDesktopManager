#include <QTest>
#include "modules/value-editor/abstractkeyfactory.h"

class FakeKeyFactory : public ValueEditor::AbstractKeyFactory
{
public:
    void loadKey(QSharedPointer<RedisClient::Connection> connection,
                 QString keyFullPath, int dbIndex,
                 std::function<void(QSharedPointer<ValueEditor::Model>, const QString&)> callback) override
    {
        loadKeyCalled++;
        QVERIFY(connection.isNull() == false);
        QVERIFY(keyFullPath.isEmpty() == false);
        QVERIFY(dbIndex >= 0);
    }

    void addKey(QSharedPointer<RedisClient::Connection> connection,
                 QString keyFullPath, int dbIndex, QString type,
                 const QVariantMap &row) override
    {
        addKeyCalled++;
        QVERIFY(connection.isNull() == false);
        QVERIFY(keyFullPath.isEmpty() == false);
        QVERIFY(dbIndex >= 0);
        QVERIFY(type.isEmpty() == false);
        QVERIFY(row.isEmpty() == false);
    }

    uint loadKeyCalled;
    uint addKeyCalled;
};
