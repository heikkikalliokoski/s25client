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

#include "defines.h" // IWYU pragma: keep
#include "AddonBool.h"
#include "Loader.h"
#include "Window.h"
#include "controls/ctrlCheck.h"
#include "mygettext.h"

void AddonBool::hideGui(Window* window, unsigned int id) const
{
    Addon::hideGui(window, id);
    ctrlCheck* check = window->GetCtrl<ctrlCheck>(id + 2);
    if(check)
        check->SetVisible(false);
}

void AddonBool::createGui(Window* window, unsigned int id, unsigned short& y, bool readonly, unsigned int status) const
{
    Addon::createGui(window, id, y, readonly, status);

    ctrlCheck* check = window->GetCtrl<ctrlCheck>(id + 2);
    if(!check)
    {
        check = window->AddCheckBox(id + 2, 430, y, 220, 20,  TC_GREY, _("Use"), NormalFont, readonly );
        setGuiStatus(window, id, status);
    }

    check->SetVisible(true);
    check->Move(430, y);

    y += 30;
}

void AddonBool::setGuiStatus(Window* window, unsigned int id, unsigned int status) const
{
    ctrlCheck* check = window->GetCtrl<ctrlCheck>(id + 2);

    if(check)
        check->SetCheck( (status != 0) );
}

unsigned int AddonBool::getGuiStatus(Window* window, unsigned int id, bool& failed) const
{
    ctrlCheck* check = window->GetCtrl<ctrlCheck>(id + 2);
    if(!check)
    {
        failed = true;
        return getDefaultStatus();
    }
    failed = false;

    return (check->GetCheck() ? 1 : 0);
}

unsigned AddonBool::getNumOptions() const
{
    return 2;
}
