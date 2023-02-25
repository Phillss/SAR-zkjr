#include "algom.h"

Algom::Algom()
{

}
Algom::Algom(QString name_cn,QString name_py,int default_t,int min_t,int max_t){
    this->name_cn=name_cn;
    this->name_py=name_py;
    this->default_t=default_t;
    this->min_t=min_t;
    this->max_t=max_t;
}
Algom::Algom(QString name_cn,QString name_py,int default_t,int min_t,int max_t,int win_hei,int win_wid){
    this->name_cn=name_cn;
    this->name_py=name_py;
    this->default_t=default_t;
    this->min_t=min_t;
    this->max_t=max_t;
    this->win_hei=win_hei;
    this->win_wid=win_wid;
}
QString Algom::getNamecn(){
    return name_cn;
}
QString Algom::getNamepy(){
    return name_py;
}
int Algom::getDefaultt(){
    return default_t;
}
int Algom::getmaxt(){
    return max_t;
}
int Algom::getmint(){
    return min_t;
}
int Algom::getWinhei(){
    return win_hei;
}
int Algom::getWinwid(){
    return win_wid;
}
bool Algom::getFlag(){
    return flag;
}
void Algom::setNamecn(QString name){
    this->name_cn=name;
}
void Algom::setNamepy(QString name){
    this->name_py=name;
}
void Algom::setDefaultt(int t){
    this->min_t=t;
}
void Algom::setmint(int t){
    this->min_t=t;
}
void Algom::setmaxt(int t){
    this->max_t=t;
}
void Algom::setWinhei(int h){
    this->win_hei=h;
}
void Algom::setWinwid(int w){
    this->win_wid=w;
}
void Algom::setFlag(bool flag){
    this->flag=flag;
}
QString Algom::tostring(){
    return name_cn+":"+name_py+":"+QString::number(min_t)+":"+QString::number(max_t)+":"+QString::number(win_hei)+":"+QString::number(win_wid);
}
