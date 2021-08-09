//
// Created by dujiajun on 2021/8/8.
//

#include "SRSender.h"

using namespace std;
using namespace oc;

std::vector<oc::block> SRSender::runPermuteShare(std::vector<oc::Channel>& chls)
{
	return osn_sender.run_osn(chls);
}

void SRSender::runMPOPRF(std::vector<oc::Channel>& chls, const oc::block& commonSeed, const oc::u64& set_size, const oc::u64& logHeight, const oc::u64& width, const oc::u64& hashLengthInBytes, const oc::u64& h1LengthInBytes, const oc::u64& bucket1, const oc::u64& bucket2)
{
	PRNG prng(oc::toBlock(123));
	mp_oprf_sender.setParams(commonSeed, set_size, logHeight, width, hashLengthInBytes, h1LengthInBytes, bucket1, bucket2);
	mp_oprf_sender.run(prng, chls);
}

void SRSender::setSenderSet(const std::vector<oc::block>& sender_set, size_t receiver_size)
{
	this->sender_set_size = sender_set.size();
	this->sender_set = sender_set;

	shuffle_size = receiver_size;
	receiver_set_size = receiver_size;

	osn_sender.init(shuffle_size, 1);
}

void SRSender::output(std::vector<oc::Channel>& chls)
{
	PRNG prng(oc::toBlock(123));

	auto share = runPermuteShare(chls);

	runMPOPRF(chls, toBlock(123456), shuffle_size, log2ceil(shuffle_size), get_mp_oprf_width(shuffle_size), get_mp_oprf_hash_in_bytes(shuffle_size), 32, 1 << 8, 1 << 8);

	size_t num_threads = chls.size();
	auto routine = [&](size_t tid)
	{
		for (size_t x = tid; x < sender_set_size; x += num_threads)
		{
			vector<block> tmp(share.size());
			for (size_t i = 0; i < share.size(); i++)
				tmp[i] = _mm_xor_si128(sender_set[x], share[i]);
			//timer->setTimePoint("sender before get single oprf");
			auto oprfs = mp_oprf_sender.get_oprf(tmp);
			//timer->setTimePoint("sender after get single oprf");
			chls[tid].send(oprfs);
			//timer->setTimePoint("sender after send single oprf");
		}
	};
	vector<thread> thrds(num_threads);
	for (size_t i = 0; i < num_threads; i++)
		thrds[i] = std::thread(routine, i);
	for (size_t i = 0; i < num_threads; i++)
		thrds[i].join();

	vector<array<block, 2>> msgs(share.size());
	for (size_t i = 0; i < sender_set.size(); i++)
	{
		msgs[i][0] = sender_set[i];
		msgs[i][1] = ZeroBlock;
	}
	ot_sender.sendChosen(msgs, prng, chls[0]);
}

void SRSender::setTimer(oc::Timer& timer)
{
	this->timer = &timer;
	osn_sender.setTimer(timer);
	mp_oprf_sender.setTimer(timer);
}