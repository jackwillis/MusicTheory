#include "Tuning.h"

namespace Tuning {
	using boost::hana::overload;

	const int CENTS_PER_OCTAVE = 1200;

	auto cents_to_ratio_as_double(Cents cents) -> double
	{
		return std::pow(2, cents / CENTS_PER_OCTAVE);
	}

	auto ratio_to_cents(Ratio ratio) -> Cents
	{
		const double f_ratio = boost::rational_cast<double>(ratio);
		return std::log2(f_ratio) * CENTS_PER_OCTAVE;
	}

	auto TuningInterval::cents() const -> Cents
	{
		return std::visit(overload(
			[](Cents cents) { return cents; },
			[](Ratio ratio) { return ratio_to_cents(ratio); }
		), this->v);
	}

	// Farey algorithm: find closest rational to double in [0, 1]
	// http://www.johndcook.com/blog/2010/10/20/best-rational-approximation/
	//
	auto farey(double x, int max_denom) -> boost::rational<int>
	{
		int a = 0, b = 1, c = 1, d = 1;
		double mediant;
		while (b <= max_denom && d <= max_denom)
		{
			mediant = (double)(a + c) / (double)(b + d);
			if (x == mediant)
			{
				if (b + d <= max_denom) return boost::rational<int>{a + c, b + d};
				else if (d > b) return boost::rational<int>{c, d};
				else return boost::rational<int>{a, b};
			}
			else if (x > mediant) { a += c; b += d; }
			else { c += a; d += b; }
		}

		if (b > max_denom) { return boost::rational<int>{c, d}; }
		else { return boost::rational<int>{a, b}; }
	}

	auto closest_rational(double x) -> boost::rational<int>
	{
		const int integral_part = static_cast<int>(std::trunc(x));
		const double fractional_part = x - integral_part;
		const int max_denom = 200;
		const auto rational_fractional_part = farey(fractional_part, max_denom);
		return integral_part + rational_fractional_part;
	}

	auto TuningInterval::ratio() const -> Ratio
	{
		return std::visit(overload(
			[](Cents cents) { return closest_rational(cents_to_ratio_as_double(cents)); },
			[](Ratio ratio) { return ratio; }
		), this->v);
	}

	auto TuningInterval::ratio_as_double() const -> double
	{
		return std::visit(overload(
			[](Cents cents) { return cents_to_ratio_as_double(cents); },
			[](Ratio ratio) { return boost::rational_cast<double>(ratio); }
		), this->v);
	}

	// https://stackoverflow.com/a/17504209
	int powi(int base, unsigned int exp)
	{
		int res = 1;
		while (exp) {
			if (exp & 1)
				res *= base;
			exp >>= 1;
			base *= base;
		}
		return res;
	}

	auto TuningInterval::transpose_octaves(int octaves) const -> TuningInterval
	{
		return std::visit(overload(
			[&octaves](Cents cents) -> TuningInterval {
				return TuningInterval{ cents + (octaves * CENTS_PER_OCTAVE) };
			},
			[&octaves](Ratio ratio) -> TuningInterval {
				if (octaves == 0) {
					return TuningInterval{ ratio };
				}
				else if (octaves > 0) {
					// multiply ratio by 2^octaves
					return TuningInterval{
						ratio.numerator() * powi(2, octaves),
						ratio.denominator()
					};
				}
				else {
					// multiply ratio by (1/2)^-octaves
					return TuningInterval{
						ratio.numerator(),
						ratio.denominator() * powi(2, -octaves)
					};
				}
			}
			), this->v);
	}

	std::ostream& operator<<(std::ostream& os, const TuningInterval& interval)
	{
		std::visit(overload(
			[&os](Cents cents) { os << cents; },
			[&os](Ratio ratio) { os << ratio; }
		), interval.v);
		return os;
	}

	// Tuning class

	auto Tuning::degree() const -> int
	{
		return static_cast<int>(this->intervals.size());
	}

	// https://stackoverflow.com/a/11714601/3551701
	auto euclidean_remainder(int a, int b) -> int
	{
		assert(b != 0);
		int r = a % b;
		return r >= 0 ? r : r + std::abs(b);
	}

	auto Tuning::at(int n) const -> TuningInterval
	{
		if (n == 0) {
			return TuningInterval{ 1, 1 };
		}
		else {
			const int octave = n < 0 ?
				n / this->degree() - 1 :
				(n - 1) / this->degree();

			const int index = euclidean_remainder(n - 1, this->degree());
			const auto& interval = this->intervals.at(index);

			return interval.transpose_octaves(octave);
		}
	}

	void Tuning::stream_scala(std::ostream& out) const
	{
		out << "! "
			<< this->name << "\n! \n"
			<< this->description << '\n'
			<< this->degree() << "\n!\n";

		for (const auto& interval : this->intervals)
		{
			out << interval << '\n';
		}
	}

	void Tuning::stream_table(std::ostream& out) const
	{
		const auto small_pad = std::setw(6);
		const auto big_pad = std::setw(15);

		out << small_pad << "Index"
			<< big_pad << "Str"
			<< big_pad << "Cents"
			<< big_pad << "Ratio"
			<< '\n';

		for (int i = this->degree() * -1; i != this->degree() * 3; ++i)
		{
			const auto interval = this->at(i);
			out << small_pad << i
				<< big_pad << interval
				<< big_pad << interval.cents()
				<< big_pad << interval.ratio()
				<< '\n';
		}
	}
}