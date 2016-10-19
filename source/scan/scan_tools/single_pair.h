/// \file
/// \brief The single_pair class header

/// \copyright
/// CATH Tools - Protein structure comparison tools such as SSAP and SNAP
/// Copyright (C) 2011, Orengo Group, University College London
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

#ifndef _CATH_TOOLS_SOURCE_SCAN_SCAN_TOOLS_SINGLE_PAIR_H
#define _CATH_TOOLS_SOURCE_SCAN_SCAN_TOOLS_SINGLE_PAIR_H

#include "scan/scan_tools/scan_type.h"

namespace cath { class protein_list; }
namespace cath { namespace scan { class record_scores_scan_action; } }

namespace cath {
	namespace scan {

		/// \brief TODOCUMENT
		class single_pair : public scan_type {
		private:
			virtual std::unique_ptr<scan_type> do_clone() const override final;

			virtual std::pair<record_scores_scan_action, scan_metrics> do_perform_scan(const protein_list &,
			                                                                           const protein_list &) const override final;

		public:
			virtual ~single_pair() noexcept = default;
		};
	} // namespace scan
} // namespace cath

#endif
