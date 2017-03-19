/// \file
/// \brief The pdb_file_superposition_outputter class header

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

#ifndef _CATH_TOOLS_SOURCE_OUTPUTTER_SUPERPOSITION_OUTPUTTER_PDB_FILE_SUPERPOSITION_OUTPUTTER_H
#define _CATH_TOOLS_SOURCE_OUTPUTTER_SUPERPOSITION_OUTPUTTER_PDB_FILE_SUPERPOSITION_OUTPUTTER_H

#include <boost/filesystem/path.hpp>

#include "outputter/superposition_outputter/superposition_outputter.hpp"
#include "superposition/superposition_content_spec.hpp"

namespace cath {
	namespace opts {

		/// \brief TODOCUMENT
		class pdb_file_superposition_outputter final : public superposition_outputter {
		private:
			/// \brief TODOCUMENT
			const boost::filesystem::path output_file;

			/// \brief The specification of what should be included in the superposition
			sup::superposition_content_spec content_spec;

			virtual std::unique_ptr<superposition_outputter> do_clone() const override final;
			virtual void do_output_superposition(const sup::superposition_context &,
			                                     std::ostream &) const override final;
			virtual bool do_involves_display_spec() const override final;

		public:
			pdb_file_superposition_outputter(const boost::filesystem::path &,
			                                 const sup::superposition_content_spec &);
		};

	} // namespace opts
} // namespace cath

#endif
