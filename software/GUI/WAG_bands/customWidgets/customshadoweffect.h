#pragma once
/**
Code found here:
http://stackoverflow.com/questions/23718827/qt-shadow-around-window
**/

#include <QGraphicsDropShadowEffect>
#include <QGraphicsEffect>

class CustomShadowEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    explicit CustomShadowEffect(QObject *parent);
    ~CustomShadowEffect(){}
    void draw(QPainter* painter);
    QRectF boundingRectFor(const QRectF& rect) const;

    inline void setDistance(qreal distance) { _distance = distance; updateBoundingRect(); }
    inline qreal distance() const { return _distance; }

    inline void setBlurRadius(qreal blurRadius) { _blurRadius = blurRadius; updateBoundingRect(); }
    inline qreal blurRadius() const { return _blurRadius; }

    inline void setColor(const QColor& color) { _color = color; }
    inline QColor color() const { return _color; }

private:
    qreal  _distance;
    qreal  _blurRadius;
    QColor _color;
};
