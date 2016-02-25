//Project
#include "superslider.h"

//Qt
#include <QMouseEvent>
#include <QDebug>
#include "qmimedata.h"
#include "qdrag.h"
#include "qwidgetaction.h"
#include "qapplication.h"
#include "qpixmap.h"
#include "qcursor.h"
#include "qguiapplication.h"
#include "qdir.h"
#include <QProxyStyle>

class SliderProxy : public QProxyStyle
{
public:
  int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const
  {
    switch(metric) {
    case PM_SliderThickness  : return 25;
    case PM_SliderLength     : return 25;
    default                  : return (QProxyStyle::pixelMetric(metric,option,widget));
    }
  }
};

SuperSlider::SuperSlider(QWidget *parent)
  : QSlider(parent)
{
  //styling
  setOrientation(Qt::Horizontal);
  setAcceptDrops(true);
  SliderProxy *aSliderProxy = new SliderProxy();
  connect(this, SIGNAL(sliderPressed()), this, SLOT(update()));

  setStyleSheet("QSlider::handle { image: url(:/icons/handle.png);}");
  setStyle(aSliderProxy);

  setValue(QSlider::maximum()+10);
  //setting up the alternate handle
  alt_handle = new SuperSliderHandle(this);
  addAction(new QWidgetAction(alt_handle));
  alt_handle->move(this->pos().x(), this->pos().y() +3);
  alt_handle->setValue(0);

  //set up timebar slider thing
  time_bar = new SuperSliderTimeBar(this);
  addAction(new QWidgetAction(time_bar));
  time_bar->move(this->pos().x()+30, this->pos().y());
}

SuperSliderHandle::SuperSliderHandle(SuperSlider *_parent)
  : QLabel(_parent)
{
  parent = _parent;
  filter = new SliderEventFilter(parent);

  //styling
  setAcceptDrops(true);
  QPixmap pix = QPixmap(":/icons/handle.png");
  pix =  pix.scaled(10, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  setPixmap(pix);
}

SuperSliderTimeBar::SuperSliderTimeBar(SuperSlider *_parent)
  : QLabel(_parent)
{
  parent = _parent;

  //styling
  QPixmap pix = QPixmap(":/icons/timePoint.png");
  pix =  pix.scaled(2, 25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  setPixmap(pix);
}

int SuperSlider::alt_value()
{
  return alt_handle->value();
}

void SuperSlider::alt_setValue(int value)
{
  alt_handle->setValue(value);
}

void SuperSlider::mousePressEvent(QMouseEvent *)
{
    qGuiApp->installEventFilter(this);
}

// The left handle
void SuperSlider::alt_update()
{
    int curser = (QPoint(this->mapFromGlobal(QCursor::pos()).x(),this->y())).x()-13;
    // ratio is curser's position from 0-100 in relation to the slider
    float ratio = ((float)curser)/((float)width()-25.0)*100.0;
    int offset = getOffset();
    int max = value();
    int min = 0;
    bool lessThanMax = ratio < max-offset;
    bool greaterThanMin = ratio > min;
    if(lessThanMax && greaterThanMin)
      alt_handle->setValue(ratio);
    else if (!lessThanMax)
        alt_handle->setValue(max-offset);
    else if (!greaterThanMin)
        alt_handle->setValue(min);
  emit alt_valueChanged(alt_value());
}

// The right handle
void SuperSlider::update()
{
    int curser = (QPoint(this->mapFromGlobal(QCursor::pos()).x(),this->y())).x()-13;
    // ratio is curser's position from 0-100 in relation to the slider
    float ratio = ((float)curser)/((float)width()-25.0)*100.0;
    int sliderVal = value();
    int offset = getOffset()+1;
    if (ratio > sliderVal +15 || ratio < sliderVal-15)
        return;
    int max = 100;
    int min = alt_handle->value();
    bool lessThanMax = ratio < max;
    bool greaterThanMin = ratio > min+offset;
    if(lessThanMax && greaterThanMin)
      this->setValue(ratio);
    else if (!lessThanMax)
        this->setValue(max);
    else if (!greaterThanMin)
        this->setValue(min+offset);
}

bool SuperSlider::eventFilter(QObject* obj, QEvent* event)
{
  switch(event->type())
  {
  case QEvent::MouseButtonRelease:
    qGuiApp->removeEventFilter(this);
    return true;
    break;
  case QEvent::MouseMove:
    update();
    return true;
    break;
  default:
    return QObject::eventFilter(obj, event);
  }
  return false;
}

void SuperSliderHandle::mousePressEvent(QMouseEvent *)
{
  qGuiApp->installEventFilter(filter);
  parent->clearFocus();
}

bool SliderEventFilter::eventFilter(QObject* obj, QEvent* event)
{
  switch(event->type())
  {
  case QEvent::MouseButtonRelease:
    qGuiApp->removeEventFilter(this);
    return true;
    break;
  case QEvent::MouseMove:
    grandParent->alt_update();
    return true;
    break;
  default:
    return QObject::eventFilter(obj, event);
  }
  return false;
}

void SuperSliderHandle::setValue(double in_value)
{
  float range = parent->width()-26.0; // 8 on left, 18 on right
  float ratio = in_value/100.0;
  int location = ratio*range + 8;
  move(location, y());
  m_value = in_value;
}

int SuperSliderHandle::value()
{
  return m_value;
}
void SuperSlider::Reset()
{
  int horBuffer = (alt_handle->width());
  QPoint myPos = mapToGlobal(pos());
  QPoint point(myPos.x() + width() - horBuffer, myPos.y()- alt_handle->height());
  point = alt_handle->mapFromParent(point);

  alt_handle->move(point);
  alt_handle->show();
  alt_handle->raise();

  time_bar->show();
  time_bar->raise();
}

void SuperSliderTimeBar::setValue(double value)
{
  double width = parent->width();
  double range = parent->maximum() - parent->minimum();
  int location = (value - parent->minimum())/range;
  location = location *width;
  move(y(),location);
}

int SuperSliderTimeBar::value()
{
  double width = parent->width(), position = pos().x();
  double value = position/width;
  double range = parent->maximum() - parent->minimum();
  return parent->minimum() + (value * range);
}

void SuperSlider::resized(){
    alt_handle->setValue(alt_handle->value());
}

int SuperSlider::getOffset() {
    // What about 2...?
    if (width() > 400)
        return 1;
    else if (width() > 310)
        return 3;
    else if (width() > 250)
        return 4;
    else if (width() > 200)
        return 5;
    else if (width() > 180)
        return 6;
    else if (width() > 162)
        return 7;
    else if (width() > 144)
        return 8;
    else
        return 9;
}

void SuperSlider::setTimebarPosition(int newPos) {
    float range = width()-26.0; // 8 on left, 18 on right
    float ratio = newPos/100.0;
    int location = ratio*range + 8;

    time_bar->move(location, y());

}

void SuperSlider::catchCurrentFrameChange(int newPos) {
    qDebug()<<"SuperSlider: New pos: "<<newPos;
    float range = width()-26.0; // 8 on left, 18 on right
    float ratio = newPos/100.0;
    int location = ratio*range + 8;
    qDebug()<<"SuperSlider: location: "<<location;

    time_bar->move(location, y());
}
