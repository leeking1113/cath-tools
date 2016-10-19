/// \file
/// \brief The pair insertion operator header

/// \copyright
/// Tony Lewis's Common C++ Library Code (here imported into the CATH Tools project and then tweaked, eg namespaced in cath)
/// Copyright (C) 2007, Tony Lewis
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _CATH_TOOLS_SOURCE_COMMON_PAIR_INSERTION_OPERATOR_H
#define _CATH_TOOLS_SOURCE_COMMON_PAIR_INSERTION_OPERATOR_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <utility>

namespace std {

	/// \brief An insertion operator for a pair
	///
	/// It's rather unpleasant to be putting this into the std namespace, which is bad practice
	/// Unfortunately, this is the way to get things to work in the Boost Test library.
	template <typename T, typename U>
	ostream & operator<<(ostream          &arg_os,
	                     const pair<T, U> &arg_pair
	                     ) {
		ostringstream temp_ss;
		temp_ss << "pair<";
		temp_ss << typeid( T ).name();
		temp_ss << ", ";
		temp_ss << typeid( U ).name();
		temp_ss << ">(";
		temp_ss << std::right << std::setw( 7 ) << arg_pair.first;
		temp_ss << ",";
		temp_ss << std::right << std::setw( 7 ) << arg_pair.second;
		temp_ss << " )";
		arg_os << temp_ss.str();
		return arg_os;
	}
} // namespace std

#endif
