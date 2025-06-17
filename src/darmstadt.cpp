/*
    SPDX-FileCopyrightText: 2024 Jin Liu <m.liu.jin@gmail.com>, 2025 tildearrow <me@tildearrow.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "darmstadt.h"

#include "cursor.h"
#include "effect/effecthandler.h"

#include <QPainter>

Q_LOGGING_CATEGORY(KWIN_DARMSTADT, "kwin_effect_darmstadt", QtWarningMsg)

namespace KWin
{

DarmstadtEffect::DarmstadtEffect()
    : m_cursor(Cursors::self()->mouse())
    , m_listenfd(-1)
    , m_observe(NULL)
{
  qCDebug(KWIN_DARMSTADT) << "Opening Darmstadt socket...";

  if ((m_listenfd=socket(AF_UNIX,SOCK_STREAM,0))<0) {
    qCCritical(KWIN_DARMSTADT) << "Unable to create Darmstadt socket! " << strerror(errno);
    return;
  }

  m_sockaddr.family=AF_UNIX;
  strncpy(m_sockaddr.path,"/tmp/darmcursor",PATH_MAX);

  if (bind(m_listenfd,(struct sockaddr*)&m_sockaddr,sizeof(sa_family_t)+strlen(m_sockaddr.path)+1)<0) {
    if (::connect(m_listenfd,(struct sockaddr*)&m_sockaddr,sizeof(sa_family_t)+strlen(m_sockaddr.path)+1)<0) {
      qCDebug(KWIN_DARMSTADT) << "Removing previous socket...";
      if (unlink(m_sockaddr.path)<0) {
        qCCritical(KWIN_DARMSTADT) << "Unable to remove Darmstadt socket! " << strerror(errno);
        close(m_listenfd);
        return;
      }
    } else {
      qCCritical(KWIN_DARMSTADT) << "Unable to bind Darmstadt socket because it already exists! " << strerror(errno);
      close(m_listenfd);
      return;
    }
    if (bind(m_listenfd,(struct sockaddr*)&m_sockaddr,sizeof(sa_family_t)+strlen(m_sockaddr.path)+1)<0) {
      qCCritical(KWIN_DARMSTADT) << "Unable to bind Darmstadt socket! " << strerror(errno);
      close(m_listenfd);
      return;
    }
  }

  if (listen(m_listenfd,4)<0) {
    qCCritical(KWIN_DARMSTADT) << "Unable to listen on Darmstadt socket! " << strerror(errno);
    close(m_listenfd);
    return;
  }

  m_observe=new QSocketNotifier(m_listenfd,QSocketNotifier::Read);
  connect(m_observe, &QSocketNotifier::activated, this, [this](QSocketDescriptor socket, QSocketNotifier::Type type) {
    int newClient=accept(m_listenfd,NULL,NULL);
    if (newClient<0) {
      qCCritical(KWIN_DARMSTADT) << "Unable to accept connection to Darmstadt socket! " << strerror(errno);
    } else {
      // greet with current cursor position
      float buf[2];
      QPointF pos=m_cursor->pos();
      QPointF hot=m_cursor->hotspot();

      buf[0]=pos.x()-hot.x();
      buf[1]=pos.y()-hot.y();

      int len=write(newClient,buf,sizeof(buf));
      if (len!=sizeof(buf)) {
        if (errno==EPIPE) {
          close(newClient);
        }
      } else {
        m_clients.append(newClient);
      }
    }
  });

  qCDebug(KWIN_DARMSTADT) << "Darmstadt socket opened.";

  auto sendNewPosition=[this]() {
    // write to socket if connected
    float buf[2];
    QPointF pos=m_cursor->pos();
    QPointF hot=m_cursor->hotspot();

    buf[0]=pos.x()-hot.x();
    buf[1]=pos.y()-hot.y();

    for (int& i: m_clients) {
      if (i==-1) continue;
      int len=write(i,buf,sizeof(buf));
      if (len!=sizeof(buf)) {
        if (errno==EPIPE) {
          close(i);
          i=-1;
        }
      }
    }
  };

  connect(m_cursor, &Cursor::posChanged, this, sendNewPosition);
  connect(m_cursor, &Cursor::cursorChanged, this, sendNewPosition);
}

DarmstadtEffect::~DarmstadtEffect()
{
  if (m_observe) {
    delete m_observe;
    m_observe=NULL;
  }
  for (int i: m_clients) {
    if (i==-1) continue;
    close(i);
    i=-1;
  }
  m_clients.clear();
  if (m_listenfd>=0) {
    close(m_listenfd);
    m_listenfd=-1;
    qCDebug(KWIN_DARMSTADT) << "Darmstadt socket closed.";
  }
}

bool DarmstadtEffect::supported()
{
    return true;
}

bool DarmstadtEffect::isActive() const
{
    return m_isActive;
}

} // namespace KWin

#include "moc_darmstadt.cpp"
