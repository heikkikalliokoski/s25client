// Copyright (c) 2005 - 2015 Settlers Freaks (sf-team at siedler25.org)
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
#include "dskSelectMap.h"

#include "WindowManager.h"
#include "Loader.h"
#include "GameServer.h"
#include "GameClient.h"
#include "ListDir.h"
#include "files.h"
#include "fileFuncs.h"
#include "LobbyClient.h"
#include "controls/ctrlOptionGroup.h"
#include "controls/ctrlPreviewMinimap.h"
#include "controls/ctrlTable.h"
#include "controls/ctrlText.h"
#include "desktops/dskDirectIP.h"
#include "desktops/dskHostGame.h"
#include "desktops/dskLobby.h"
#include "desktops/dskSinglePlayer.h"
#include "desktops/dskLAN.h"
#include "mapGenerator/MapGenerator.h"

#include "ingameWindows/iwMsgbox.h"
#include "ingameWindows/iwSave.h"
#include "ingameWindows/iwDirectIPCreate.h"
#include "ingameWindows/iwPleaseWait.h"
#include "ingameWindows/iwMapGenerator.h"

#include "ogl/glArchivItem_Font.h"
#include "ogl/glArchivItem_Map.h"
#include "libsiedler2/src/ArchivItem_Map_Header.h"
#include "libsiedler2/src/prototypen.h"
#include "libutil/src/ucString.h"
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

/** @class dskSelectMap
 *
 *  Klasse des Map-Auswahl Desktops.
 */

/** @var dskSelectMap::type
 *
 *  Typ des Servers.
 */

/** @var dskSelectMap::name
 *
 *  Name des Servers.
 */

/** @var dskSelectMap::pass
 *
 *  Passwort des Servers.
 */

/**
 *  Konstruktor von @p dskSelectMap.
 *
 *  @param[in] type Typ des Servers
 *  @param[in] name Server-Name
 *  @param[in] pass Server-Passwort
 */
dskSelectMap::dskSelectMap(const CreateServerInfo& csi)
    : Desktop(LOADER.GetImageN("setup015", 0)),
      csi(csi)
{
    // Die Tabelle für die Maps
    AddTable( 1, 110,  35, 680, 400, TC_GREY, NormalFont, 6, _("Name"), 250, ctrlTable::SRT_STRING, _("Author"), 216, ctrlTable::SRT_STRING, _("Player"), 170, ctrlTable::SRT_NUMBER, _("Type"), 180, ctrlTable::SRT_STRING, _("Size"), 134, ctrlTable::SRT_MAPSIZE, "", 0, ctrlTable::SRT_STRING);

    // "Karten Auswahl"
    AddText(  2, 400,   5, _("Selection of maps"), COLOR_YELLOW, glArchivItem_Font::DF_CENTER, LargeFont);

    // "Zurück"
    AddTextButton(3, 380, 560, 200, 22, TC_RED1, _("Back"), NormalFont);
    // "Spiel laden..."
    AddTextButton(4, 590, 530, 200, 22, TC_GREEN2, _("Load game..."), NormalFont);
    // "Weiter"
    AddTextButton(5, 590, 560, 200, 22, TC_GREEN2, _("Continue"), NormalFont);
    // random map generation
    AddTextButton(6, 380, 530, 150, 22, TC_GREEN2, _("Random Map"), NormalFont);
    // random map settings
    AddTextButton(7, 540, 530, 40, 22, TC_GREEN2, _("..."), NormalFont);

    ctrlOptionGroup* optiongroup = AddOptionGroup(10, ctrlOptionGroup::CHECK, scale_);
    // "Alte"
    optiongroup->AddTextButton(0, 10,  35, 90,  22, TC_GREY, _("Old maps"), NormalFont);
    // "Neue"
    optiongroup->AddTextButton(1, 10,  60, 90,  22, TC_GREY, _("New maps"), NormalFont);
    // "Eigene"
    optiongroup->AddTextButton(2, 10,  85, 90,  22, TC_GREY, _("Own maps"), NormalFont);
    // "Kontinente"
    optiongroup->AddTextButton(3, 10, 110, 90,  22, TC_GREY, _("Continents"), NormalFont);
    // "Kampagne"
    optiongroup->AddTextButton(4, 10, 135, 90,  22, TC_GREY, _("Campaign"), NormalFont);
    // "RTTR"
    optiongroup->AddTextButton(5, 10, 160, 90,  22, TC_GREY, _("RTTR"), NormalFont);
    // "Andere"
    optiongroup->AddTextButton(6, 10, 185, 90,  22, TC_GREY, _("Other"), NormalFont);
    // "Andere"
    optiongroup->AddTextButton(7, 10, 210, 90,  22, TC_GREY, _("Sea"), NormalFont);
    // "Heruntergeladene"
    optiongroup->AddTextButton(8, 10, 235, 90,  22, TC_GREY, _("Played"), NormalFont);

    AddPreviewMinimap(11, 110, 445, 140, 140, NULL);
    AddText(12, 260, 470, _("Map: "), COLOR_YELLOW, glArchivItem_Font::DF_LEFT, NormalFont);
    AddText(13, 260, 490, _("Mapfile: "), COLOR_YELLOW, glArchivItem_Font::DF_LEFT, NormalFont);

    // "Eigene" auswählen
    optiongroup->SetSelection(5, true);

    LOBBYCLIENT.SetInterface(this);
    GAMECLIENT.SetInterface(this);
}

