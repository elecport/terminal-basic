/*
 * This file is part of Terminal-BASIC: a lightweight BASIC-like language
 * interpreter.
 * 
 * Copyright (C) 2016-2018 Andrey V. Skvortsov <starling13@mail.ru>
 * Copyright (C) 2019-2021 Terminal-BASIC team
 *     <https://github.com/terminal-basic-team>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "task.hpp"
#include "ascii.hpp"
#include <Arduino.h>

Task::Task(BASIC::HALProxyStream& stream):
    m_halproxyStream(stream)
{
}

void
Task::getString(char* buf, uint8_t size)
{
  uint8_t bufferPos = 0;
  if (size == 0)
    return;
  while (true) {
    HAL_time_sleep_ms(10);
    HAL_update();
    if (m_halproxyStream.available()) {
      char c = m_halproxyStream.read();
      if (c == char(ASCII::LF) || c == char(ASCII::CR)) {
        buf[bufferPos] = '\0';
        return;
      } else if (c == char(ASCII::DEL)) {
        if (bufferPos > 0) {
          --bufferPos;
          m_halproxyStream.write(c);
        }
      } else {
        if (bufferPos < (size-1)) {
          buf[bufferPos++] = c;
          m_halproxyStream.write(c);
        }
      } // end if
    } // end if
  } // end while
}
