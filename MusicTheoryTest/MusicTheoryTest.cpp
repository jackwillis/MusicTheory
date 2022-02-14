#define BOOST_TEST_MODULE MusicTheory::Tuning
#include <boost/test/included/unit_test.hpp>

#include "../MusicTheory/Tuning.h" // project being tested

#include <sstream>

using MusicTheory::Tuning;
using MusicTheory::TuningInterval;

BOOST_AUTO_TEST_CASE(output_tuning_as_scala)
{
    const auto tuning = Tuning{
      "bremmer_ebvt3.scl",
      "Bill Bremmer EBVT III temperament (2011)",
      std::vector<TuningInterval>{
          TuningInterval{94.87252},
          TuningInterval{197.05899},
          TuningInterval{297.8},
          TuningInterval{395.79561},
          TuningInterval{4, 3},
          TuningInterval{595.89736},
          TuningInterval{699.31190},
          TuningInterval{796.82704},
          TuningInterval{896.20299},
          TuningInterval{999.1},
          TuningInterval{1096.17389},
          TuningInterval{2, 1}
        }
    };

	const auto expected_output = R"EOF(! bremmer_ebvt3.scl
!
Bill Bremmer EBVT III temperament (2011)
12
!
94.8725
197.059
297.8
395.796
4/3
595.897
699.312
796.827
896.203
999.1
1096.17
2/1
)EOF";

    std::ostringstream output_stream;
    tuning.stream_scala(output_stream);
    BOOST_TEST(expected_output == output_stream.str(),
        boost::test_tools::per_element());
}