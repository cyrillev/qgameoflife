#include <QString>
#include <QtTest>

#include "generic/model/golquadtree.h"

using namespace gol;

class QuadTreeUnitTestTest : public QObject
{
    Q_OBJECT

public:
    QuadTreeUnitTestTest();

    void testCase_check_empty_quadtree();
    void testCase_set_value();
    void testCase_unset_value();
    void testCase_one_cell_dies_in_next_generation();
    void testCase_repeater_inside_grid();
    void testCase_repeater();
    void testCase_repeater_in_between_2_grids();
private Q_SLOTS:
    void testCase_glider();

};

QuadTreeUnitTestTest::QuadTreeUnitTestTest()
{
}

void QuadTreeUnitTestTest::testCase_check_empty_quadtree()
{
    golquadtree quadtree(10000, 20000);
    QVERIFY2(quadtree.width() == 10000, "width is 10000");
    QVERIFY2(quadtree.height() == 20000, "height is 20000");
    QVERIFY2(quadtree.get(10 ,10) == false, "There is no data in an empty tree");
}

void QuadTreeUnitTestTest::testCase_set_value()
{
    golquadtree quadtree(10000, 10000);
    quadtree.set(10 ,10);
    QVERIFY2(quadtree.get(10 ,10) == true, "cell (10,10) is set");
}

void QuadTreeUnitTestTest::testCase_unset_value()
{
    golquadtree quadtree(10000, 10000);
    quadtree.set(10 ,10);
    QVERIFY2(quadtree.get(10 ,10) == true, "cell (10,10) is set");
    quadtree.dump();
    quadtree.unset(10 ,10);
    QVERIFY2(quadtree.get(10 ,10) == false, "cell (10,10) is not set");
}

void QuadTreeUnitTestTest::testCase_one_cell_dies_in_next_generation()
{
    golquadtree quadtree(10000, 10000);
    quadtree.set(10 ,10);

    boost::unordered_set<point_t> new_cells;
    boost::unordered_set<point_t> dead_cells;
    quadtree.nextGeneration(new_cells, dead_cells);

    QVERIFY2( dead_cells.find( point_t(10,10) ) != dead_cells.end(), "cell (10,10) is dead" );
    QVERIFY2( dead_cells.size() == 1,    "There are only 1 dead cell");
    QVERIFY2(quadtree.get(10 ,10) == false, "cell (10,10) is not set");
}


void QuadTreeUnitTestTest::testCase_repeater_inside_grid()
{
    golquadtree quadtree(10000, 10000);
    quadtree.set(10 ,10);
    quadtree.set(11 ,10);
    quadtree.set(12 ,10);

    boost::unordered_set<point_t> new_cells;
    boost::unordered_set<point_t> dead_cells;
    quadtree.nextGeneration(new_cells, dead_cells);

    //             #
    // ###    =>   #
    //             #
    QVERIFY2( dead_cells.find( point_t(10,10) ) != dead_cells.end(), "cell (10,10) is dead" );
    QVERIFY2( dead_cells.find( point_t(12,10) ) != dead_cells.end(), "cell (12,10) is dead" );
    QVERIFY2( dead_cells.size() == 2,    "There are only 2 dead cells");

    QVERIFY2( new_cells.find( point_t( 9,11) ) != dead_cells.end(), "cell ( 9,11) is born" );
    QVERIFY2( new_cells.find( point_t(11,11) ) != dead_cells.end(), "cell (11,11) is born" );
    QVERIFY2( new_cells.size() == 2,    "There are only 2 new cells");

    QVERIFY2( quadtree.get(10,  9) == false, "cell (10, 9) is not set");
    QVERIFY2( quadtree.get(10, 10) == false, "cell (10,10) is not set");
    QVERIFY2( quadtree.get(10, 11) == false, "cell (10,11) is not set");
    QVERIFY2( quadtree.get(11,  9) == true,  "cell (11, 9) is set");
    QVERIFY2( quadtree.get(11, 10) == true,  "cell (11,10) is set");
    QVERIFY2( quadtree.get(11, 11) == true,  "cell (11,11) is set");
    QVERIFY2( quadtree.get(12,  9) == false, "cell (12, 9) is not set");
    QVERIFY2( quadtree.get(12, 10) == false, "cell (12,10) is not set");
    QVERIFY2( quadtree.get(12, 11) == false, "cell (12,11) is not set");
}

void QuadTreeUnitTestTest::testCase_repeater()
{
    golquadtree quadtree(10000, 10000);
    quadtree.set(0 ,0);
    quadtree.set(1 ,0);
    quadtree.set(2 ,0);

    boost::unordered_set<point_t> new_cells;
    boost::unordered_set<point_t> dead_cells;
    quadtree.nextGeneration(new_cells, dead_cells);

    //             #
    // ###    =>   #
    //             #
    QVERIFY2( dead_cells.find( point_t(0,0) ) != dead_cells.end(), "cell (0,0) is dead" );
    QVERIFY2( dead_cells.find( point_t(2,0) ) != dead_cells.end(), "cell (2,0) is dead" );
    QVERIFY2( dead_cells.size() == 2,    "There are only 2 dead cells");

    QVERIFY2( new_cells.find( point_t(-1,1) ) != dead_cells.end(), "cell (-1,1) is born" );
    QVERIFY2( new_cells.find( point_t( 1,1) ) != dead_cells.end(), "cell (1,1) is born" );
    QVERIFY2( new_cells.size() == 2,    "There are only 2 new cells");

    QVERIFY2( quadtree.get(0, -1) == false, "cell (0,-1) is not set");
    QVERIFY2( quadtree.get(0,  0) == false, "cell (0, 0) is not set");
    QVERIFY2( quadtree.get(0,  1) == false, "cell (0, 1) is not set");
    QVERIFY2( quadtree.get(1, -1) == true,  "cell (1,-1) is set");
    QVERIFY2( quadtree.get(1,  0) == true,  "cell (1, 0) is set");
    QVERIFY2( quadtree.get(1,  1) == true,  "cell (1, 1) is set");
    QVERIFY2( quadtree.get(2, -1) == false, "cell (2,-1) is not set");
    QVERIFY2( quadtree.get(2 , 0) == false, "cell (2, 0) is not set");
    QVERIFY2( quadtree.get(2 , 1) == false, "cell (2, 1) is not set");

    quadtree.dump();

}


