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

#include "btchat_task.hpp"
#include "ascii.hpp"

#include "BluetoothSerial.h"

BTChat::BTChat(BASIC::HALProxyStream& stream):
    Task(stream)
{
  m_btSerial = new BluetoothSerial;
  m_send.data[0] = '\0'; m_send.position = 0;
  m_receive.data[0] = '\0'; m_receive.position = 0;
}

BTChat::~BTChat()
{
  delete m_btSerial;
}

void
BTChat::init()
{
  HAL_gfx_setmode(2);
  m_halproxyStream.println("\e[34mBLUETOOTH CHAT\e[32m\n");
  
  m_halproxyStream.println("r. Create chat");
  m_halproxyStream.println("j. Request join to the chat");
  m_halproxyStream.println("q. Quit\e[0m");
}

bool
BTChat::step()
{
  HAL_time_sleep_ms(10);
  if (m_state == State_t::CHAT_SLAVE)
    return stepSlave();
  if (m_halproxyStream.available()) {
    char c = m_halproxyStream.read();
    if (c == 'q')
      return false;
    else if (c == 'j') {
      char buf[8];
      m_halproxyStream.print("Enter your ID: ");
      this->getString(buf, 9);
      if (strlen(buf) > 0) {
        m_btSerial->begin(buf);
        m_halproxyStream.print("\n\rUsing ID ");
        m_halproxyStream.println(buf);
        HAL_time_sleep_ms(2000);
        HAL_gfx_setmode(1);
        m_state = State_t::CHAT_SLAVE;
      } else {
        m_halproxyStream.print("\r\n\e[31mEmpty string!\e[0m");
        HAL_time_sleep_ms(2000);
        init();
      }
    }
  }
	return true;
}

bool
BTChat::stepSlave()
{
  if (m_btSerial->available()) {
    char c = m_btSerial->read();
    if (c == char(ASCII::CR)) {
      m_receive.data[m_receive.position] = '\0';
      m_halproxyStream.print("\e[24;1H\e[33m");
      m_halproxyStream.print(m_receive.data);
      m_halproxyStream.println("\e[0m");
      m_halproxyStream.print(m_send.data);
      m_receive.position = 0;
      m_receive.data[0] = '\0';
    } else if (c == char(ASCII::LF)) {
      // null
    } else if (m_receive.position < 40){
      m_receive.data[m_receive.position++] = c;
    }
  }
  if (m_halproxyStream.available()) {
    char c = m_halproxyStream.read();
    if (c == char(ASCII::CR)) {
      m_send.data[m_send.position] = '\0';
      m_btSerial->println(m_send.data);

      m_halproxyStream.print("\e[24;1H\e[34m");
      m_halproxyStream.print(m_send.data);
      m_halproxyStream.println("\e[0m");
      
      m_send.position = 0;
      m_send.data[0] = '\0';
    } else if (c == char(ASCII::LF)) {
      // null
    } else if (c == char(ASCII::DEL)) {
      if (m_send.position > 0) {
        --m_send.position;
        m_send.data[m_send.position] = '\0';
        m_halproxyStream.write(char(ASCII::BS));
        m_halproxyStream.write(' ');
        m_halproxyStream.write(char(ASCII::BS));
      }
    } else if (m_send.position < 40) {
      m_halproxyStream.write(c);
      m_send.data[m_send.position++] = c;
      m_send.data[m_send.position] = '\0';
    }
  }
  return true;
}
