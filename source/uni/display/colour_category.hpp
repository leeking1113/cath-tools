/// \file
/// \brief The colour_category class header

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

#ifndef _CATH_TOOLS_SOURCE_DISPLAY_COLOUR_CATEGORY_H
#define _CATH_TOOLS_SOURCE_DISPLAY_COLOUR_CATEGORY_H

namespace cath {

	/// \brief Represent the category of use of a colour
	///
	/// The motivation is to avoid colours' names clashing
	enum class colour_category : char {
		STRUC_ONLY,  ///< Using a colour only for colouring whole structures
		STRUC_OR_RES ///< Using a colour for colouring structures or residues
	};

} // namespace cath

#endif