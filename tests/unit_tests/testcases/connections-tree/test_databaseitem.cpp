#include "test_databaseitem.h"
#include "connections-tree/items/databaseitem.h"
#include "connections-tree/items/serveritem.h"
#include "connections-tree/model.h"
#include "mocks/itemoperationsmock.h"

#include <QtCore>
#include <QTest>
#include <QSignalSpy>
#include <QMenu>

using namespace ConnectionsTree;

TestDatabaseItem::TestDatabaseItem(QObject *parent) :
    QObject(parent)
{
}

void TestDatabaseItem::testLoadKeys()
{
    //given
    ItemOperationsMock* operations = new ItemOperationsMock();
    QSharedPointer<Operations> operations_(dynamic_cast<Operations*>(operations));
    operations->databases.append({0, 55});

    for (int i=1; i < 1000000; i++) {
        operations->keys.append(QString("test-%1-key").arg(i));
    }
    operations->keys.append("test-2-key");
    operations->keys.append("test-2-key:subkey");
    operations->keys.append("test-2-key:namespace:subkey2");
    Model dummyModel;
    QSharedPointer<ServerItem> parentItem(new ServerItem("test", operations_, dummyModel));
    parentItem->setWeakPointer(parentItem.toWeakRef());

    DummyParentView view;

    //when
    parentItem->onClick(view);
    qDebug() << parentItem->childCount();
    QSharedPointer<TreeItem> item = parentItem->child(0);

    QSignalSpy spy((DatabaseItem*)item.data(), SIGNAL(keysLoaded(unsigned int)));
    bool actualResult = item->onClick(view);

    //then
    QCOMPARE(spy.wait(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(item->childCount(), (unsigned int)1000001);
    QCOMPARE(actualResult, true);
    QCOMPARE(item->getDisplayName(), QString("db0 (1000002/55)"));
    QCOMPARE(item->getIcon().isNull(), false);
    QCOMPARE(item->getAllChilds().isEmpty(), false);
    QCOMPARE(item->isEnabled(), true);
    QCOMPARE(item->isLocked(), false);
}

void TestDatabaseItem::testUnloadKeys()
{
    //given
    ItemOperationsMock* operations = new ItemOperationsMock();
    DatabaseItem item(0, 300, QSharedPointer<Operations>(dynamic_cast<Operations*>(operations)),
                      QWeakPointer<ConnectionsTree::TreeItem>());

    //when
    item.unload();

    //then
    QCOMPARE(item.childCount(), (unsigned int)0);
    QCOMPARE(item.isLocked(), false);
}

void TestDatabaseItem::testContextMenu()
{
    //given
    ItemOperationsMock* operations = new ItemOperationsMock();
    DatabaseItem item(0, 300, QSharedPointer<Operations>(dynamic_cast<Operations*>(operations)),
                      QWeakPointer<ConnectionsTree::TreeItem>());
    DummyParentView view;

    //when
    QSharedPointer<QMenu> actualResult = item.getContextMenu(view);

    //then
    QCOMPARE(actualResult->isEmpty(), false);
}
