#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "main.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "GlobalNamespace/VRRig.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "GorillaLocomotion/Player.hpp"
#include "config.hpp"
#include "LongArmMonkeWatchView.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"
#include "monkecomputer/shared/Register.hpp"
#include "custom-types/shared/register.hpp"
#include "UnityEngine/Vector3.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "GlobalNamespace/VRRig.hpp"
ModInfo modInfo;

#define INFO(value...) getLogger().info(value)
#define ERROR(value...) getLogger().error(value)

using namespace UnityEngine;
using namespace GorillaLocomotion;
using namespace GlobalNamespace;

bool moddedRoom = false;
bool gotoffset = false;
bool once = false;
bool once2 = false;
Vector3 Loffset;
Vector3 Roffset;

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(Player_Update, "GorillaLocomotion", "Player", "Update", void, Il2CppObject* self) {
    Player_Update(self);
    GorillaLocomotion::Player* playerInstance = GorillaLocomotion::Player::get_Instance();
    if(playerInstance == nullptr) return;
    GameObject* go = playerInstance->get_gameObject();
    auto* player1 = go->GetComponent<GorillaLocomotion::Player*>();
    Il2CppObject* currentRoom = CRASH_UNLESS(il2cpp_utils::RunMethod("Photon.Pun", "PhotonNetwork", "get_CurrentRoom"));
    if (!moddedRoom || !currentRoom) {
        config.enabled = false;
        moddedRoom = false;
    }
    if (config.enabled){ 
        bool startInput = OVRInput::Get(OVRInput::Button::Start, OVRInput::Controller::LTouch);
        if (startInput) {
            config.enabled = false;
        }
    }
    using namespace GorillaLocomotion;
    if (!once) {
        if (config.enabled) {
            player1->leftHandOffset = Vector3(0.3, 0.0, 0.3);
            player1->rightHandOffset = Vector3(-0.3, 0.0, 0.3);
            once = true;
            once2 = false;
        }
    }
    if (!once2) {
        if (!config.enabled) {
            player1->leftHandOffset = Roffset;
            player1->rightHandOffset = Loffset;
            once = false;
            once2 = true;
        }
    }
}

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(Player_Awake, "GorillaLocomotion", "Player", "Awake", void, GorillaLocomotion::Player* self)
{
    Player_Awake(self);
    GorillaLocomotion::Player* playerInstance = GorillaLocomotion::Player::get_Instance();
    if(playerInstance == nullptr) return;
    GameObject* go = playerInstance->get_gameObject();
    auto* player1 = go->GetComponent<GorillaLocomotion::Player*>();
    if (!gotoffset) {
        Roffset = player1->leftHandOffset;
        Loffset = player1->rightHandOffset;
        gotoffset = true;
    }
}

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(PhotonNetworkController_OnJoinedRoom, "", "PhotonNetworkController", "OnJoinedRoom", void, Il2CppObject* self) {
    PhotonNetworkController_OnJoinedRoom(self);
    Il2CppObject* currentRoom = CRASH_UNLESS(il2cpp_utils::RunMethod("Photon.Pun", "PhotonNetwork", "get_CurrentRoom"));
    if (currentRoom) {
        moddedRoom = !CRASH_UNLESS(il2cpp_utils::RunMethod<bool>(currentRoom, "get_IsVisible"));
    }
    else moddedRoom = true;
}

extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
}

extern "C" void load() {
    getLogger().info("Loading mod...");
    GorillaUI::Init();
    INSTALL_HOOK(getLogger(), PhotonNetworkController_OnJoinedRoom);
	INSTALL_HOOK(getLogger(), Player_Update);
	INSTALL_HOOK(getLogger(), Player_Awake);
    custom_types::Register::AutoRegister();
    GorillaUI::Register::RegisterWatchView<LongArmMonke::LongArmMonkeWatchView*>("<b><i><color=#FF7676>Looooooong Arm Monke</color></i></b>", VERSION);
    LoadConfig();
    getLogger().info("Mod loaded!");
}