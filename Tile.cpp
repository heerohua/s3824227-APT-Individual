#include "Tile.h"
#include "TileCodes.h"

Tile::Tile(Colour colour, Shape shape) : colour(colour), shape(shape) {}

Colour Tile::getColour() const {
    return colour;
}

Shape Tile::getShape() const {
    return shape;
}

// Overload the << operator for the Tile class
std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    os << tile.getColour() << tile.getShape();
    return os;
}

std::string Tile::print() {
    return colour + std::to_string(shape);
}

// This function checks if two Tile objects are equal.
bool Tile::operator==(const Tile& other) const {

    return colour == other.getColour() && shape == other.getShape();
}

// This function checks if two Tile objects are not equal.
bool Tile::operator!=(const Tile& other) const {

    return !(*this == other);
}

// This function checks if the tile has a match by color or shape, is the same or doesn't match at all
std::string Tile::matchType(const Tile& other)
{
    if (colour == other.getColour() && shape == other.getShape()) {
        return "same";
    }
    if (colour == other.getColour()) {
        return "color";
    }
    if (shape == other.getShape()) {
        return "shape";
    }
    return "no-match";
}

// Returns the appropriate concatenated colour output for a tile being converted to colour
std::string Tile::tileStringToColour() const {
    return tileStringColourOptions() + getColour() + std::to_string(getShape()) + RESET_COLOUR;
}

// Determines the colour a tile should be using the TileCodes colour definitions
std::string Tile::tileStringColourOptions() const {
    switch (colour) {
        case RED: return RED_COLOUR;
        case ORANGE: return ORANGE_COLOUR;
        case YELLOW: return YELLOW_COLOUR;
        case GREEN: return GREEN_COLOUR;
        case BLUE: return BLUE_COLOUR;
        case PURPLE: return PURPLE_COLOUR;
        default: return RESET_COLOUR;
    }
}

