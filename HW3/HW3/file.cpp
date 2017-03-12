//
//  main.cpp
//  HW3
//
//  Created by Kevin on 2/12/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

class File {
public:
    File(string name);
    virtual ~File();
    string name() const;
    virtual void open() const = 0;
    virtual void redisplay() const;
private:
    string m_name;
};

class TextMsg : public File {
public:
    TextMsg(string name);
    virtual ~TextMsg();
    virtual void open() const ;
};

class Video : public File {
public:
    Video(string name, int time);
    virtual ~Video();
    virtual void open() const;
    virtual void redisplay() const;
private:
    int m_time;
};

class Picture : public File {
public:
    Picture(string name);
    virtual ~Picture();
    virtual void open() const ;
};

//File Stuff

File :: File(string name){
    m_name = name;
}

File::~File(){}

string File::name() const{
    return m_name;
}

void File::redisplay() const {
    cout << "refresh the screen";
}

//TextMsg stuff
TextMsg::TextMsg(string name):File(name){}
TextMsg::~TextMsg(){
    cout << "Destroying " << File::name() << ", a text message" << endl;
}
void TextMsg::open() const {
    cout << "open text message";
}
//Video stuff
Video::Video(string name, int time):File(name){
    m_time = time;
}
Video::~Video(){
    cout << "Destroying " << File::name() << ", a video" << endl;
}
void Video::open() const {
    cout << "play " << m_time << " second video";
}
void Video::redisplay() const {
    cout << "replay video";
}
//Picture stuff
Picture::Picture(string name):File(name){}
Picture::~Picture(){
    cout << "Destroying the picture " << File::name() << endl;
}
void Picture::open() const {
    cout << "show picture";
}

void openAndRedisplay(const File* f)
{
    cout << f->name() << ": ";
    f->open();
    cout << endl << "Redisplay: ";
    f->redisplay();
    cout << endl;
}

