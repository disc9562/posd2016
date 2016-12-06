#ifndef SHAPEMEDIA_H_INCLUDED
#define SHAPEMEDIA_H_INCLUDED

#include "Shapes.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>
class ShapeMedia;
class ComboMedia;
class MediaVisitor{
public:
    virtual void visitComboMedia(ComboMedia *cm) = 0;
    virtual void visitShapeMedia(ShapeMedia *sm) = 0;
};

class AreaVisitor : public MediaVisitor{
public:
    void visitComboMedia(ComboMedia *cm);
    void visitShapeMedia(ShapeMedia *sm);
    double getArea() const {return total;}
private:
    double total = 0;
};

class PerimeterVisitor : public MediaVisitor{
public:
    void visitComboMedia(ComboMedia *cm);
    void visitShapeMedia(ShapeMedia *sm);
    double getPerimeter() const {return total;}
private:
    double total = 0;
};
class DescriptionVisitor {
public:
    DescriptionVisitor (): desc(std::string("")) {}
    void visitShapeMedia(ShapeMedia *sm);
    void visitComboMedia(ComboMedia *cm);
    std::string getDescription() const {return desc;}
private:
    std::string desc;
};

class Media {
public :
    virtual double area() const {} ;
    virtual double perimeter() const {} ;
    virtual void accept(MediaVisitor * mv) {};
    virtual void accept(DescriptionVisitor * dv) {};
    virtual void removeMedia(Media *m) {};
    virtual void add(Media * m) {
        throw std::string("Illegal: add on media");
    }
    virtual std::string text(Rectangle boundingbox, std::string text) const{};
};

class ShapeMedia :public Media {
private:
    Shape* shape ;
public :
    ShapeMedia( Shape* s ): shape(s) {}
    virtual void accept(DescriptionVisitor * dv) {
        dv->visitShapeMedia(this);
    }
    void accept(MediaVisitor * mv){
        mv -> visitShapeMedia(this);
    }
    double area() const { return shape->area() ; }
    double perimeter() const { return shape->perimeter() ; }
    Shape * getShape()const {return shape;}
};

class ComboMedia : public Media {
private:
    std::vector<Media*> media;
public:
    ComboMedia( std::vector<Media*>m ) : media(m) {}
    ComboMedia () {}
    double area( ) const {
        double total =0;
        for (Media *m: media)
            total += m->area();
        return total;
    }
    double perimeter() const{
        double total = 0;
        for (Media *m: media)
            total += m->perimeter();
        return total;
    }
    void accept(MediaVisitor * mv){
        for (Media *m:media)
            m->accept(mv);
        mv->visitComboMedia(this);
        }
    void accept(DescriptionVisitor * dv) {
        for (Media *m: media)
            m->accept(dv);
        dv->visitComboMedia(this);
    }
    void add (Media *m) {
        media.push_back(m);
    }
    void removeMedia(Media *m){
        std::vector <Media *>::const_iterator it;
        for(it = media.begin();it != media.end();++it){
            if((*it) == m){
                it = media.erase(it);
                break;
            }
            else
                (*it)->removeMedia(m);
        }
        /*std::vector<Media*> temp;
        for (Media *m: media)
        {
            if(m == removeEle)
                removeMedia(removeEle);
            else
                temp.push_back(m);
        }
        media = temp;*/
        //return temp.size();
    }
};

class TextMedia : public Media{
public:
    TextMedia(Rectangle boundingbox, std::string text):r(boundingbox), txt(text){};
    std::string getText(){return txt;}
    Rectangle getRectangle(){return r;}
private:
    std::string txt;
    Rectangle r;
};

void AreaVisitor::visitShapeMedia(ShapeMedia *sm){
    total = sm->area();
}

void PerimeterVisitor::visitShapeMedia(ShapeMedia *sm){
    total = sm->perimeter();
}

void DescriptionVisitor::visitShapeMedia(ShapeMedia *sm) {
    desc += sm->getShape()->description();
}
void AreaVisitor::visitComboMedia(ComboMedia *cm){
    total = cm->area();
}
void PerimeterVisitor::visitComboMedia(ComboMedia *cm){
    total = cm->perimeter();
}


void DescriptionVisitor::visitComboMedia(ComboMedia *cm) {
    desc = std::string("combo(")+desc+std::string(")");
}

class MediaBuilder{
public:
    virtual void buildComboMedia() = 0;
    virtual void buildShapeMedia(Shape *s) = 0;
    virtual Media *getMedia() = 0;
};

class ShapeMediaBuilder : public MediaBuilder{
public:
    ShapeMediaBuilder () : shape(0){}
    void buildComboMedia(){}
    void buildShapeMedia(Shape *s){
        shape = new ShapeMedia(s);
    }
    Media * getMedia() {return shape;}
private:
    ShapeMedia * shape;
};

class ComboMediaBuilder : public MediaBuilder
{
public:
    ComboMediaBuilder () :combo(0) {}
    void buildComboMedia() {combo = new ComboMedia;}
    void buildShapeMedia(Shape *s){
        shape = new ShapeMedia(s);
        combo->add(shape);
    }
    void buildAddComboMedia(Media * cm){
        combo->add(cm);
    }
    Media *getMedia() {return combo;}
    Media *getShapeMedia() {return shape;}
private:
    ComboMedia *combo;
    ShapeMedia * shape;
};

class TextMediaBuilder : public MediaBuilder{
public:
    TextMediaBuilder ():text(0){}
    void buildComboMedia(){}
    void buildShapeMedia(Shape *s){}
    void buildTextMedia(Rectangle r,std::string s){
        text = new TextMedia(r,s);
    }
    Media *getMedia(){return text;}
private:
    TextMedia *text;
};
#endif // SHAPEMEDIA_H_INCLUDED
