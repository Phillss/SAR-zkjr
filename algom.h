#ifndef ALGOM_H
#define ALGOM_H
#include <QString>

class Algom
{
public:
    Algom();
    Algom(QString name_cn,QString name_py,int default_t,int min_t,int max_t);
    Algom(QString name_cn,QString name_py,int default_t,int min_t,int max_t,int win_hei,int win_wid);
    QString getNamecn();
    QString getNamepy();
    int getDefaultt();
    int getmint();
    int getmaxt();
    int getWinhei();
    int getWinwid();
    bool getFlag();
    bool getProfileFlag();
    void setNamecn(QString name);
    void setNamepy(QString name);
    void setDefaultt(int t);
    void setmint(int t);
    void setmaxt(int t);
    void setWinhei(int h);
    void setWinwid(int w);
    void setFlag(bool flag);
    void setProfileFlag(bool flag);
    QString tostring();
private:
    QString name_cn,name_py;
    int default_t,min_t,max_t;
    int win_hei,win_wid;
    bool flag=false,profileFlag=false;
};

#endif // ALGOM_H
