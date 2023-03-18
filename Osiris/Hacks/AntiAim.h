#pragma once

#include "../ConfigStructs.h"
struct Vector;
struct UserCmd;

namespace AntiAim
{
    inline float static_yaw{};

    enum moving_flag
    {
        freestanding,
        moving,
        jumping,
        ducking,
        duck_jumping,
        slow_walking,
        fake_ducking
    };

    inline const char* moving_flag_text[]
    {
        "空闲冷却中",
        "移动中",
        "跳跃中",
        "下蹲中",
        "蹲跳中",
        "慢走中",
        "假蹲中"
    };

    inline const char* peek_mode_text[]
    {
        "无",
        "Peek真身",
        "Peek假身",
        "随机",
        "转换"
    };

    inline const char* lby_mode_text[]
    {
        "正常",
        "逆向（静态）",
        "摇摆（动态）"
    };

    void rage(UserCmd* cmd, const Vector& previousViewAngles, const Vector& currentViewAngles, bool& sendPacket) noexcept;
    void legit(UserCmd* cmd, const Vector& previousViewAngles, const Vector& currentViewAngles, bool& sendPacket) noexcept;

    void run(UserCmd* cmd, const Vector& previousViewAngles, const Vector& currentViewAngles, bool& sendPacket) noexcept;
    void updateInput() noexcept;
    bool canRun(UserCmd* cmd) noexcept;

    inline int auto_direction_yaw{};

    inline moving_flag latest_moving_flag{};

    moving_flag get_moving_flag(const UserCmd* cmd) noexcept;

    float getLastShotTime();
    bool getIsShooting();
    bool getDidShoot();
    void setLastShotTime(float shotTime);
    void setIsShooting(bool shooting);
    void setDidShoot(bool shot);
}
