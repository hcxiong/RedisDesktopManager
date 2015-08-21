#pragma once

#include "basetestcase.h"

class TestViewModel : public BaseTestCase
{
    Q_OBJECT

private slots:
    // Tests for QML exposed functions
    void testAddKey();
//    void testRenameKey();
//    void testRemoveKey();
//    void testCloseTab();
//    void testSetCurrentTab();
//    void testGetValue();

//    //Tests for c++ only functions
    void testOpenTab();
//    void closeDbKeys();

    // Test model
    void testAbstractModelMethods();
};
