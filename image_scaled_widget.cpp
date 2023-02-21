#include "image_scaled_widget.h"


image_scaled_widget::image_scaled_widget(int w,int h)
{
    QByteArray image;
    int with=w,heigth= h;
    memset(original_arrays,255,sizeof(original_arrays));
    image.append((char*)original_arrays,sizeof(original_arrays));
    QImage cur_show_image=QImage((uchar*)image.data(),with,heigth,with,QImage::Format_Indexed8);
    m_pix=QPixmap::fromImage(cur_show_image);
    vec_original_array.resize(320*256);
    m_scaleValue = 1.0;
    m_scaleDafault = 1.0;
    m_isMove = false;
    setAcceptDrops(true);//If enabled is true, this item will accept hover events; otherwise, it will ignore them. By default, items do not accept hover events.
    setAcceptHoverEvents(true);

}

void image_scaled_widget::change_new_image(QPixmap *pixmap,QByteArray &arry)
{//更换图片
    m_pix = *pixmap;
    memcpy(&vec_original_array[0],arry.data(),arry.size());
}


QRectF image_scaled_widget::boundingRect() const
{
    return QRectF(-m_pix.width() / 2, -m_pix.height() / 2,
                  m_pix.width(), m_pix.height());
}
void image_scaled_widget::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    painter->drawPixmap(-m_pix.width() / 2, -m_pix.height() / 2, m_pix);
}
void image_scaled_widget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，


        m_isMove = true;//标记鼠标左键被按下
    }
    else if(event->button() == Qt::RightButton)
    {
        ResetItemPos();//右击鼠标重置大小
    }

}
#include <QWidget>
void image_scaled_widget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isMove)
    {
        QPointF point = (event->pos() - m_startPos)*m_scaleValue;
        moveBy(point.x(), point.y());
    }


}

void image_scaled_widget::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{


    int x=event->pos().x()+(qreal)m_pix.width()/2;
    int y=event->pos().y()+(qreal)m_pix.height()/2;
    int offset=y*m_pix.width()+x;
   //QColor temp_color= m_pix.toImage().pixelColor(QPoint(x,y));
    //this->setToolTip(QString("X:%1,Y:%2 R:%3,G:%4,B:%5").arg(x).arg(y).arg(temp_color.red()).arg(temp_color.green()).arg(temp_color.blue()));
    if(offset<vec_original_array.size())
    {

        //this->setToolTip(QString("%1,%2,%3").arg(x).arg(y).arg(vec_original_array.at(offset)));
    }

}
void image_scaled_widget::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    m_isMove = false;//标记鼠标左键已经抬起
}
void image_scaled_widget::wheelEvent(QGraphicsSceneWheelEvent *event)//鼠标滚轮事件
{
    if((event->delta() > 0)&&(m_scaleValue >= 50))//最大放大到原始图像的50倍
    {
        return;
    }
    else if((event->delta() < 0)&&(m_scaleValue <= m_scaleDafault))//图像缩小到自适应大小之后就不继续缩小
    {
        ResetItemPos();//重置图片大小和位置，使之自适应控件窗口大小
    }
    else
    {
        qreal qrealOriginScale = m_scaleValue;
        if(event->delta() > 0)//鼠标滚轮向前滚动
        {
            m_scaleValue*=1.1;//每次放大10%
        }
        else
        {
            m_scaleValue*=0.9;//每次缩小10%
        }
        setScale(m_scaleValue);
        if(event->delta() > 0)
        {
            moveBy(-event->pos().x()*qrealOriginScale*0.1, -event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
        }
        else
        {
            moveBy(event->pos().x()*qrealOriginScale*0.1, event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
        }
    }
}
void image_scaled_widget::setQGraphicsViewWH(int nwidth, int nheight)//将主界面的控件QGraphicsView的width和height传进本类中，并根据图像的长宽和控件的长宽的比例来使图片缩放到适合控件的大小
{
    return;
    int nImgWidth = m_pix.width();
    int nImgHeight = m_pix.height();
    qreal temp1 = nwidth*1.0/nImgWidth;
    qreal temp2 = nheight*1.0/nImgHeight;
    if(temp1>temp2)
    {
        m_scaleDafault = temp2;
    }
    else
    {
        m_scaleDafault = temp1;
    }
    setScale(m_scaleDafault);
    m_scaleValue = m_scaleDafault;
}
void image_scaled_widget::ResetItemPos()//重置图片位置
{
    m_scaleValue = m_scaleDafault;//缩放比例回到一开始的自适应比例
    setScale(m_scaleDafault);//缩放到一开始的自适应大小
    setPos(0,0);
}
qreal image_scaled_widget::getScaleValue() const
{
    return m_scaleValue;
}