dskSelectMap::~dskSelectMap()
{
}

void dskSelectMap::Msg_OptionGroupChange(const unsigned int  /*ctrl_id*/, const int selection)
{
    ctrlTable* table = GetCtrl<ctrlTable>(1);

    // Tabelle leeren
    table->DeleteAllItems();

    // Old, New, Own, Continents, Campaign, RTTR, Other, Sea, Played
    static const boost::array<unsigned, 9> ids = {{ 39, 40, 41, 42, 43, 52, 91, 93, 48 }};

    // Und wieder füllen lassen
    FillTable(ListDir(GetFilePath(FILE_PATHS[ids[selection]]), "swd"));
    FillTable(ListDir(GetFilePath(FILE_PATHS[ids[selection]]), "wld"));

    // Dann noch sortieren
    bool sortAsc = true;
    table->SortRows(0, &sortAsc);

    // und Auswahl zurücksetzen
    table->SetSelection(0);
}

/**
 *  Occurs when user changes the selection in the table of maps.
 */
void dskSelectMap::Msg_TableSelectItem(const unsigned int ctrl_id, const int selection)
{
    switch(ctrl_id)
    {
        case 1:
        {
            ctrlTable* table = GetCtrl<ctrlTable>(1);

            // is the selection valid?
            if(selection < table->GetRowCount())
            {
                // get path to map from table
                std::string path = table->GetItemText(selection, 5);

                libsiedler2::ArchivInfo ai;
                // load map data
                if(libsiedler2::loader::LoadMAP(path, ai) == 0)
                {
                    glArchivItem_Map* map = dynamic_cast<glArchivItem_Map*>(ai.get(0));
                    if(map)
                    {
                        ctrlPreviewMinimap* preview = GetCtrl<ctrlPreviewMinimap>(11);
                        preview->SetMap(map);

                        ctrlText* text = GetCtrl<ctrlText>(12);
                        text->SetText(cvStringToUTF8(map->getHeader().getName()) );
                        text->Move(preview->GetX(true) + preview->GetWidth() + 10, text->GetY(true), true);

                        text = GetCtrl<ctrlText>(13);
                        text->SetText(path);
                        text->Move(preview->GetX(true) + preview->GetWidth() + 10, text->GetY(true), true);
                    }
                }
            }
        } break;
    }
}

void dskSelectMap::GoBack()
{
    if (csi.type == ServerType::LOCAL)
        WINDOWMANAGER.Switch(new dskSinglePlayer);
    else if (csi.type == ServerType::LAN)
        WINDOWMANAGER.Switch(new dskLAN);
    else if (csi.type == ServerType::LOBBY && LOBBYCLIENT.IsLoggedIn())
        WINDOWMANAGER.Switch(new dskLobby);
    else
        WINDOWMANAGER.Switch(new dskDirectIP);
}

void dskSelectMap::Msg_ButtonClick(const unsigned int ctrl_id)
{
    switch(ctrl_id)
    {
        case 3: // "Zurück"
        {
            GoBack();
        } break;
        case 4: // "Spiel laden..."
        {
            // Ladefenster aufrufen
            WINDOWMANAGER.Show(new iwLoad(csi));
        } break;
        case 5: // "Weiter"
        {
            StartServer();
        } break;
        case 6: // random map
        {
            CreateRandomMap();
        } break;
        case 7: // random map generator settings
        {
            WINDOWMANAGER.Show(new iwMapGenerator(rndMapSettings));
        } break;
    }
}

void dskSelectMap::Msg_TableChooseItem(const unsigned ctrl_id, const unsigned selection)
{
    // Doppelklick auf bestimmte Map -> weiter
    StartServer();
}

void dskSelectMap::CreateRandomMap()
{
    // setup filepath for the random map
    std::string mapPath = GetFilePath(FILE_PATHS[48]);
    mapPath.append("Random.swd");

    // create a random map and save filepath
    MapGenerator generator;
    generator.Create(mapPath, rndMapSettings);
    
    // select the "played maps" entry
    ctrlOptionGroup* optionGroup = GetCtrl<ctrlOptionGroup>(10);
    optionGroup->SetSelection(8, true);
    
    // search for the random map entry and select it in the table
    ctrlTable* table = GetCtrl<ctrlTable>(1);
    for (int i = 0; i < table->GetRowCount(); i++)
    {
        std::string entryPath = table->GetItemText(i, 5);
        
        if (entryPath == mapPath)
        {
            table->SetSelection(i);
            break;
        }
    }
}

