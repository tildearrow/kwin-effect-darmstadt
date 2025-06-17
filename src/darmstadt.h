/*
    SPDX-FileCopyrightText: 2024 Jin Liu <m.liu.jin@gmail.com>, 2025 tildearrow <me@tildearrow.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "effect/effect.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <QSocketNotifier>

namespace KWin
{

class Cursor;

class DarmstadtEffect : public Effect
{
    Q_OBJECT

public:
    DarmstadtEffect();
    ~DarmstadtEffect() override;

    static bool supported();
    bool isActive() const override;

private:
    Cursor *m_cursor;
    bool m_isActive = false;
    int m_listenfd;
    QSocketNotifier* m_observe;
    QList<int> m_clients;
    struct sockaddrA {
      sa_family_t family;
      char path[PATH_MAX];
    } m_sockaddr;
};

} // namespace KWin
