/*
    SPDX-FileCopyrightText: 2024 Jin Liu <m.liu.jin@gmail.com>, 2025 tildearrow <me@tildearrow.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "darmstadt.h"

namespace KWin
{

KWIN_EFFECT_FACTORY_SUPPORTED(DarmstadtEffect,
                              "metadata.json",
                              return DarmstadtEffect::supported();)

} // namespace KWin

#include "main.moc"
