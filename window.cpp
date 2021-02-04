/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "renderarea.h"
#include "window.h"
#include <math.h>
#include <QPushButton>
#include <QFont>

//! [1]
Window::Window()
{
//    m_cursor_timer.setInterval(5);
//    m_cursor_timer.setSingleShot(false);
//    connect(&m_cursor_timer,SIGNAL(timeout()),this,SLOT(sltGetMousePos()));
//    m_cursor_timer.start();

    m_x0=-1;
    m_x1=-1;
    m_y0=-1;
    m_y1=-1;
    m_drawing = false;
//    m_x0 = 0;
//    m_y0 = 0;
//    m_x1 = 20;
//    m_y1 = 400;
//    path.moveTo(m_x0, m_y0);
//    path.quadTo((m_x1+m_x0)/2,(m_y1+m_y0)/2,m_x1,m_y1);
    renderAreas = new RenderArea(path);
    renderAreas->setStyleSheet("background-color:#FFFFFF;");
    mainLayout = new QGridLayout;
    mainLayout->addWidget(renderAreas, 0, 0, 1, 4);

    QFontDatabase::addApplicationFont("/opt/arm/qt5/lib/fonts/homa.ttf");
    QFont font_homa("IRHoma", 32, QFont::Normal);

    QPushButton* btnReset = new QPushButton();
    btnReset->setText(tr("Retry"));
    btnReset->setFont(font_homa);
    btnReset->setMinimumSize(200,80);
    btnReset->setStyleSheet("font-size: 32px; background-color:orange; border-radius: 10px;");
    connect(btnReset,SIGNAL(clicked()),this,SLOT(sltResetSignPad()));
    mainLayout->addWidget(btnReset, 1, 0);

    QPushButton* btnSubmit = new QPushButton();
    btnSubmit->setText(tr("Submit"));
    btnSubmit->setFont(font_homa);
    btnSubmit->setMinimumSize(200,80);
    btnSubmit->setStyleSheet("font-size: 32px; background-color:orange;  border-radius: 10px;");
    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(sltSubmitSignature()));
    mainLayout->addWidget(btnSubmit, 1, 3);

    setLayout(mainLayout);
    this->setStyleSheet("background-color:#FFFFFF;");

   //-----------------------------------------------

//    m_x0 = m_x1;
//    m_y0 = m_y1;
//    m_x1 = 400;
//    m_y1 = 50;
//    path.moveTo(m_x0, m_y0);
//    path.quadTo((m_x1+m_x0)/2,(m_y1+m_y0)/2,m_x1,m_y1);
//    renderAreas->updatePath(path);
    setWindowTitle(tr("Painter Paths"));

    qApp->installEventFilter(this);
}

bool Window::eventFilter(QObject *obj, QEvent *event)
{

  if (event->type() == QEvent::MouseButtonPress)
  {
      qDebug()<<renderAreas->width()<<","<<renderAreas->height();
      QPoint globalCursorPos = QCursor::pos();
      if(globalCursorPos.x()<renderAreas->x() ||
              globalCursorPos.x()>(renderAreas->x()+renderAreas->width()) ||
              globalCursorPos.y()<renderAreas->y() ||
              globalCursorPos.y()>(renderAreas->y()+renderAreas->height()))
      {
          return false;
      }

      m_x1 = globalCursorPos.x()-renderAreas->x();
      m_y1 = globalCursorPos.y()-renderAreas->y();
      path.moveTo(m_x1, m_y1);
      path.lineTo(QPoint(m_x1+1,m_y1));
      renderAreas->updatePath(path);

      m_drawing = true;
  }
  else if (m_drawing && event->type() == QEvent::MouseMove)
  {
//    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
//    statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
      QPoint globalCursorPos = QCursor::pos();
      if(globalCursorPos.x()<renderAreas->x() ||
              globalCursorPos.x()>(renderAreas->x()+renderAreas->width()) ||
              globalCursorPos.y()<renderAreas->y() ||
              globalCursorPos.y()>(renderAreas->y()+renderAreas->height()))
      {
          return false;
      }
      m_x0 = m_x1;
      m_y0 = m_y1;
      m_x1 = globalCursorPos.x()-renderAreas->x();
      m_y1 = globalCursorPos.y()-renderAreas->y();
      if(m_x0>=0 && m_y0>=0 && m_x1>=0 && m_y1>=0)
      {
          if(m_x0!=m_x1 || m_y0!=m_y1)
          {
              renderAreas->enableSavingImage();
              path.moveTo(m_x0, m_y0);
              path.quadTo((m_x1+m_x0)/2,(m_y1+m_y0)/2,m_x1,m_y1);
  //            path.cubicTo((m_x1+m_x0)/2,(m_y1+m_y0)/2,(m_x1+m_x0)/2,(m_y1+m_y0)/2,m_x1,m_y1);
              renderAreas->updatePath(path);
              qDebug()<<globalCursorPos;
          }
      }
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
      m_drawing = false;
      m_x0=-1;
      m_x1=-1;
      m_y0=-1;
      m_y1=-1;
  }
  else if (event->type() == QEvent::MouseButtonDblClick)
  {
      m_drawing = false;
      m_x0=-1;
      m_x1=-1;
      m_y0=-1;
      m_y1=-1;

      renderAreas->saveSignature();

      path = QPainterPath();
      renderAreas->updatePath(path);
  }
  return false;
}

void Window::sltSubmitSignature(){
    renderAreas->saveSignature();
    this->close();
}

void Window::sltResetSignPad(){
    m_drawing = false;
    m_x0=-1;
    m_x1=-1;
    m_y0=-1;
    m_y1=-1;

    path = QPainterPath();
    renderAreas->updatePath(path);
}
