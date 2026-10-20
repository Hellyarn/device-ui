#pragma once
#include "lvgl.h"

namespace EkoRelayHome
{
void init();
void showMenu();
bool focusMenu();
void updateCounts(unsigned online, unsigned total, unsigned unread);
void updateStatus(bool radioEnabled, bool recentPacket, const char *clock);
} // namespace EkoRelayHome
