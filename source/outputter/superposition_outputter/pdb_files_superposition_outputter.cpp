/// \file
/// \brief The pdb_files_superposition_outputter class definitions

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

#include "pdb_files_superposition_outputter.hpp"

#include <boost/range/irange.hpp>

#include "common/clone/make_uptr_clone.hpp"
#include "common/size_t_literal.hpp"
#include "file/pdb/pdb.hpp"
#include "superposition/io/superposition_io.hpp"
#include "superposition/superposition_context.hpp"

using namespace cath::common::literals;
using namespace cath::common;
using namespace cath::file;
using namespace cath::opts;
using namespace cath::sup;

using boost::filesystem::path;
using boost::irange;
using std::ostream;
using std::unique_ptr;

/// \brief A standard do_clone method.
unique_ptr<superposition_outputter> pdb_files_superposition_outputter::do_clone() const {
	return { make_uptr_clone( *this ) };
}

/// \brief TODOCUMENT
void pdb_files_superposition_outputter::do_output_superposition(const superposition_context &arg_supn_context, ///< TODOCUMENT
                                                                ostream                     &/*arg_ostream*/   ///< TODOCUMENT
                                                                ) const {
	const pdb_list  pdbs  = get_supn_content_pdbs( arg_supn_context, content_spec );
	const str_vec  &names = arg_supn_context.get_names();

	for (const size_t &pdb_ctr : irange( 0_z, pdbs.size() ) ) {
		write_superposed_pdb_to_file(
			arg_supn_context.get_superposition(),
			( output_dir / names[ pdb_ctr ] ).string(),
			pdbs[ pdb_ctr ],
			pdb_ctr
		);
	}
}

/// \brief TODOCUMENT
bool pdb_files_superposition_outputter::do_involves_display_spec() const {
	return false;
}

/// \brief Ctor for pdb_files_superposition_outputter
pdb_files_superposition_outputter::pdb_files_superposition_outputter(const path                       &arg_output_dir,  ///< TODOCUMENT
                                                                     const superposition_content_spec &arg_content_spec ///< The specification of what should be included in the superposition
                                                                     ) : output_dir  { arg_output_dir   },
                                                                         content_spec{ arg_content_spec } {
}

