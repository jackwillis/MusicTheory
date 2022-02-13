#pragma once

#include <boost/hana/functional/overload.hpp>
#include <boost/rational.hpp>

#include <cmath>
#include <ostream>
#include <variant>
#include <vector>

namespace Tuning {
	typedef double Cents;
	typedef boost::rational<int> Ratio;

	class TuningInterval
	{
	private:
		const std::variant<Cents, Ratio> v;

	public:
		TuningInterval(Cents cents) : v(cents) {};
		TuningInterval(Ratio ratio) : v(ratio) {};
		TuningInterval(int numer, int denom) : v(Ratio{ numer, denom }) {};

		auto cents() const -> Cents;
		auto ratio() const -> Ratio;
		auto ratio_as_double() const -> double;

		auto transpose_octaves(int octaves) const -> TuningInterval;

		friend std::ostream& operator<<(std::ostream& os, const TuningInterval& interval);
	};

	class Tuning
	{
	private:
		const std::string name;
		const std::string description;
		const std::vector<TuningInterval> intervals;

	public:
		Tuning(std::string name,
			std::string description,
			std::vector<TuningInterval> intervals)
			: name(name), description(description), intervals(intervals) {};

		auto degree() const -> int;
		auto at(int n) const -> TuningInterval;

		void stream_scala(std::ostream& out) const;
		void stream_table(std::ostream& out) const;
	};

}