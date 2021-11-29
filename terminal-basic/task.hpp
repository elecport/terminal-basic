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

#include "HALProxyStream.hpp"

/**
 * @brief Dynamyc task base class
 * 
 * Tasks may be created and destroyed dynamically at run-time.
 * allowing to create multiapp environment
 */

#pragma once

class Task
{
protected:

	Task(BASIC::HALProxyStream&);

  void getString(char*, uint8_t);
	/**
	 * Text I/O stream at the top of HAL terminal interface
	 */
	BASIC::HALProxyStream &m_halproxyStream;

public:

	virtual ~Task() = default;
	/**
	 * Initialize task
	 */
	virtual void init() = 0;
	/**
	 * Task step
	 * 
	 * return task active flag
	 */
	virtual bool step() = 0;
};
