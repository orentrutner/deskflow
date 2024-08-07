/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2024 Symless Ltd.
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "gui/messages.h"

#include <QApplication>
#include <gtest/gtest.h>

class QtTest : public ::testing::Test {
public:
  static void SetUpTestSuite() {
    char **argv = nullptr;
    int argc = 0;
    s_app = std::make_unique<QApplication>(argc, argv);
    qInstallMessageHandler(synergy::gui::messages::messageHandler);
  }

  static void TearDownTestSuite() { s_app.reset(); }

  static std::unique_ptr<QApplication> s_app;
};