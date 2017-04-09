#pragma once
#include <SFML/Graphics/Color.hpp>
#include <cmath>

class VColour
{
public:
	static const sf::Color HSVtoRGB(float h, float s, float v, float a = 1.0f)
	{
		float hh, p, q, t, ff;
		float r, g, b;
		int i;

		if (s <= 0)
		{
			return sf::Color(
				static_cast<unsigned int>(v * 255),
				static_cast<unsigned int>(v * 255),
				static_cast<unsigned int>(v * 255),
				static_cast<unsigned int>(a * 255));
		}

		hh = h;
		hh = hh >= 360 ? 0 : hh;
		hh /= 60.0f;
		i = static_cast<int>(hh);
		ff = hh - i;
		p = v * (1 - s);
		q = v * (1 - (s * ff));
		t = v * (1 - (s * (1 - ff)));

		switch (i)
		{
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		case 5:
			r = v;
			g = p;
			b = q;
			break;
		}

		return sf::Color(
			static_cast<unsigned int>(r * 255),
			static_cast<unsigned int>(g * 255),
			static_cast<unsigned int>(b * 255),
			static_cast<unsigned int>(a * 255));
	}
	static const sf::Color LABtoRGB(float l, float a, float b, float alpha = 1.0f)
	{
		float x, y, z;
		float R, G, B;

		y = l * (1.0f / 116.0f) + 16.0f / 116.0f;
		x = a * (1.0f / 500.0f) + y;
		z = b * (-1.0f / 200.0f) + y;

		x = x > 6.0f / 29.0f ? x * x * x : x * (108.0f / 841.0f) - 432.0f / 24389.0f;
		y = l > 8.0f ? y * y * y : l * (27.0f / 24389.0f);
		z = z > 6.0f / 29.0f ? z * z * z : z * (108.0f / 841.0f) - 432.0f / 24389.0f;

		R = x * (1219569.0f / 395920.0f) + y * (-608687.0f / 395920.0f) + z * (-107481.0f / 197960.0f);
		G = x * (-80960619.0f / 87888100.0f) + y * (82435961.0f / 43944050.0f) + z * (3976797.0f / 87888100.0f);
		B = x * (93813.0f / 1774030.0f) + y * (-180961.0f / 887015.0f) + z * (107481.0f / 93370.0f);

		R = R > 0.0031308f ? pow(R, 1.0f / 2.4f) * 1.055f - 0.055f : R * 12.92f;
		G = G > 0.0031308f ? pow(G, 1.0f / 2.4f) * 1.055f - 0.055f : G * 12.92f;
		B = B > 0.0031308f ? pow(B, 1.0f / 2.4f) * 1.055f - 0.055f : B * 12.92f;

		return sf::Color(
			static_cast<unsigned int>(R * 255),
			static_cast<unsigned int>(G * 255),
			static_cast<unsigned int>(B * 255),
			static_cast<unsigned int>(alpha * 255));
	}
	static const sf::Color CMYKtoRGB(float c, float m, float y, float k, float a = 1.0f)
	{
		float r, g, b;

		r = (1 - c) * (1 - k);
		g = (1 - m) * (1 - k);
		b = (1 - y) * (1 - k);

		return sf::Color(
			static_cast<unsigned int>(r * 255),
			static_cast<unsigned int>(g * 255),
			static_cast<unsigned int>(b * 255),
			static_cast<unsigned int>(a * 255));
	}
};