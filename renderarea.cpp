#include "renderarea.h"
#include <QFileDialog>
#include <QPainter>

//! [0]
RenderArea::RenderArea(const QPainterPath &path, QWidget *parent)
    : QWidget(parent), path(path)
{
    penWidth = 1;
    rotationAngle = 0;
    m_drawing = false;
    setBackgroundRole(QPalette::Base);
    is_saving_image = false;
}
//! [0]

//! [8]
void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
//! [8] //! [9]
//    painter.scale(width() / 100.0, height() / 100.0);
//    painter.translate(50.0, 50.0);
//    painter.rotate(-rotationAngle);
//    painter.translate(-50.0, -50.0);

//! [9] //! [10]
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QLinearGradient gradient(0, 0, 0, 100);
//    gradient.setColorAt(0.0, fillColor1);
//    gradient.setColorAt(1.0, fillColor2);
    painter.setBrush(gradient);
    painter.drawPath(path);
}
//! [10]

void RenderArea::updatePath(QPainterPath &new_path)
{
    if(!m_drawing)
    {
        m_drawing = true;
        path = new_path;
        update();
        m_drawing = false;
    }
}

void RenderArea::saveSignature(){
    if(!is_saving_image)
    {
        is_saving_image = true;
        QImage img(this->size(), QImage::Format_ARGB32);
        QPainter painter2(&img);
        this->render(&painter2);
        QString path = "./test.bmp";
        img.save(path);
    }
}

