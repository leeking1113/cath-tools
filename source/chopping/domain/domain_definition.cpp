/// \file
/// \brief The domain_definition class definitions

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

#include "domain_definition.hpp"

#include <boost/filesystem/path.hpp>

using namespace boost::filesystem;
using namespace cath;
using namespace cath::chop;
using namespace cath::file;
using namespace cath::opts;
using namespace std;

/// \brief Ctor for domain_definition
domain_definition::domain_definition(const domain &arg_domain,  ///< TODOCUMENT
                                     const string &arg_pdb_name ///< TODOCUMENT
                                     ) : the_domain( arg_domain   ),
                                         pdb_name  ( arg_pdb_name ) {
}

/// \brief TODOCUMENT
const domain & domain_definition::get_domain() const {
	return the_domain;
}

/// \brief TODOCUMENT
const string & domain_definition::get_pdb_name() const {
	return pdb_name;
}
