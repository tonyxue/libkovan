/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

/**
 * \file depth_exception.hpp
 * \brief This file describes the class Exception
 * \author Stefan Zeltner
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _DEPTH_EXCEPTION_HPP_
#define _DEPTH_EXCEPTION_HPP_

#include <string>
#include <stdexcept>
#include "export.h"

namespace depth
{
  class EXPORT_SYM Exception : public std::runtime_error
  {
  public:
    Exception(const std::string &msg);
  };
}

#endif
