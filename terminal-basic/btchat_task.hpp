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

class BluetoothSerial;

class BTChat : public Task
{
public:

	explicit BTChat(BASIC::HALProxyStream&);

  ~BTChat() override;

private:

  enum class State_t
  {
    IDLE, CHAT_SLAVE
  };

  struct Buffer_t
  {
    char data[40];
    uint8_t position;
  };

  bool stepSlave();

  BluetoothSerial *m_btSerial;

  State_t m_state;

  Buffer_t m_receive, m_send;

	// Task interface
public:

	void init() override;

	bool step() override;
};
