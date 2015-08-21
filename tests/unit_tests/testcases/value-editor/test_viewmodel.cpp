#include "test_viewmodel.h"
#include <QSignalSpy>
#include <modeltest.h>
#include "modules/value-editor/viewmodel.h"
#include "modules/connections-tree/items/keyitem.h"
#include "mocks/fakeKeyFactory.h"
#include "testcases/connections-tree/mocks/itemoperationsmock.h"

void TestViewModel::testAddKey()
{
    // Given
    using namespace ValueEditor;

    QSharedPointer<FakeKeyFactory> keyFactory(new FakeKeyFactory());
    ViewModel model(keyFactory.dynamicCast<AbstractKeyFactory>());
    QString keyName("fake");
    QString keyType("string");
    QVariantMap row{{"value","fake"}};
    auto connection = getFakeConnection();

    // When
    // Case 1: Empty NewKeyRequest - addKey() on keyfactory is not called
    model.addKey(keyName, keyType, row);
    QCOMPARE((uint)0, keyFactory->addKeyCalled);

    // Case 2: New Request with valid connection - addKey() called
    QSignalSpy spy(&model, SIGNAL(newKeyDialog(QString, QString)));
    model.openNewKeyDialog(connection.dynamicCast<RedisClient::Connection>(),
                           0, "fake_key_prefix");
    QCOMPARE(spy.count(), 1);
    model.addKey(keyName, keyType, row);
    QCOMPARE((uint)1, keyFactory->addKeyCalled);

    // Case 3: New Request with invalid connection - addKey() not called
    model.openNewKeyDialog(connection.dynamicCast<RedisClient::Connection>(),
                           0, "fake_key_prefix");
    connection.clear();
    model.addKey(keyName, keyType, row);
    QCOMPARE((uint)1, keyFactory->addKeyCalled);
}

void TestViewModel::testOpenTab()
{
    // Given
    auto connection = getFakeConnection();
    // ViewModel
    using namespace ValueEditor;
    QSharedPointer<FakeKeyFactory> keyFactory(new FakeKeyFactory());
    ViewModel model(keyFactory.dynamicCast<AbstractKeyFactory>());
    // KeyItem
    ItemOperationsMock* operations = new ItemOperationsMock();
    QSharedPointer<ConnectionsTree::Operations> operations_(
                dynamic_cast<ConnectionsTree::Operations*>(operations));
    QSharedPointer<ConnectionsTree::KeyItem> key(
                "fake:full:path", 0, perations_,
                QWeakPointer<ConnectionsTree::TreeItem>()
    );

    // When
    model.openTab(connection.dynamicCast<RedisClient::Connection>(),
                  key, false);

    // Then
    // TBD
}


void TestViewModel::testAbstractModelMethods()
{
    // Given
    using namespace ValueEditor;

    QSharedPointer<AbstractKeyFactory> keyFactory(new FakeKeyFactory());
    ViewModel model(keyFactory);

    // When
    ModelTest test(&model);

    // Then
    // No assertions
}
