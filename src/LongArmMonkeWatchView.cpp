#include "LongArmMonkeWatchView.hpp"
#include "config.hpp"
#include "monkecomputer/shared/ViewLib/MonkeWatch.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include <sstream>
#include <iomanip>

DEFINE_TYPE(LongArmMonke, LongArmMonkeWatchView);

using namespace GorillaUI;
using namespace UnityEngine;
extern bool moddedRoom;

namespace LongArmMonke
{
    void LongArmMonkeWatchView::Awake()
    {
        settingSelector = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, false);

        settingSelector->max = 1;

    }

    void LongArmMonkeWatchView::DidActivate(bool firstActivation)
    {
        std::function<void(bool)> fun = std::bind(&LongArmMonkeWatchView::OnEnter, this, std::placeholders::_1);
        settingSelector->selectionCallback = fun;
        Redraw();
    }

    void LongArmMonkeWatchView::OnEnter(int index)
    {
        if (settingSelector->currentSelectionIndex == 0) 
        {
            config.enabled ^= 1;
        }
    }

    void LongArmMonkeWatchView::Update()
    {
        Redraw();
    }

    void LongArmMonkeWatchView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawBody();

        MonkeWatch::Redraw();
    }

    void LongArmMonkeWatchView::DrawHeader()
    {
        text += "<size=80><b><i><color=#FF7676> Looooooong Arm Monke:</color></i></b></size>";
        text += "<size=60>\n</size>";
    }

    void LongArmMonkeWatchView::DrawBody()
    {
        text += settingSelector->currentSelectionIndex == 0 ? "<size=60><color=#FF5115>></color></size>" : "<size=60> </size>";
        text += config.enabled ? "<size=60><color=#00ff00> Enabled</color></size>" : "<size=60><color=#ff0000> Disabled</color></size>"; 
        text += "<size=100>\n\n\n</size>";
        text += "<size=100>\n\n\n</size>";
        text += "<size=100>\n\n\n</size>";
        if (!moddedRoom) {
            text += "<size=50><color=#FF6D49>\nThis mod is disabled \ndue to player not \nbeing in a private room.\n</color></size>";
        }
        text += "<size=50>\nPress the Start button on your \nLeft Controller to quick disable.\n</size>"; 
    }

    void LongArmMonkeWatchView::OnKeyPressed(int value)
    {
        EKeyboardKey key = (EKeyboardKey)value;
        if (!settingSelector->HandleKey(key)) // if it was not up/down/enter
        {
            switch (settingSelector->currentSelectionIndex)
            {
                case 0:
                    break;
                default:
                    break;
            }
        }
        Redraw();
    }
}
