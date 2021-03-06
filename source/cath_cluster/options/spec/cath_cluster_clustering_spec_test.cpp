/// \file
/// \brief The cath_cluster_clustering_spec test suite

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

#include <boost/optional/optional_io.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "cath_cluster/options/spec/cath_cluster_clustering_spec.hpp"

#include <string>

namespace cath { namespace test { } }

using namespace ::cath::clust;
using namespace ::cath::test;
using namespace ::std::literals::string_literals;

using ::boost::none;
using ::boost::test_tools::per_element;

namespace cath {
	namespace test {

		/// \brief The cath_cluster_clustering_spec_test_suite_fixture to assist in testing cath_cluster_clustering_spec
		struct cath_cluster_clustering_spec_test_suite_fixture {
		protected:
			~cath_cluster_clustering_spec_test_suite_fixture() noexcept = default;

			/// \brief link_dirn::STRENGTH levels that are in the correct order so they should induce a warning
			const strength_vec jumbled_strength_levels         {   35,   95,   60,  100 };

			/// \brief Valid link_dirn::STRENGTH levels
			const strength_vec valid_strength_levels           {   35,   60,   95,  100 };

			/// \brief The expected result of sorting the valid link_dirn::STRENGTH levels
			const strength_vec expected_strength_levels_result { -100,  -95,  -60,  -35 };


			/// \brief link_dirn::DISSIMILARITY levels that are in the correct order so they should induce a warning
			const strength_vec jumbled_distance_levels         {    4,    2,    3,    1 };

			/// \brief Valid link_dirn::DISSIMILARITY levels
			const strength_vec valid_distance_levels           {    4,    3,    2,    1 };

			/// \brief The expected result of sorting the valid link_dirn::DISSIMILARITY levels
			const strength_vec expected_distance_levels_result {    1,    2,    3,    4 };
		};

	}
} // namespace cath

BOOST_FIXTURE_TEST_SUITE(cath_cluster_clustering_spec_test_suite, cath_cluster_clustering_spec_test_suite_fixture)

BOOST_AUTO_TEST_CASE(levels_processing_is_correct) {
	BOOST_TEST( get_dissim_sort_warning  ( jumbled_strength_levels, link_dirn::STRENGTH      ) == "The levels (35, 95, 60, 100) are not sorted to be increasing as would be expected with a strength link direction"s );
	BOOST_TEST( get_dissim_sort_warning  ( valid_strength_levels,   link_dirn::STRENGTH      ) == none                                           );
	BOOST_TEST( make_dissim_and_sort_copy( valid_strength_levels,   link_dirn::STRENGTH      ) == expected_strength_levels_result, per_element{} );
	BOOST_TEST( get_max_dissim           ( valid_strength_levels,   link_dirn::STRENGTH      ) == -35                                            );

	BOOST_TEST( get_dissim_sort_warning  ( jumbled_distance_levels, link_dirn::DISSIMILARITY ) == "The levels (4, 2, 3, 1) are not sorted to be decreasing as would be expected with a distance link direction"s );
	BOOST_TEST( get_dissim_sort_warning  ( valid_distance_levels,   link_dirn::DISSIMILARITY ) == none                                           );
	BOOST_TEST( make_dissim_and_sort_copy( valid_distance_levels,   link_dirn::DISSIMILARITY ) == expected_distance_levels_result, per_element{} );
	BOOST_TEST( get_max_dissim           ( valid_distance_levels,   link_dirn::DISSIMILARITY ) == 4                                              );
}

BOOST_AUTO_TEST_SUITE_END()
