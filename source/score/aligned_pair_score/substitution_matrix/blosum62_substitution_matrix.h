/// \file
/// \brief The make_subs_matrix_blosum62 header

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

#ifndef _CATH_TOOLS_SOURCE_SCORE_ALIGNED_PAIR_SCORE_SUBSTITUTION_MATRIX_BLOSUM62_SUBSTITUTION_MATRIX_H
#define _CATH_TOOLS_SOURCE_SCORE_ALIGNED_PAIR_SCORE_SUBSTITUTION_MATRIX_BLOSUM62_SUBSTITUTION_MATRIX_H

namespace cath { namespace score { class substitution_matrix; } }

namespace cath {
	namespace score {
		substitution_matrix make_subs_matrix_blosum62();
	} // namespace score
} // namespace cath

#endif


