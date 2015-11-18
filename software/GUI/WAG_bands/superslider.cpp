//Project
#include "superslider.h"

//Qt
#include <QMouseEvent>
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
  alt_handle->move(this->pos().x()+8, this->pos().y() +3);

  //set up timebar slider thing
//  time_bar = new SuperSliderTimeBar(this);
//  addAction(new QWidgetAction(time_bar));
//  time_bar->move(this->pos().x()+30, this->pos().y());
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

void SuperSlider::mousePressEvent(QMouseEvent *mouseEvent)
{
    qGuiApp->installEventFilter(this);
}

// The left handle
void SuperSlider::alt_update()
{
    // curser position
  QPoint point(alt_handle->mapToParent(alt_handle->mapFromGlobal(QCursor::pos())).x(),alt_handle->y());
  QPoint max = QPoint((this->x()+value()-295)/.430434, alt_handle->y());
  QPoint min(alt_handle->mapToParent(alt_handle->mapFromGlobal(QPoint(this->pos().x()+10, alt_handle->y()))).x(),alt_handle->y());
  bool lessThanMax = point.x() < max.x();
  bool greaterThanMin = point.x() > min.x();
  qDebug("slider: %i, min: %i, max: %i, point: %i", alt_handle->x(), min.x(), max.x(), point.x());
  if(lessThanMax && greaterThanMin)
    alt_handle->move(point);
  else if(!lessThanMax)
      alt_handle->move(max);
  else if(!greaterThanMin)
      alt_handle->move(min);
  emit alt_valueChanged(alt_value());
}

// The right handle
void SuperSlider::update()
{
    // curser in same coordinate frame as slider
    int curser = ((QPoint(this->mapFromGlobal(QCursor::pos()).x(),this->y())).x()-10)/2.33;
    int sliderVal = value();
    if (curser > sliderVal +8 || curser < sliderVal-8)
        return;
    int max = 100; // -10 for offset.
    int min = (alt_handle->x()-9)*0.4323144; // +6 is for offset of left handle. +5 is to go next to left handle
    qDebug("sliderVal: %i, min: %i", sliderVal, min);
    bool lessThanMax = curser < max;
    bool greaterThanMin = curser > min+5;
    if(lessThanMax && greaterThanMin)
      this->setValue(curser);
    else if (!lessThanMax)
        this->setValue(max);
    else if (!greaterThanMin)
        this->setValue(min+5);
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

void SuperSliderHandle::mousePressEvent(QMouseEvent *mouseEvent)
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

void SuperSliderHandle::setValue(double value)
{
  double width = parent->width(), position = pos().x();
  double range = parent->maximum() - parent->minimum();
  int location = (value - parent->minimum())/range;
  location = location *width;
  move(y(),location);
}

int SuperSliderHandle::value()
{
  double width = parent->width(), position = pos().x();
  double value = position/width;
  double range = parent->maximum() - parent->minimum();
  return parent->minimum() + (value * range);
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
  double width = parent->width(), position = pos().x();
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
