/// \file
/// \brief The viewer class definitions

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

#include "viewer.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>

#include "alignment/alignment_context.hpp"
#include "common/algorithm/transform_build.hpp"
#include "common/size_t_literal.hpp"
#include "display/display_colourer/display_colourer.hpp"
#include "display/display_colourer/display_colourer_consecutive.hpp"
#include "display/options/display_spec.hpp"
#include "display/viewer/pymol/pymol_tools.hpp"
#include "display_colour/display_colour.hpp"
#include "display_colour/display_colour_list.hpp"
#include "exception/invalid_argument_exception.hpp"
#include "file/pdb/pdb.hpp"
#include "file/pdb/pdb_atom.hpp"
#include "file/pdb/pdb_residue.hpp"
#include "superposition/superposition_context.hpp"

#include <sstream>

using namespace boost::algorithm;
using namespace cath;
using namespace cath::align;
using namespace cath::common;
using namespace cath::detail;
using namespace cath::file;
using namespace cath::sup;

using boost::algorithm::is_space;
using boost::format;
using boost::irange;
using boost::lexical_cast;
using boost::numeric_cast;
using std::max;
using std::ostream;
using std::setfill;
using std::setw;
using std::string;
using std::unique_ptr;

/// \brief Default is to no accept multiple colourings
bool viewer::do_accepts_multiple_colourings() const {
	return false;
}

/// \brief Default to writing no commands to the specified ostream when beginning a colouring with the specified colourer
void viewer::do_begin_colouring(ostream                &/*arg_ostream*/, ///< The ostream to which the PyMOL commands should be written
                                const display_colourer &/*arg_colourer*/ ///< The display_colourer to be used for the colouring that is beginning
                                ) {
}

/// \brief Default to writing no commands to the specified ostream when ending a colouring with the specified colourer
void viewer::do_end_colouring(ostream                &/*arg_ostream*/, ///< The ostream to which the PyMOL commands should be written
                              const display_colourer &/*arg_colourer*/ ///< The display_colourer to be used for the colouring that is ending
                              ) {
}

/// \brief TODOCUMENT
string viewer::default_executable() const {
	return do_default_executable();
}

/// \brief TODOCUMENT
string viewer::default_file_extension() const {
	return do_default_file_extension();
}

/// \brief TODOCUMENT
void viewer::write_start(ostream &arg_ostream ///< TODOCUMENT
                         ) const {
	do_write_start(arg_ostream);
}

/// \brief TODOCUMENT
void viewer::write_load_pdbs(ostream             &arg_ostream,       ///< TODOCUMENT
                             const superposition &arg_superposition, ///< TODOCUMENT
                             const pdb_list      &arg_pdbs,          ///< TODOCUMENT
                             const str_vec       &arg_names          ///< TODOCUMENT
                             ) const {
	return do_write_load_pdbs(
		arg_ostream,
		arg_superposition,
		arg_pdbs,
		arg_names
	);
}

/// \brief TODOCUMENT
void viewer::define_colour(ostream             &arg_ostream,    ///< TODOCUMENT
                           const display_colour &arg_colour,     ///< TODOCUMENT
                           const string        &arg_colour_name ///< TODOCUMENT
                           ) const {
	do_define_colour(arg_ostream, arg_colour, arg_colour_name);
}

/// \brief TODOCUMENT
string viewer::get_colour_pdb_str(const string &arg_colour_name, ///< TODOCUMENT
                                  const string &arg_pdb_name     ///< TODOCUMENT
                                  ) const {
	return do_get_colour_pdb_str( arg_colour_name, arg_pdb_name );
}

/// \brief TODOCUMENT
string viewer::get_colour_pdb_residues_str(const string         &arg_colour_name, ///< TODOCUMENT
                                           const string         &arg_pdb_name,    ///< TODOCUMENT
                                           const residue_id_vec &arg_residues     ///< TODOCUMENT
                                           ) const {
	return do_get_colour_pdb_residues_str( arg_colour_name, arg_pdb_name, arg_residues );
}

/// \brief TODOCUMENT
void viewer::write_alignment_extras(ostream                     &arg_ostream,              ///< TODOCUMENT
                                    const superposition_context &arg_superposition_context ///< TODOCUMENT
                                    ) const {
	if ( ! arg_superposition_context.has_alignment() ) {
		BOOST_THROW_EXCEPTION(invalid_argument_exception("Cannot write alignment extras for superposition_context that doesn't contain an alignment"));
	}
	return do_write_alignment_extras( arg_ostream, arg_superposition_context );
}

/// \brief TODOCUMENT
void viewer::write_end(ostream &arg_ostream ///< TODOCUMENT
                       ) const {
	do_write_end(arg_ostream);
}

