#include "Colour.h"

Colour::Colour()
	:Colour(0)
{}

Colour::Colour(unsigned int val)
	: colour(val)
{}

Colour::Colour(BYTE r, BYTE g, BYTE b)
	:Colour(r, g, b, 255)
{}

Colour::Colour(BYTE r, BYTE g, BYTE b, BYTE a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

Colour::Colour(const Colour& src)
	:Colour(src.colour)
{}

Colour& Colour::operator=(const Colour& src)
{
	colour = src.colour;
	return *this;
}

bool Colour::operator==(const Colour& rhs) const
{
	return (colour==rhs.colour);
}

bool Colour::operator!=(const Colour& rhs) const
{
	return !(*this==rhs);
}

constexpr BYTE Colour::GetR() const
{
	return rgba[0];
}

void Colour::SetR(BYTE r)
{
	rgba[0] = r;
}

constexpr BYTE Colour::GetG() const
{
	return rgba[1];
}

void Colour::SetG(BYTE g)
{
	rgba[1] = g;
}

constexpr BYTE Colour::GetB() const
{
	return rgba[2];
}

void Colour::SetB(BYTE b)
{
	rgba[2] = b;
}

constexpr BYTE Colour::GetA() const
{
	return rgba[3];
}

void Colour::SetA(BYTE a)
{
	rgba[3] = a;
}
