/// \file
/// \brief The ssap_ostream_alignment_outputter class header

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

#ifndef _CATH_TOOLS_SOURCE_OUTPUTTER_ALIGNMENT_OUTPUTTER_SSAP_OSTREAM_ALIGNMENT_OUTPUTTER_H
#define _CATH_TOOLS_SOURCE_OUTPUTTER_ALIGNMENT_OUTPUTTER_SSAP_OSTREAM_ALIGNMENT_OUTPUTTER_H

#include "outputter/alignment_outputter/alignment_outputter.h"

namespace cath {
	namespace opts {

		/// \brief TODOCUMENT
		class ssap_ostream_alignment_outputter final : public alignment_outputter {
		private:
			virtual std::unique_ptr<alignment_outputter> do_clone() const override final;
			virtual void do_output_alignment(const align::alignment_context &,
			                                 std::ostream &) const override final;
			virtual bool do_involves_display_spec() const override final;

		public:
			virtual ~ssap_ostream_alignment_outputter() noexcept = default;
		};

	} // namespace opts
} // namespace cath

#endif
