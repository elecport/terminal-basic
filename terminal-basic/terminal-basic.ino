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

#include "basic_task.hpp"
#include "btchat_task.hpp"

static Task* activeTask;
static BASIC::HALProxyStream halproxyStream(0);

static void _init()
{
  HAL_gfx_setmode(2);
	halproxyStream.println("Select application:");
	halproxyStream.println("1. Terminal-BASIC");
  halproxyStream.println("2. Bluetooth chat");
}

void
setup()
{
	HAL_initialize();
	_init();
}

void
loop()
{
	HAL_update();
	if (activeTask == nullptr) {
		HAL_time_sleep_ms(10);
		if (halproxyStream.available()) {
			char c = halproxyStream.read();
			if (c == '1')
				activeTask = new BASIC::Task(halproxyStream);
			else if (c == '2')
				activeTask = new BTChat(halproxyStream);
			else
				return;
		} else
			return;
		// POSTCONDITION: activeTask is a newly created task object
		activeTask->init();
	}
	if (!activeTask->step()) {
		delete activeTask;
		activeTask = nullptr;
		_init();
	}
}
