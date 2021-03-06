/// \file
/// \brief The display_colourer class definitions

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

#include "display_colourer.hpp"

#include "alignment/alignment_context.hpp"
#include "chopping/region/region.hpp"
#include "common/clone/check_uptr_clone_against_this.hpp"
#include "common/cpp14/make_unique.hpp"
#include "display/display_colour_spec/display_colour_spec.hpp"
#include "display/display_colourer/detail/score_colour_handler.hpp"
#include "display/display_colourer/display_colourer_alignment.hpp"
#include "display/display_colourer/display_colourer_consecutive.hpp"
#include "display/options/display_spec.hpp"
#include "display/viewer/viewer.hpp"
#include "file/pdb/pdb.hpp"
#include "file/pdb/pdb_atom.hpp"
#include "file/pdb/pdb_residue.hpp"
#include "superposition/superposition_context.hpp"

using namespace cath;
using namespace cath::align;
using namespace cath::chop;
using namespace cath::common;
using namespace cath::detail;
using namespace cath::file;
using namespace cath::sup;

using std::ostream;
using std::reference_wrapper;
using std::unique_ptr;

/// \brief Ctor from specification for post-modifying the colouring based on scores
display_colourer::display_colourer(const score_colour_handler &prm_score_colour_handler ///< Specification for post-modifying the colouring based on scores
                                   ) : the_score_colour_handler{ prm_score_colour_handler } {
}

/// \brief Standard approach to achieving a virtual copy-ctor
unique_ptr<display_colourer> display_colourer::clone() const {
	return check_uptr_clone_against_this( do_clone(), *this );
}

/// \brief Getter for the optional specification for post-modifying the colouring based on scores
const score_colour_handler_opt & display_colourer::get_score_colour_handler_opt() const {
	return the_score_colour_handler;
}

/// \brief TODOCUMENT
display_colour_spec display_colourer::get_colour_spec(const alignment_context &prm_alignment_context ///< TODOCUMENT
                                                      ) const {
	const alignment &aln         = prm_alignment_context.get_alignment();
	const auto       num_entries = aln.num_entries();
	const auto       aln_length  = aln.length();

	if ( aln_length <= 0 || num_entries <= 0 ) {
		BOOST_THROW_EXCEPTION(invalid_argument_exception("Unable to colour the alignment_context because the alignment is empty"));
	}
	if ( num_entries != get_num_entries( prm_alignment_context ) ) {
		BOOST_THROW_EXCEPTION(invalid_argument_exception("Unable to colour the alignment_context because the number of entries doesn't match the number in the structures context"));
	}
	auto &&result_spec = do_get_colour_spec( prm_alignment_context );

	return has_score_colour_handler( *this )
		? adjust_display_colour_spec_copy(
			std::forward< decltype( result_spec ) >( result_spec ),
			get_score_colour_handler( *this ),
			aln
		)
		: result_spec;
}

/// \brief Return whether the specified display_colourer has a score_colour_handler
///
/// \relates display_colourer
bool cath::has_score_colour_handler(const display_colourer &prm_display_colourer ///< The display_colourer to query
                                    ) {
	return static_cast<bool>( prm_display_colourer.get_score_colour_handler_opt() );
}

/// \brief Get the specified display_colourer's score_colour_handler
///
/// \pre `has_score_colour_handler( prm_display_colourer )` else throws an invalid_argument_exception
///
/// \relates display_colourer
const score_colour_handler & cath::get_score_colour_handler(const display_colourer &prm_display_colourer ///< The display_colourer to query
                                                            ) {
	if ( ! has_score_colour_handler( prm_display_colourer ) ) {
		BOOST_THROW_EXCEPTION(invalid_argument_exception("Cannot retrieve score_colour_handler from display_colourer which doesn't have one"));
	}
	return prm_display_colourer.get_score_colour_handler_opt().get();
}

/// \brief TODOCUMENT
///
/// \relates display_colourer
unique_ptr<const display_colourer> cath::get_display_colourer(const display_spec            &prm_display_spec,   ///< TODOCUMENT
                                                              const display_colour_gradient &prm_colour_gradient ///< TODOCUMENT
                                                              ) {
	const score_colour_handler colour_handler{
		prm_display_spec.get_show_scores_if_present(),
		prm_display_spec.get_scores_to_equivs(),
		prm_display_spec.get_normalise_scores()
	};
	if ( prm_display_spec.get_gradient_colour_alignment() ) {
		return { common::make_unique< display_colourer_alignment   >( prm_colour_gradient,                 colour_handler ) };
	}
	return { common::make_unique< display_colourer_consecutive >( get_colour_list( prm_display_spec ), colour_handler ) };
}

