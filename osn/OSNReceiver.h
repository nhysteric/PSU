#pragma once

#include <atomic>
#include <libOTe/Tools/Coproto.h>
#include <vector>
#include "cryptoTools/Common/Timer.h"

class OSNReceiver {
    size_t size;
    int ot_type;
    oc::Timer *timer;
    std::atomic<int> cpus;

    void rand_ot_send(
        std::vector<std::array<osuCrypto::block, 2>> &sendMsg,
        std::vector<std::array<oc::cp::Socket, 2>> &chls);
    void silent_ot_send(
        std::vector<std::array<osuCrypto::block, 2>> &sendMsg,
        std::vector<std::array<oc::cp::Socket, 2>> &chls);

    std::vector<std::vector<oc::block>> gen_benes_client_osn(
        int values, std::vector<std::array<oc::cp::Socket, 2>> &chls);
    void prepare_correction(
        int n,
        int Val,
        int lvl_p,
        int perm_idx,
        std::vector<oc::block> &src,
        std::vector<std::array<std::array<osuCrypto::block, 2>, 2>> &ot_output,
        std::vector<std::array<osuCrypto::block, 2>> &correction_blocks);

public:
    OSNReceiver(size_t size = 0, int ot_type = 0);
    void init(size_t size, int ot_type = 0);
    std::vector<oc::block> run_osn(
        std::vector<oc::block> inputs, std::vector<std::array<oc::cp::Socket, 2>> &chls);
    void setTimer(oc::Timer &timer);
};