void QuadTreeUnitTestTest::testCase_repeater_in_between_2_grids()
{
    golquadtree quadtree(10000, 10000);
    quadtree.set(-1 ,0);
    quadtree.set( 0 ,0);
    quadtree.set( 1 ,0);
    quadtree.dump();

    boost::unordered_set<point_t> new_cells;
    boost::unordered_set<point_t> dead_cells;
    quadtree.nextGeneration(new_cells, dead_cells);

    //             #
    // ###    =>   #
    //             #
    QVERIFY2( dead_cells.find( point_t(-1,0) ) != dead_cells.end(), "cell (-1,0) is dead" );
    QVERIFY2( dead_cells.find( point_t( 1,0) ) != dead_cells.end(), "cell ( 1,0) is dead" );
    QVERIFY2( dead_cells.size() == 2,    "There are only 2 dead cells");

    QVERIFY2( new_cells.find( point_t(0, 1) ) != dead_cells.end(), "cell (0, 1) is born" );
    QVERIFY2( new_cells.find( point_t(0,-1) ) != dead_cells.end(), "cell (0,-1) is born" );
    QVERIFY2( new_cells.size() == 2,    "There are only 2 new cells");

    QVERIFY2( quadtree.get(-1, -1) == false, "cell (-1,-1) is not set");
    QVERIFY2( quadtree.get(-1,  0) == false, "cell (-1, 0) is not set");
    QVERIFY2( quadtree.get(-1,  1) == false, "cell (-1, 1) is not set");
    QVERIFY2( quadtree.get( 0, -1) == true,  "cell ( 0,-1) is set");
    QVERIFY2( quadtree.get( 0,  0) == true,  "cell ( 0, 0) is set");
    QVERIFY2( quadtree.get( 0,  1) == true,  "cell ( 0, 1) is set");
    QVERIFY2( quadtree.get( 1, -1) == false, "cell ( 1,-1) is not set");
    QVERIFY2( quadtree.get( 1 , 0) == false, "cell ( 1, 0) is not set");
    QVERIFY2( quadtree.get( 1 , 1) == false, "cell ( 1, 1) is not set");

    quadtree.dump();

}

void QuadTreeUnitTestTest::testCase_glider()
{
    golquadtree quadtree(10000, 10000);
    quadtree.set(-1 ,2);
    quadtree.set( 0 ,0);
    quadtree.set( 0 ,1);
    quadtree.set( 1 ,1);
    quadtree.set( 1 ,2);
    quadtree.dump();

    // # #
    //  ##
    //  #

    boost::unordered_set<point_t> new_cells;
    boost::unordered_set<point_t> dead_cells;
    quadtree.nextGeneration(new_cells, dead_cells);

    quadtree.dump();

    QVERIFY2( dead_cells.find( point_t(-1,0) ) != dead_cells.end(), "cell (-1,0) is dead" );
    QVERIFY2( dead_cells.find( point_t( 1,0) ) != dead_cells.end(), "cell ( 1,0) is dead" );
    QVERIFY2( dead_cells.size() == 2,    "There are only 2 dead cells");

    QVERIFY2( new_cells.find( point_t(0, 1) ) != dead_cells.end(), "cell (0, 1) is born" );
    QVERIFY2( new_cells.find( point_t(0,-1) ) != dead_cells.end(), "cell (0,-1) is born" );
    QVERIFY2( new_cells.size() == 2,    "There are only 2 new cells");

    QVERIFY2( quadtree.get(-1, -1) == false, "cell (-1,-1) is not set");
    QVERIFY2( quadtree.get(-1,  0) == false, "cell (-1, 0) is not set");
    QVERIFY2( quadtree.get(-1,  1) == false, "cell (-1, 1) is not set");
    QVERIFY2( quadtree.get( 0, -1) == true,  "cell ( 0,-1) is set");
    QVERIFY2( quadtree.get( 0,  0) == true,  "cell ( 0, 0) is set");
    QVERIFY2( quadtree.get( 0,  1) == true,  "cell ( 0, 1) is set");
    QVERIFY2( quadtree.get( 1, -1) == false, "cell ( 1,-1) is not set");
    QVERIFY2( quadtree.get( 1 , 0) == false, "cell ( 1, 0) is not set");
    QVERIFY2( quadtree.get( 1 , 1) == false, "cell ( 1, 1) is not set");


}
QTEST_APPLESS_MAIN(QuadTreeUnitTestTest)

#include "tst_quadtreeunittesttest.moc"