/// \brief TODOCUMENT
///
/// \relates display_colourer
display_colour_spec cath::get_colour_spec(const display_colourer &prm_display_colourer, ///< TODOCUMENT
                                          const strucs_context   &prm_strucs_context,   ///< TODOCUMENT
                                          const alignment        &prm_alignment         ///< TODOCUMENT
                                          ) {
	return prm_display_colourer.get_colour_spec( alignment_context{ prm_alignment, prm_strucs_context } );
}

/// \brier Helper guard to notify a viewer at the start and end of a colouring with a display_colourer
class viewer_colour_notifier_guard final {
private:
	/// \brier Const-reference to the display_colourer that will be colouring the viewer
	reference_wrapper<const display_colourer> the_colourer;

	/// \brier Reference to the stream to which the viewer data is to be written
	ostream_ref the_os;

	/// \brier Reference to the viewer to notify
	reference_wrapper<viewer> the_viewer;

public:
	/// \brief Ctor, which calls begin_colouring() on the viewer
	viewer_colour_notifier_guard(const display_colourer &prm_display_colourer, ///< The display_colourer that will be colouring the viewer
	                             ostream                &prm_os,               ///< The stream to which the viewer data is to be written
	                             viewer                 &prm_viewer            ///< The viewer to notify
	                             ) : the_colourer{ prm_display_colourer },
	                                 the_os      { prm_os               },
	                                 the_viewer  { prm_viewer           } {
		the_viewer.get().begin_colouring( the_os.get(), the_colourer.get() );
	}

	/// \brief Dtor, which calls end_colouring() on the viewer
	~viewer_colour_notifier_guard() {
		try {
			the_viewer.get().end_colouring( the_os.get(), the_colourer.get() );
		}
		catch (...) {
		}
	}
};

/// \brief Write instructions for the specified viewer to the specified ostream
///        to represent the specified display_colourer in the context of the specified alignment_context
///
/// This is the version where there *is* an alignment that can be used for the colouring.
///
/// \relates display_colourer
void cath::colour_viewer(const display_colourer  &prm_colourer, ///< The display_colourer to write to the ostream
                         ostream                 &prm_os,       ///< The ostream to which the instructions should be written
                         viewer                  &prm_viewer,   ///< The viewer defining the instructions to be written
                         const alignment_context &prm_aln_con   ///< The alignment_context providing the context for the display_colourer
                         ) {
	const viewer_colour_notifier_guard the_guard{ prm_colourer, prm_os, prm_viewer };
	colour_viewer_with_spec(
		prm_colourer.get_colour_spec( prm_aln_con ),
		prm_viewer,
		prm_aln_con,
		prm_os
	);
}

/// \brief Write instructions for the specified viewer to the specified ostream
///        to represent the specified alignment_free_display_colourer in the context of
///        the specified cleaned structure names
///
/// This is the version where there *isn't* any alignment that can be used for the colouring.
///
/// \relates display_colourer
void cath::colour_viewer(const alignment_free_display_colourer &prm_colourer,                 ///< The alignment_free_display_colourer to write to the ostream
                         ostream                               &prm_os,                       ///< The ostream to which the instructions should be written
                         viewer                                &prm_viewer,                   ///< The viewer defining the instructions to be written
                         const pdb_list                        &prm_pdbs,                     ///< The structures to be coloured
                         const str_vec                         &prm_cleaned_names_for_viewer, ///< The names of the structures (cleaned for the viewer)
                         const region_vec_opt_vec              &prm_regions                   ///< The key regions of the structures
                         ) {
	const viewer_colour_notifier_guard the_guard{ prm_colourer, prm_os, prm_viewer };
	colour_viewer_with_spec(
		prm_colourer.get_colour_spec_from_regions( prm_regions ),
		prm_viewer,
		prm_pdbs,
		prm_cleaned_names_for_viewer,
		prm_os
	);
}

///// \brief TODOCUMENT
/////
///// \relates display_colourer
//void cath::colour_alignment(const display_colourer      &prm_colourer, ///< TODOCUMENT
//                            ostream                     &prm_os,       ///< TODOCUMENT
//                            const superposition_context &prm_sup_con   ///< TODOCUMENT
//                            ) {
//	const display_colour_spec the_spec = prm_colourer.get_colour_spec( prm_sup_con );
//	colour_alignment_with_spec(
//		the_spec,
//		prm_sup_con.get_alignment(),
//		prm_sup_con.get_pdbs_cref(),
//		clean_names_for_viewer( prm_sup_con ),
//		prm_os
//	);
//}