/// Startet das Spiel mit einer bestimmten Auswahl in der Tabelle
void dskSelectMap::StartServer()
{
    ctrlTable* table = GetCtrl<ctrlTable>(1);
    unsigned short selection = table->GetSelection();

    // Ist die Auswahl gültig?
    if(selection < table->GetRowCount())
    {
        // Kartenpfad aus Tabelle holen
        std::string mapPath = table->GetItemText(selection, 5);

        // Server starten
        if(!GAMESERVER.TryToStart(csi, mapPath, MAPTYPE_OLDMAP))
        {
            GoBack();
        }
        else
        {
            // Verbindungsfenster anzeigen
            WINDOWMANAGER.Show(new iwPleaseWait);
        }
    }
}

void dskSelectMap::Msg_MsgBoxResult(const unsigned msgbox_id, const MsgboxResult  /*mbr*/)
{
    if(msgbox_id == 0) // Verbindung zu Server verloren?
    {
        GAMECLIENT.Stop();

        if(csi.type == ServerType::LOBBY &&  LOBBYCLIENT.IsLoggedIn()) // steht die Lobbyverbindung noch?
            WINDOWMANAGER.Switch(new dskLobby);
        else if(csi.type == ServerType::LOBBY)
            WINDOWMANAGER.Switch(new dskDirectIP);
        else if(csi.type == ServerType::LAN)
            WINDOWMANAGER.Switch(new dskLAN);
        else
            WINDOWMANAGER.Switch(new dskSinglePlayer);
    }
}

void dskSelectMap::CI_NextConnectState(const ConnectState cs)
{
    switch(cs)
    {
        case CS_FINISHED:
        {
            WINDOWMANAGER.Switch(new dskHostGame(csi.type));
        } break;
        default:
            break;
    }

}

void dskSelectMap::CI_Error(const ClientError ce)
{
    switch(ce)
    {
        case CE_INCOMPLETEMESSAGE:
        case CE_CONNECTIONLOST:
        case CE_WRONGMAP:
        {
            // Error messages, CE_* values cannot be gotten here but are added to avoid memory access errors
            const boost::array<std::string, 8> errors =
            {{
                _("Incomplete message was received!"),
                "CE_SERVERFULL",
                "CE_WRONGPW",
                _("Lost connection to server!"),
                "CE_INVALIDSERVERTYPE",
                _("Map transmission was corrupt!"),
                "CE_WRONGVERSION",
                "CE_LOBBYFULL"
            }};

            WINDOWMANAGER.Show(new iwMsgbox(_("Error"), errors[ce], this, MSB_OK, MSB_EXCLAMATIONRED, 0));
        } break;
        default: break;
    }
}

/**
 *  (Lobby-)Status: Benutzerdefinierter Fehler (kann auch Conn-Loss o.ä sein)
 */
void dskSelectMap::LC_Status_Error(const std::string& error)
{
    WINDOWMANAGER.Show(new iwMsgbox(_("Error"), error, this, MSB_OK, MSB_EXCLAMATIONRED, 0));
}

/**
 *  (Lobby-)Server wurde erstellt.
 */
void dskSelectMap::LC_Created()
{
    // ggf. im nächstes Stadium weiter
    GAMESERVER.Start();
}

void dskSelectMap::FillTable(const std::vector<std::string>& files)
{
    ctrlTable* table = GetCtrl<ctrlTable>(1);

    BOOST_FOREACH(const std::string& filePath, files)
    {
        // Karteninformationen laden
        libsiedler2::ArchivInfo map;
        if(libsiedler2::loader::LoadMAP(filePath, map, true) != 0)
            continue;

        const libsiedler2::ArchivItem_Map_Header* header = &(dynamic_cast<const glArchivItem_Map*>(map.get(0))->getHeader());
        RTTR_Assert(header);

        if(header->getPlayer() > MAX_PLAYERS)
            continue;

        const bfs::path luaFilepath = bfs::path(filePath).replace_extension("lua");
        const bool hasLua = bfs::is_regular_file(luaFilepath);

        // Und Zeilen vorbereiten
        char players[64], size[32];
        snprintf(players, 64, _("%d Player"), header->getPlayer());
        snprintf(size, 32, "%dx%d", header->getWidth(), header->getHeight());

        // und einfügen
        const std::string landscapes[3] =
        {
            _("Greenland"),
            _("Wasteland"),
            _("Winter world")
        };

        std::string name = cvStringToUTF8(header->getName());
        if(hasLua)
            name += " (*)";
        std::string author = cvStringToUTF8(header->getAuthor());

        table->AddRow(0, name.c_str(), author.c_str(), players, landscapes[header->getGfxSet()].c_str(), size, filePath.c_str());
    }
}

