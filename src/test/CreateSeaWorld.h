// Copyright (c) 2016 Settlers Freaks (sf-team at siedler25.org)
//
// This file is part of Return To The Roots.
//
// Return To The Roots is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Return To The Roots is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Return To The Roots. If not, see <http://www.gnu.org/licenses/>.

#ifndef CreateSeaWorld_h__
#define CreateSeaWorld_h__

#include "gameTypes/Nation.h"
#include <vector>

class GameWorldGame;

/// Creates a world for up to 4 players,
/// with a sea on the outside and a lake on the inside with each player having access to both
struct CreateSeaWorld
{
    CreateSeaWorld(unsigned width, unsigned height, unsigned numPlayers);
    bool operator()(GameWorldGame& world) const;
private:
    unsigned width_, height_;
    std::vector<Nation> playerNations_;
};

/// World for 2 players with all water except a small patch of land with player 0 HQ in the middle and player 1 HQ in the bottom left
struct CreateWaterWorld
{
    CreateWaterWorld(unsigned width, unsigned height, unsigned numPlayers);
    bool operator()(GameWorldGame& world) const;
private:
    unsigned width_, height_;
    std::vector<Nation> playerNations_;
};

#endif // CreateSeaWorld_h__
