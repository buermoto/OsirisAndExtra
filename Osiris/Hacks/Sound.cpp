#include "../imgui/imgui.h"

#include "../Config.h"
#include "../Interfaces.h"

#include "Sound.h"

#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"
#include "../SDK/LocalPlayer.h"

#if OSIRIS_SOUND()

static struct SoundConfig {
    int chickenVolume = 100;

    struct Player {
        int masterVolume = 100;
        int headshotVolume = 100;
        int weaponVolume = 100;
        int footstepVolume = 100;
    };

    std::array<Player, 3> players;
} soundConfig;

void Sound::modulateSound(std::string_view name, int entityIndex, float& volume) noexcept
{
    auto modulateVolume = [&](int SoundConfig::Player::* proj) {
        if (const auto entity = interfaces->entityList->getEntity(entityIndex); localPlayer && entity && entity->isPlayer()) {
            if (entityIndex == localPlayer->index())
                volume *= std::invoke(proj, soundConfig.players[0]) / 100.0f;
            else if (!entity->isOtherEnemy(localPlayer.get()))
                volume *= std::invoke(proj, soundConfig.players[1]) / 100.0f;
            else
                volume *= std::invoke(proj, soundConfig.players[2]) / 100.0f;
        }
    };

    modulateVolume(&SoundConfig::Player::masterVolume);

    using namespace std::literals;

    if (name == "Player.DamageHelmetFeedback"sv)
        modulateVolume(&SoundConfig::Player::headshotVolume);
    else if (name.find("Weapon"sv) != std::string_view::npos && name.find("Single"sv) != std::string_view::npos)
        modulateVolume(&SoundConfig::Player::weaponVolume);
    else if (name.find("Step"sv) != std::string_view::npos)
        modulateVolume(&SoundConfig::Player::footstepVolume);
    else if (name.find("Chicken"sv) != std::string_view::npos)
       volume *= soundConfig.chickenVolume / 100.0f;
}

void Sound::drawGUI() noexcept
{
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnOffset(1, 300.f);
    ImGui::PushID("Sound");
    ImGui::SliderInt("鸡鸡音效", &soundConfig.chickenVolume, 0, 200, "%d%%");

    static int currentCategory{ 0 };
    ImGui::PushItemWidth(110.0f);
    ImGui::Combo("", &currentCategory, "本地玩家\0队友\0敌人\0");
    ImGui::PopItemWidth();
    ImGui::SliderInt("全局音量", &soundConfig.players[currentCategory].masterVolume, 0, 200, "%d%%");
    ImGui::SliderInt("爆头音量", &soundConfig.players[currentCategory].headshotVolume, 0, 200, "%d%%");
    ImGui::SliderInt("武器音量", &soundConfig.players[currentCategory].weaponVolume, 0, 200, "%d%%");
    ImGui::SliderInt("脚步音量", &soundConfig.players[currentCategory].footstepVolume, 0, 200, "%d%%");
    ImGui::PopID();
    ImGui::NextColumn();
    ImGui::Columns(1);
}

void Sound::resetConfig() noexcept
{
    soundConfig = {};
}

static void to_json(json& j, const SoundConfig::Player& o)
{
    const SoundConfig::Player dummy;

    WRITE("Master volume", masterVolume);
    WRITE("Headshot volume", headshotVolume);
    WRITE("Weapon volume", weaponVolume);
    WRITE("Footstep volume", footstepVolume);
}

json Sound::toJson() noexcept
{
    const SoundConfig dummy;

    json j;
    to_json(j["Chicken volume"], soundConfig.chickenVolume, dummy.chickenVolume);
    j["Players"] = soundConfig.players;
    return j;
}

static void from_json(const json& j, SoundConfig::Player& p)
{
    read(j, "Master volume", p.masterVolume);
    read(j, "Headshot volume", p.headshotVolume);
    read(j, "Weapon volume", p.weaponVolume);
    read(j, "Footstep volume", p.footstepVolume);
}

void Sound::fromJson(const json& j) noexcept
{
    read(j, "Chicken volume", soundConfig.chickenVolume);
    read(j, "Players", soundConfig.players);
}

#else
void Sound::modulateSound(std::string_view name, int entityIndex, float& volume) noexcept {}

// GUI
void Sound::menuBarItem() noexcept {}
void Sound::tabItem() noexcept {}
void Sound::drawGUI(bool contentOnly) noexcept {}

// Config
json Sound::toJson() noexcept { return {}; }
void Sound::fromJson(const json& j) noexcept {}
void Sound::resetConfig() noexcept {}
#endif
