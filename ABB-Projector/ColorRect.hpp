# pragma once
# include <Siv3D.hpp>

class ColorRect : public RectF
{
public:

	ColorF color;

	ColorRect() = default;

	ColorRect(const RectF& rect, const ColorF& color) :
		RectF(rect), color(color) {};

	void draw() const
	{
		RectF::draw(color);
	}
};

Optional<ColorRect> FromString(const String& string);
