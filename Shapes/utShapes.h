#ifndef UTSHAPES_H_INCLUDED
#define UTSHAPES_H_INCLUDED

#include "..\cppunitlite\TestHarness.h"
#include "Shapes.h"
#include "ShapeMedia.h"
#include <vector>
#include <math.h>


const double epsilon = 0.000001;
// added this line
TEST (first, Rectangle)
{
    Rectangle rect(0,0,4,2);
    DOUBLES_EQUAL(8,rect.area(),epsilon);
}

TEST(sencond,Circle)
{
    Circle circ(0,0,10);
    DOUBLES_EQUAL(300,circ.area(),epsilon);
}

TEST (fifth, sumOfArea)
{
    Rectangle r1(0,0,4,2);
    Circle c1(0,0,10);
    std::vector<Shape *> ss;
    ss.push_back(&r1);
    ss.push_back(&c1);
    DOUBLES_EQUAL(308,sumOfArea(ss),epsilon);
}

TEST ( seven, media ) {
    Rectangle r1(0,0,4,2);
    ShapeMedia sR1( &r1 ) ;
    DOUBLES_EQUAL(8,sR1.area(),epsilon);
}

TEST ( eight, comboMedia ) {
    Rectangle r1(0,0,4,2);
    Rectangle r2(0,0,4,3);
    ShapeMedia sR1( &r1 ) ;
    ShapeMedia sR2( &r2 ) ;
    std::vector<Media *> ss{&sR1,&sR2};
    ComboMedia cm(ss);
    DOUBLES_EQUAL(20,cm.area(),epsilon) ;
}

#include <iostream>

TEST (ComboMedia, DescriptionVisitor) {
    Rectangle r1(0,0,4,2);
    Circle r2(0,0,10);
    ShapeMedia sR1( &r1 ) ;
    ShapeMedia sR2( &r2 ) ;
    std::vector<Media *> ss{&sR1,&sR2};
    ComboMedia cm(ss);
    DescriptionVisitor dv;
    cm.accept(&dv);
    CHECK(std::string("combo(r(0 0 4 2) c(0 0 10) )") == dv.getDescription());
}

/*TEST (ShapeMedia, DescriptionVisitor) {
    Rectangle r1(0,0,4,2);
    ShapeMedia sR1( &r1 ) ;
    DescriptionVisitor dv;
    sR1.accept(&dv);
    CHECK(std::string("r(0 0 4 2) ") == dv.getDescription());
}*/

TEST (IllegalAdd, ShapeMedia) {
    Rectangle r1(0,0,4,2);
    Circle r2(0,0,10);
    ShapeMedia sR1( &r1 ) ;
    ShapeMedia sR2( &r2 ) ;
    try {
        sR1.add(&sR2);
        FAIL("should not be here");
    } catch(std::string s) {
        CHECK(std::string("Illegal: add on media") == s);
    }
}

TEST (Hexagon ,ComboMedia) {
    Rectangle r1(0,0,2,sqrt(12));
    Triangle t1(0,0,-1,sqrt(3),0,sqrt(12));
    Triangle t2(2,0,3,sqrt(3),2,sqrt(12));
    ShapeMedia sR1(&r1);
    ShapeMedia sT1(&t1);
    ShapeMedia sT2(&t2);
    std::vector<Media *> ss{&sR1};
    ComboMedia cm(ss);
    DOUBLES_EQUAL(sqrt(48),cm.area(),epsilon);
    DOUBLES_EQUAL(sqrt(48) + 4,cm.perimeter(),epsilon);
    cm.add(&sT1);
    DOUBLES_EQUAL(sqrt(75),cm.area(),epsilon);
    DOUBLES_EQUAL(sqrt(108) + 8,cm.perimeter(),epsilon);
    cm.add(&sT2);
    DOUBLES_EQUAL(sqrt(108),cm.area(),epsilon);
    DOUBLES_EQUAL(8 * sqrt(3) + 12,cm.perimeter(),epsilon);
}

TEST (AreaVisitor ,MediaVisitor){
    Rectangle r1(0,0,2,sqrt(12));
    Triangle t1(0,0,-1,sqrt(3),0,sqrt(12));
    ShapeMedia sR1(&r1);
    ShapeMedia sT1(&t1);
    std::vector<Media *> ss{&sR1,&sT1};
    ComboMedia cm(ss);
    Triangle t2(2,0,3,sqrt(3),2,sqrt(12));
    ShapeMedia sT2(&t2);
    std::vector<Media *> ss1{&cm,&sT2};
    ComboMedia cm1(ss1);
    AreaVisitor av;
    cm1.accept(&av);
    DOUBLES_EQUAL(sqrt(108),av.getArea(),epsilon);
    cm.accept(&av);
    DOUBLES_EQUAL(sqrt(75),av.getArea(),epsilon);
    DOUBLES_EQUAL(sqrt(48),r1.area(),epsilon);
    DOUBLES_EQUAL(sqrt(3),t1.area(),epsilon);
    DOUBLES_EQUAL(sqrt(3),t2.area(),epsilon);
}

