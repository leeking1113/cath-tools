/// \file
/// \brief The open_ifstream / open_ofstream header

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

#ifndef _CATH_TOOLS_SOURCE_SRC_COMMON_COMMON_FILE_OPEN_FSTREAM_HPP
#define _CATH_TOOLS_SOURCE_SRC_COMMON_COMMON_FILE_OPEN_FSTREAM_HPP

#include <boost/filesystem.hpp>

#include "common/exception/runtime_error_exception.hpp"

#include <iosfwd>

namespace cath {
	namespace common {
		namespace detail {

			enum class fstream_type : bool {
				READING,
				WRITING
			};

			/// \brief Function used to implement open_ifstream and open_ofstream
			///
			/// This:
			/// - sets the exceptions to throw on failbit or badbit
			/// - tries to open the file
			/// - catches any failure and rethrows as a runtime_error_exception
			template <typename fstream_t>
			void open_fstream_impl(fstream_t                     &prm_fstream,     ///< TODOCUMENT
			                       const boost::filesystem::path &prm_filename,    ///< TODOCUMENT
			                       const std::ios_base::openmode &prm_mode,        ///< TODOCUMENT
			                       const fstream_type            &prm_fstream_type ///< TODOCUMENT
			                       ) {
				const bool is_reading                     = (prm_fstream_type == fstream_type::READING);
				const std::string r_or_w_str              = is_reading ? "reading" : "writing";
				const std::ios_base::iostate r_or_w_state = is_reading ? (                     std::ios::badbit )
				                                                       : ( std::ios::failbit | std::ios::badbit );
				prm_fstream.exceptions( r_or_w_state );
				try {
					prm_fstream.open(prm_filename.string().c_str(), prm_mode);
				}
				// Catch any I/O exceptions
				catch (const std::exception &ex) {
			//		const std::string reading_or_writing_str = (prm_fstream_type == FILE_READING) ? "reading" : "writing";
					const std::string error_message(
						"Cannot open file \""
						+ prm_filename.string()
						+ "\" for "
						+ r_or_w_str
						+ " ["
						+ ex.what()
						+ "] : "
						+ std::strerror( errno )
					);
					BOOST_THROW_EXCEPTION(cath::common::runtime_error_exception(error_message));
				};

				assert(prm_fstream.is_open());
				assert(prm_fstream.good());
			}
		} // namespace detail

		void open_ifstream(std::ifstream &,
		                   const boost::filesystem::path &,
		                   const std::ios_base::openmode & = std::ios_base::in);

		void open_ofstream(std::ofstream &,
		                   const boost::filesystem::path &,
		                   const std::ios_base::openmode & = std::ios_base::out);

	} // namespace common
} // namespace cath

#endif
