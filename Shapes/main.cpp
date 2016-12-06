#include "..\cppunitlite\TestHarness.h"
#include "utShapes.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#define DELIMS " \t\r\n"
using namespace std;
Shape * newShape(const string temp);
int main()
{
    TestResult tr;
    TestRegistry::runAllTests(tr);
    string cmd;
    //string arg[10];
    map<string, Media*> mapMedia;
    while(cmd != "exit"){
        cout<< ":- ";
        getline(cin, cmd);
        cout << cmd << endl;

        stringstream ss(cmd);
        string sub_str;
        vector<string> cmdBuffer;

        while(getline(ss, sub_str, ' '))
            cmdBuffer.push_back(sub_str);

        string temp = cmdBuffer[cmdBuffer.size() - 1];
        //cout << temp << endl;


        if(cmd.find("def") != string::npos){
            Shape * s = newShape(temp);
            if(temp.find("Circle(") == 0){
                cout << "in Circle" <<endl;
                mapMedia.insert(pair<string, Media*>(cmdBuffer[1], new ShapeMedia(s)));
            }
            else if(temp.find("Rectangle(") == 0){
                cout << "in Rect" <<endl;
                mapMedia.insert(pair<string, Media*>(cmdBuffer[1], new ShapeMedia(s)));
            }
            else{
                int firstIndex = temp.find('{',0);
                int endIndex = temp.find('}',0);
                cout << firstIndex << ":::" << endIndex <<endl;
                string shapeName = temp.substr(firstIndex + 1, endIndex - (firstIndex + 1));
                cout << shapeName <<endl;

                stringstream ss(shapeName);
                string sub_str;
                vector<string> name;
                ComboMedia *cm = new ComboMedia;
                while(getline(ss, sub_str, ',')){
                    map<string, Media*>::iterator iter = mapMedia.find(sub_str);
                    cout << iter -> first << endl;
                    cm->add(iter -> second);
                }
                mapMedia.insert(pair<string, Media*>(cmdBuffer[1], cm));
                //cout << cm.
            }
        }
        else if(cmd.find(".area") != string::npos){
            int pointIndex = cmd.find('.',0);
            string shapeName = cmd.substr(0,pointIndex);
            map<string, Media*>::iterator iter;
            iter = mapMedia.find(shapeName);
            AreaVisitor av;
            Media *m;

            m = iter -> second;
            m -> accept(&av);
            cout << av.getArea() << endl;
        }

        else if(cmd.find(".perimeter") != string::npos){
            int pointIndex = cmd.find('.',0);
            string shapeName = cmd.substr(0, pointIndex);
            map<string, Media*>::iterator iter;
            iter = mapMedia.find(shapeName);
            PerimeterVisitor pv;
            Media *m;

            m = iter -> second;
            m -> accept(&pv);
            cout << pv.getPerimeter() << endl;
        }
        else if (cmdBuffer[0] == "delete"){
            map<string, Media*>::iterator iter;
            if(cmdBuffer[2] == "from"){
                iter = mapMedia.find(cmdBuffer[3]);
                Media *cm = iter -> second;

                iter = mapMedia.find(cmdBuffer[1]);
                cm -> removeMedia(iter -> second);
            }
            else{
                iter = mapMedia.find(cmdBuffer[1]);

            }
        }
        else if (cmd.find("show") != string::npos){
            for(map<string, Media*>::iterator iter = mapMedia.begin(); iter != mapMedia.end(); iter++){
                cout << iter -> first << endl;
            }
        }
        else{
            cout << "error" << endl;
        }
    }
    return 0;
}

Shape * newShape(const string temp){
    int startIndex = temp.find('(',0);
    int endIndex = temp.find(')',0);
    //cout << startIndex << ":::" << endIndex << endl;
    string argv = temp.substr(startIndex + 1, endIndex);
    stringstream ss(argv);
    string sub_str;
    vector<string> argvBuffer;

    while(getline(ss, sub_str, ','))
        argvBuffer.push_back(sub_str);

    switch(argvBuffer.size()){
        case 3:{
            stringstream toDouble1(argvBuffer[0]);
            stringstream toDouble2(argvBuffer[1]);
            stringstream toDouble3(argvBuffer[2]);
            double x, y, r;
            toDouble1 >> x;
            toDouble2 >> y;
            toDouble3 >> r;
            return new Circle(x, y, r);
        }
        case 4:{
            stringstream toDouble1(argvBuffer[0]);
            stringstream toDouble2(argvBuffer[1]);
            stringstream toDouble3(argvBuffer[2]);
            stringstream toDouble4(argvBuffer[3]);
            double x, y, l, h;
            toDouble1 >> x;
            toDouble2 >> y;
            toDouble3 >> l;
            toDouble4 >> h;
            return new Rectangle(x, y, l, h);
        }
        default:
            return nullptr;
    }
}

