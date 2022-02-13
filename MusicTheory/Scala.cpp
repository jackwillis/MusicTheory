//  Scala.cpp : Scala scale file parser
//

#include "Tuning.h"
#include <iostream>

int main()
{
    using MusicTheory::Tuning;
    using MusicTheory::TuningInterval;

    const Tuning bremmer_ebvt3 = Tuning{
      "bremmer_ebvt3.scl",
      "Bill Bremmer EBVT III temperament (2011)",
      std::vector<TuningInterval>{
          TuningInterval{94.87252},
          TuningInterval{197.05899},
          TuningInterval{297.80000},
          TuningInterval{395.79561},
          TuningInterval{4, 3},
          TuningInterval{595.89736},
          TuningInterval{699.31190},
          TuningInterval{796.82704},
          TuningInterval{896.20299},
          TuningInterval{999.10000},
          TuningInterval{1096.17389},
          TuningInterval{2, 1}
        }
    };

    bremmer_ebvt3.stream_scala(std::cout);
    std::cout << "---------------------------------------------\n";
    bremmer_ebvt3.stream_table(std::cout);
}