TEST (PerimeterVisitor, MediaVisitor){
    Rectangle r1(0,0,2,sqrt(12));
    Triangle t1(0,0,-1,sqrt(3),0,sqrt(12));
    ShapeMedia sR1(&r1);
    ShapeMedia sT1(&t1);
    std::vector<Media *> ss{&sR1,&sT1};
    ComboMedia cm(ss);
    Triangle t2(2,0,3,sqrt(3),2,sqrt(12));
    ShapeMedia sT2(&t2);
    std::vector<Media *> ss1{&cm,&sT2};
    ComboMedia cm1(ss1);
    PerimeterVisitor pv;
    cm1.accept(&pv);
    DOUBLES_EQUAL(8*sqrt(3) + 12,pv.getPerimeter(),epsilon);
    cm.accept(&pv);
    DOUBLES_EQUAL(sqrt(108) + 8,pv.getPerimeter(),epsilon);
    DOUBLES_EQUAL(4*sqrt(3) + 4,r1.perimeter(),epsilon);
    DOUBLES_EQUAL(2*sqrt(3) + 4,t1.perimeter(),epsilon);
    DOUBLES_EQUAL(2*sqrt(3) + 4,t2.perimeter(),epsilon);
}

TEST (ShapeMediaBuilder, ShapeMediaBuilder){
    ShapeMediaBuilder sb;
    Circle cir(0,0,5);
    sb.buildShapeMedia(&cir);
    DescriptionVisitor dv;
    sb.getMedia()->accept(&dv);
    CHECK(std::string("c(0 0 5) ") == dv.getDescription());
}

TEST (ComboMediaBuilder, ComboMediaBuilder){
    ComboMediaBuilder cb;
    cb.buildComboMedia();
    Rectangle r1(10,0,15,5);
    Circle c1(12,5,2);
    cb.buildShapeMedia(&r1);
    cb.buildShapeMedia(&c1);

    ComboMediaBuilder cb1;
    cb1.buildComboMedia();
    cb1.buildAddComboMedia(cb.getMedia());
    Rectangle r2(0,0,25,20);
    cb1.buildShapeMedia(&r2);

    ComboMediaBuilder cb2;
    cb2.buildComboMedia();
    cb2.buildAddComboMedia(cb1.getMedia());
    Triangle t1(0,20,16,32,25,20);
    cb2.buildShapeMedia(&t1);

    DescriptionVisitor dv;
    cb2.getMedia()->accept(&dv);
    //std::cout << dv.getDescription() << std::endl;
    CHECK(std::string("combo(combo(combo(r(10 0 15 5) c(12 5 2) )r(0 0 25 20) )t(0 20 16 32 25 20) )")==dv.getDescription())
}

TEST(TextMedia, TextMedia){
    Rectangle r1(0,0,4,2);
    std::string s1 = "hello";
    TextMediaBuilder tmb;
    tmb.buildTextMedia(r1,s1);
    //Media *tm = tmb.getMedia();
    //TextMedia *tm = (TextMedia)(*m);
    DOUBLES_EQUAL(((TextMedia*)tmb.getMedia())->getRectangle().area() , 8, epsilon);
    CHECK(((TextMedia*)tmb.getMedia())->getText() == "hello");
}

TEST(RemoveMedia,RemoveMedia){
    ComboMediaBuilder cb;
    cb.buildComboMedia();
    Rectangle r1(10,0,15,5);
    Circle c1(12,5,2);
    cb.buildShapeMedia(&r1);
    cb.buildShapeMedia(&c1);

    ComboMediaBuilder cb1;
    cb1.buildComboMedia();
    cb1.buildAddComboMedia(cb.getMedia());
    Rectangle r2(0,0,25,20);
    cb1.buildShapeMedia(&r2);

    ComboMediaBuilder cb2;
    cb2.buildComboMedia();
    cb2.buildAddComboMedia(cb1.getMedia());
    Triangle t1(0,20,16,32,25,20);
    cb2.buildShapeMedia(&t1);

    cb2.getMedia()->removeMedia(cb1.getShapeMedia());
    DescriptionVisitor dv;
    cb2.getMedia()->accept(&dv);
    //std::cout << dv.getDescription() << std::endl;
    CHECK(std::string("combo(combo(combo(r(10 0 15 5) c(12 5 2) ))t(0 20 16 32 25 20) )") == dv.getDescription());
}
#endif // UTSHAPES_H_INCLUDED
