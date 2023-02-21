#ifndef IMAGE_SCALED_WIDGET_H
#define IMAGE_SCALED_WIDGET_H

#include <QWidget>
#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QMouseEvent>
#include <QPointF>
#include <QDragEnterEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>
#include <QVector>
enum Enum_ZoomState{
    NO_STATE,
    RESET,
    ZOOM_IN,
    ZOOM_OUT
};
class image_scaled_widget : public QGraphicsItem
{
public:
    image_scaled_widget(int w=320,int h=256);
    void change_new_image(QPixmap *pixmap,QByteArray &arry);
    QRectF  boundingRect() const;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void    wheelEvent(QGraphicsSceneWheelEvent *event);
    void    ResetItemPos();
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    qreal   getScaleValue() const;
    void    setQGraphicsViewWH(int nwidth,int nheight);
private:
    qreal       m_scaleValue;
    qreal       m_scaleDafault;
    QPixmap     m_pix;
    int         m_zoomState;
    bool        m_isMove;
    QPointF     m_startPos;
    quint16     original_arrays[256][320];//delete me
    QVector<quint16>    vec_original_array;
};

#endif // IMAGE_SCALED_WIDGET_H
