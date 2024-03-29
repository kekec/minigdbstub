#include <vector>
#include <iostream>
#include <string>
#include <signal.h>

#include "test_common.hpp"
#include "minigdbstub.h"
#include "gtest/gtest.h"

// --- Tests ---

TEST(minigdbstub, test_recvs) {
    // Create mock test packets
    std::vector<const char*> mockPackets = {
        "+$g#67",
        "+$G#47",
        "+$c#63",
        "+$Ga700467f#46"
    };

    for (auto packet : mockPackets) {
        // Create getchar buffer
        std::vector<char> testBuff;
        for (size_t i=0; i<strlen(packet); ++i) {
            testBuff.push_back(packet[i]);
        }
        g_getcharPktHandle = &testBuff;
        g_getcharPktIndex = 0;

        // Create putchar buffer
        std::vector<char> testBuff2;
        g_putcharPktHandle = &testBuff2;

        mgdbProcObj procObj = {0};
        gdbPacket gdbPkt;
        initDynCharBuffer(&gdbPkt.pktData, MINIGDBSTUB_PKT_SIZE);

        minigdbstubRecv(&procObj, &gdbPkt);

        EXPECT_EQ(gdbPkt.commandType, packet[2]);
        freeDynCharBuffer(&gdbPkt.pktData);
    }
}