# include "ColorRect.hpp"

Optional<ColorRect> FromString(const String& string)
{
	const auto args = string.split(L',');

	if (args.size() < 6)
	{
		return none;
	}

	const auto size = ParseOpt<double>(args[2]);
	const auto x = ParseOpt<double>(args[0]);
	const auto y = ParseOpt<double>(args[1]);
	const auto r = ParseOpt<double>(args[3]);
	const auto g = ParseOpt<double>(args[4]);
	const auto b = ParseOpt<double>(args[5]);

	if (!size || !x || !y || !r || !g || !b)
	{
		return none;
	}

	return ColorRect(
		RectF(size.value()).setCenter(x.value(), y.value()),
		ColorF(r.value(), g.value(), b.value())
	);
}
