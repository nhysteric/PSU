#pragma once

#include <libOTe/Tools/Coproto.h>
#include <string>
#include <vector>
#include "benes.h"
#include "cryptoTools/Common/BitVector.h"
#include "cryptoTools/Common/Timer.h"
class OSNSender {
    size_t size;
    int ot_type;
    oc::Timer *timer;

    Benes benes;
    void silent_ot_recv(
        osuCrypto::BitVector &choices,
        std::vector<osuCrypto::block> &recvMsg,
        std::vector<std::array<oc::cp::Socket, 2>> &chls);
    void rand_ot_recv(
        osuCrypto::BitVector &choices,
        std::vector<osuCrypto::block> &recvMsg,
        std::vector<std::array<oc::cp::Socket, 2>> &chls);
    std::vector<std::array<osuCrypto::block, 2>> gen_benes_server_osn(
        int values, std::vector<std::array<oc::cp::Socket, 2>> &chls);

public:
    std::vector<int> dest;
    OSNSender(size_t size = 0, int ot_type = 0);
    void init(size_t size, int ot_type = 0, const std::string &osn_cache = "", bool cache = false);
    std::vector<oc::block> run_osn(std::vector<std::array<oc::cp::Socket, 2>> &chls);
    void setTimer(oc::Timer &timer);
};