/// \brief Strip
///
/// \relates viewer
///
/// Current heuristic:
///  - Keep alpha-numeric characters as they are
///  - Convert all space characters to underscores
///  - Drop all other characters
///
/// \todo Find a better way to do this. It doesn't seem immediately obvious that replace_all()
///       in Boost string algorithm will do the trick. Does it require regexps?
string cath::clean_name_for_viewer(const string &arg_name
                                   ) {
	string new_name;
	new_name.reserve(arg_name.size());
	for (const char &name_char : arg_name) {
		if (is_alnum()(name_char)) {
			new_name.push_back(name_char);
		}
		else if ( is_space()( name_char ) || name_char == '_' ) {
			new_name.push_back('_');
		}
	}
	return new_name;
}

/// \brief TODOCUMENT
///
/// \relates viewer
str_vec cath::clean_names_for_viewer(const str_vec &arg_names
                                     ) {
	str_vec new_names;
	new_names.reserve(arg_names.size());
	for (const string &name : arg_names) {
		new_names.push_back(clean_name_for_viewer(name));
	}
	return new_names;
}

/// \brief TODOCUMENT
///
/// \relates viewer
str_vec cath::clean_names_for_viewer(const superposition_context &arg_superposition_context ///< TODOCUMENT
                                     ) {
	return clean_names_for_viewer( arg_superposition_context.get_names_cref() );
}

/// \brief TODOCUMENT
///
/// \relates viewer
str_vec cath::clean_names_for_viewer(const alignment_context &arg_alignment_context ///< TODOCUMENT
                                     ) {
	return clean_names_for_viewer( arg_alignment_context.get_names() );
}

/// \brief Output instructions from the specified viewer for the specified superposition_context to
///        the specified ostream, using the specified display_spec and only_warn flat
///
/// \relates viewer
void cath::output_superposition_to_viewer(ostream                     &arg_ostream,                 ///< The ostream to which the data should be written
                                          viewer                      &arg_viewer,                  ///< The viewer defining the instructions to be written
                                          const display_spec          &arg_display_spec,            ///< The specification for how to display the superposition
                                          const superposition_context &arg_superposition_context,   ///< The superposition_context to output
                                          const bool                  &arg_only_warn_on_missing_aln ///< Whether to only warn (rather than throwing) if no alignment is present
                                          ) {
	// Write the start of the viewer output
	arg_viewer.write_start(arg_ostream);

	// Write the text to load the PDBs
	arg_viewer.write_load_pdbs(
		arg_ostream,
		arg_superposition_context.get_superposition_cref(),
		arg_superposition_context.get_pdbs_cref(),
		clean_names_for_viewer( arg_superposition_context )
	);

	const bool spec_is_consecutive            = is_consecutive( arg_display_spec );
	const bool spec_requires_alignment        = requires_alignment( arg_display_spec );
	const bool supn_has_alignment             = arg_superposition_context.has_alignment();
	const bool missing_wanted_alignment       = spec_requires_alignment && ! supn_has_alignment;
	const bool would_accept_extra_consecutive = arg_viewer.accepts_multiple_colourings() && ! spec_is_consecutive;

	if ( missing_wanted_alignment || would_accept_extra_consecutive) {
		if ( missing_wanted_alignment ) {
			const auto message = "Unable to apply an alignment-based coluring scheme to the superposition because it doesn't contain an alignment";
			if ( arg_only_warn_on_missing_aln ) {
				BOOST_LOG_TRIVIAL( warning ) << message;
			}
			else {
				BOOST_THROW_EXCEPTION(invalid_argument_exception(message));
			}
		}
		const display_colourer_consecutive the_colourer{ get_colour_list( arg_display_spec ) };

		colour_viewer(
			the_colourer,
			arg_ostream,
			arg_viewer,
			clean_names_for_viewer( arg_superposition_context )
		);
	}

	if ( arg_superposition_context.has_alignment() ) {
		// Apply the colour
		const unique_ptr<const display_colourer> display_colourer_ptr = get_display_colourer( arg_display_spec );

		colour_viewer(
			*display_colourer_ptr,
			arg_ostream,
			arg_viewer,
			make_alignment_context( arg_superposition_context )
		);

		// If there is an alignment then do magic with it
		// if ( arg_superposition_context.has_alignment() ) {
		arg_viewer.write_alignment_extras( arg_ostream, arg_superposition_context );
	}

	// Write the start of the viewer output
	arg_viewer.write_end( arg_ostream );
}

/// \brief Generate a name to use in the viewer for the specified colour index
///        in the specified number of colours
string cath::generate_colour_name(const size_t &arg_colour_index, ///< The index of the colour to name
                                  const size_t &arg_num_colours   ///< The total number of colours
                                  ) {
	const size_t num_width  = lexical_cast<string>( max( static_cast<size_t>( 1_z ), arg_num_colours ) - 1 ).length();
	const string format_str = "%0" + ::std::to_string( num_width ) + "d";
	return "cath_tools_defined_colour_"
		+ ( format( format_str ) % arg_colour_index ).str();
}

/// \brief Generate names to use in the viewer for the specified number of colours
str_vec cath::generate_colour_names(const size_t &arg_num_colours ///< The total number of colours
                                    ) {
	return transform_build<str_vec>(
		irange( 0_z, arg_num_colours ),
		[&] (const size_t &x) { return generate_colour_name( x, arg_num_colours ); }
	);
}
