#pragma once
#include "OSNReceiver.h"
#include "MPOPRFReceiver.h"
#include "CuckooIndex.h"
#include "cryptoTools/Common/Timer.h"
#include <PSUReceiver.h>

// Shuffle-Receiver Receiver with Cuckoo Hash
class SRCReceiver : public PSUReceiver
{
	oc::CuckooIndex<oc::ThreadSafe> cuckoo;
	std::vector<oc::block> after_cuckoo_set;

	OSNReceiver osn_receiver;
	oc::MPOPRFReceiver mp_oprf_receiver;
	oc::IknpOtExtReceiver ot_receiver;

	std::vector<oc::block> runPermuteShare(const std::vector<oc::block>& x_set, std::vector<oc::Channel>& chls);
	oc::u8* runMpOprf(std::vector<oc::Channel>& chls,
		const std::vector<oc::block>& recv_set,
		const oc::block& commonSeed,
		const oc::u64& set_size,
		const oc::u64& logHeight,
		const oc::u64& width,
		const oc::u64& hashLengthInBytes,
		const oc::u64& h1LengthInBytes,
		const oc::u64& bucket1,
		const oc::u64& bucket2);
 public:
	void setReceiverSet(const std::vector<oc::block>& receiver_set, size_t sender_size);
	std::vector<oc::block> output(std::vector<oc::Channel>& chls);
	void setTimer(oc::Timer& timer);
};
