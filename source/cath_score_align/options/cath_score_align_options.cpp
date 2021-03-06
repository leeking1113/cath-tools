/// \file
/// \brief The cath_score_align_options class definitions

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

#include "cath_score_align_options.hpp"

#include <boost/program_options.hpp>
#include <boost/shared_array.hpp>

#include "acquirer/alignment_acquirer/alignment_acquirer.hpp"
#include "acquirer/pdbs_acquirer/file_list_pdbs_acquirer.hpp"
#include "acquirer/pdbs_acquirer/istream_pdbs_acquirer.hpp"
#include "acquirer/selection_policy_acquirer/selection_policy_acquirer.hpp"
#include "alignment/alignment.hpp"
#include "alignment/common_atom_selection_policy/common_atom_select_ca_policy.hpp"
#include "alignment/common_residue_selection_policy/common_residue_select_all_policy.hpp"
#include "alignment/common_residue_selection_policy/common_residue_select_best_score_percent_policy.hpp"
#include "chopping/domain/domain.hpp"
#include "chopping/region/region.hpp"
#include "common/argc_argv_faker.hpp"
#include "common/exception/invalid_argument_exception.hpp"
#include "common/exception/not_implemented_exception.hpp"
#include "common/exception/runtime_error_exception.hpp"
#include "common/type_aliases.hpp"
#include "file/pdb/pdb.hpp"
#include "file/pdb/pdb_atom.hpp"
#include "file/pdb/pdb_residue.hpp"
#include "outputter/alignment_outputter/alignment_outputter.hpp"
#include "outputter/alignment_outputter/alignment_outputter_list.hpp"
#include "outputter/superposition_outputter/superposition_outputter.hpp"
#include "outputter/superposition_outputter/superposition_outputter_list.hpp"
#include "superposition/superposition_context.hpp"

using namespace cath;
using namespace cath::align;
using namespace cath::common;
using namespace cath::file;
using namespace cath::opts;

using boost::none;
using std::istream;
using std::string;
using std::unique_ptr;

/// \brief The name of the program that uses this executable_options
const string cath_score_align_options::PROGRAM_NAME("cath-score-align");

/// \brief Get the name of the program that uses this executable_options
string cath_score_align_options::do_get_program_name() const {
	return PROGRAM_NAME;
}

/// \brief Review all specified options and return a string containing any errors or a help string
///        (possibly using a description of all visible options)
///
/// This is a concrete definition of a virtual method that's pure in executable_options
///
/// This should only be called by executable_options, as the last step of the parse_options()
/// method, after all real parsing has completed.
///
/// \pre The options should have been parsed
///
/// \returns Any error/help string arising from the newly specified options
///          or an empty string if there aren't any
str_opt cath_score_align_options::do_get_error_or_help_string() const {
	// Grab the objects from the options blocks
	const auto num_aln_acquirers = get_num_acquirers( the_alignment_input_options_block );
	const auto num_pdb_acquirers = get_num_acquirers( the_pdb_input_options_block       );

	// If there are no objects then no options were specified so just output the standard usage error string
	if ( ( num_aln_acquirers == 0 ) && ( num_pdb_acquirers == 0 ) ) {
		return ""s;
	}

	// Check that there is exactly one source of alignment
	if ( num_aln_acquirers > 1 ) {
		return "Please specify at most one source of an alignment or superposition ("
		       + ::std::to_string( num_aln_acquirers )
		       + " specified)";
	}

	// Check that there is exactly one source of PDBs
	if ( num_pdb_acquirers != 1 ) {
		return "Please specify one source of PDBs ("
		       + ::std::to_string( num_pdb_acquirers )
		       + " specified)";
	}

	return none;
}

/// \brief Get a string to prepend to the standard help
string cath_score_align_options::do_get_help_prefix_string() const {
	return "Usage: " + PROGRAM_NAME + " alignment_source protein_file_source [superposition_outputs]\n\n"
		+ get_overview_string() + R"(

Please specify:
 * at most one alignment (default: --)" + alignment_input_options_block::PO_DO_THE_SSAPS + R"()
 * one method of reading proteins (number of proteins currently restricted to 2)";
}

/// \brief Get a string to append to the standard help (just empty here)
string cath_score_align_options::do_get_help_suffix_string() const {
	return "";
}

/// \brief Get an overview of the job that these options are for
///
/// This can be used in the --help and --version outputs
string cath_score_align_options::do_get_overview_string() const {
	return "Score an existing alignment using structural data";
}

/// \brief TODOCUMENT
void cath_score_align_options::check_ok_to_use() const {
	if ( get_error_or_help_string() ) {
		BOOST_THROW_EXCEPTION(invalid_argument_exception("Attempt to use invalid cath_score_align_options"));
	}
}

/// \brief TODOCUMENT
cath_score_align_options::cath_score_align_options() {
	super::add_options_block( the_alignment_input_options_block );
	super::add_options_block( the_pdb_input_options_block       );
}

/// \brief Getter for the pdb_input_spec
const pdb_input_spec & cath_score_align_options::get_pdb_input_spec() const {
	return the_pdb_input_options_block.get_pdb_input_spec();
}

/// \brief Getter for the alignment_input_spec
const alignment_input_spec & cath_score_align_options::get_alignment_input_spec() const {
	return the_alignment_input_options_block.get_alignment_input_spec();
}

/// \brief Get the single alignment_acquirer implied by the specified cath_score_align_options
///        (or throw an invalid_argument_exception if fewer/more are implied)
///
/// \relates cath_score_align_options
unique_ptr<const alignment_acquirer> cath::opts::get_alignment_acquirer(const cath_score_align_options &prm_cath_score_align_options ///< The cath_score_align_options to query
                                                                        ) {
	return align::get_alignment_acquirer( prm_cath_score_align_options.get_alignment_input_spec() );
}

/// \brief Get the single pdbs_acquirer implied by the specified cath_score_align_options
///        (or throw an invalid_argument_exception if fewer/more are implied)
///
/// \relates cath_score_align_options
unique_ptr<const pdbs_acquirer> cath::opts::get_pdbs_acquirer(const cath_score_align_options &prm_cath_score_align_options ///< The cath_score_align_options to query
                                                              ) {
	return get_pdbs_acquirer( prm_cath_score_align_options.get_pdb_input_spec() );
}

/// \brief Get PDBs and names as implied by the specified cath_score_align_options
///
/// Throws an invalid_argument_exception if the cath_score_align_options isn't configured to read PDBs
///
/// \relates cath_score_align_options
strucs_context cath::opts::get_pdbs_and_names(const cath_score_align_options &prm_cath_score_align_options, ///< The options to specify how to get the PDBs and names
                                              istream                        &prm_istream,                  ///< The istream, which may contain PDB data
                                              const bool                     &prm_remove_partial_residues   ///< Whether to remove partial residues from the PDB data
                                              ) {
	return get_strucs_context(
		*get_pdbs_acquirer( prm_cath_score_align_options ),
		prm_istream,
		prm_remove_partial_residues
	);
}
