#include <array>
#include <coproto/Socket/LocalAsyncSock.h>
#include <coproto/Socket/Socket.h>
#include <iostream>
#include <libOTe/Tools/Coproto.h>
#include <string>
#include <thread>
#include <vector>
#include "OSNReceiver.h"
#include "OSNSender.h"
#include "coproto/Socket/AsioSocket.h"
#include "cryptoTools/Common/CLP.h"
#include "cryptoTools/Network/IOService.h"
using namespace osuCrypto;
using namespace std;
using socketPair = std::array<cp::Socket, 2>;
vector<block> sender_shares;
vector<block> receiver_shares;
vector<block> receiver_set;
vector<int> permutation;
size_t num_threads = 1;

typedef struct Address {
    string ip;
    int port;
    Address(string ip, int port) : ip(ip), port(port)
    {}
    string getAddress()
    {
        return ip + ":" + to_string(port);
    }
} address;

address operator+=(address &a, int)
{
    address tmp = a;
    tmp.port++;
    return tmp;
}

void sender(size_t size, vector<socketPair> &chls, bool cache = false)
{
    IOService ios;

    OSNSender osn;
    osn.init(size, 1, "benes", cache);
    Timer timer;
    osn.setTimer(timer);
    timer.setTimePoint("start");
    sender_shares = osn.run_osn(chls);
    /*for (size_t i = 0; i < shares.size(); i++)
    {
        cout << i << " " << shares[i] << endl;
    }*/
    timer.setTimePoint("total");
    permutation = osn.dest;
    cout << IoStream::lock;
    cout << "Sender:" << endl;
    cout << timer << endl;
    size_t sent = 0, recv = 0;
    for (auto &chl : chls) {
        for (int i = 0; i <= 1; i++) {
            sent += chl[i].bytesSent();
            recv += chl[i].bytesReceived();
        }
    }
    cout << "recv: " << recv / 1024.0 / 1024.0 << "MB sent:" << sent / 1024.0 / 1024.0 << "MB "
         << "total: " << (recv + sent) / 1024.0 / 1024.0 << "MB" << endl;
    cout << IoStream::unlock;
}

void receiver(size_t size, vector<socketPair> &chls)
{
    receiver_set.resize(size);
    for (size_t i = 0; i < receiver_set.size(); i++) {
        receiver_set[i] = toBlock(0, i);
    }
    IOService ios;
    OSNReceiver osn;
    osn.init(receiver_set.size(), 1);
    Timer timer;
    osn.setTimer(timer);
    timer.setTimePoint("start");
    receiver_shares = osn.run_osn(receiver_set, chls);
    timer.setTimePoint("total");
    cout << IoStream::lock;
    cout << "Receiver:" << endl;
    cout << timer << endl;
    size_t sent = 0, recv = 0;
    for (auto &chl : chls) {
        for (int i = 0; i <= 1; i++) {
            sent += chl[i].bytesSent();
            recv += chl[i].bytesReceived();
        }
    }
    cout << "recv: " << recv / 1024.0 / 1024.0 << "MB sent:" << sent / 1024.0 / 1024.0 << "MB "
         << "total: " << (recv + sent) / 1024.0 / 1024.0 << "MB" << endl;
    cout << IoStream::unlock;
}

int check_result(size_t size)
{
    int correct_cnt = 0;
    for (auto i = 0; i < size; i++) {
        block tmp = sender_shares[i] ^ receiver_shares[i];
        if (tmp == receiver_set[permutation[i]]) {
            correct_cnt++;
        }
    }
    return correct_cnt;
}
int main(int argc, char **argv)
{
    CLP cmd;
    cmd.parse(argc, argv);
    size_t size = 1 << cmd.getOr("size", 10);
    num_threads = cmd.getOr("thread", 10);

    vector<socketPair> chls;
    bool wan = cmd.getOr("wan", false);
    bool cache = cmd.getOr("cache", false);
    cout << "size:" << size << " num_threads:" << num_threads << endl;
    if (wan) {
        Address address("localhost", 1212);
        auto &ioc = coproto::global_io_context();
        for (size_t i = 0; i < num_threads; i++) {
            auto r = macoro::sync_wait(macoro::when_all_ready(
                macoro::make_task(coproto::AsioAcceptor((address += i).getAddress(), ioc)),
                macoro::make_task(coproto::AsioConnect((address += i).getAddress(), ioc))));
            chls.emplace_back(std::array<Socket, 2>{ std::move(std::get<0>(r).result()),
                                                     std::move(std::get<1>(r).result()) });
        }
    } else {
        for (size_t i = 0; i < num_threads; i++) {
            auto temp = cp::LocalAsyncSocket::makePair();
            chls.emplace_back(std::array<Socket, 2>{ std::move(temp[0]), std::move(temp[1]) });
        }
    }
    auto sender_thrd = thread(sender, size, std::ref(chls), cache);
    auto receiver_thrd = thread(receiver, size, std::ref(chls));
    sender_thrd.join();
    receiver_thrd.join();
    if (size == check_result(size))
        cout << "Correct!" << endl;
    else
        cout << "Wrong!" << endl;
    return 0;
}
