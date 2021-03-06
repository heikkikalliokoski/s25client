// Copyright (c) 2005 - 2016 Settlers Freaks (sf-team at siedler25.org)
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

#ifndef ResourceNote_h__
#define ResourceNote_h__

#include "notifications/notifications.h"
#include "gameTypes/MapTypes.h"
#include "gameTypes/Resource.h"

struct ResourceNote
{
    ENABLE_NOTIFICATION(ResourceNote);

    ResourceNote(unsigned player, const MapPoint& pos, Resource res, unsigned char quantity):
        player(player), pos(pos), res(res), quantity(quantity){}

    const unsigned player;
    const MapPoint pos;
    const Resource res;
    const unsigned char quantity;
};

#endif // ResourceNote_h__
