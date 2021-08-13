#pragma once
#include "OSNSender.h"
#include "MPOPRFSender.h"
#include "CuckooIndex.h"
#include "cryptoTools/Common/Timer.h"
#include <PSUSender.h>
// Shuffle-Receiver Sender with Cuckoo Hash
class SRCSender : public PSUSender
{
	oc::CuckooIndex<oc::ThreadSafe> cuckoo;

	OSNSender osn_sender;
	oc::MPOPRFSender mp_oprf_sender;
	oc::IknpOtExtSender ot_sender;
	std::vector<oc::block> runPermuteShare(std::vector<oc::Channel>& chls);
	void runMPOPRF(std::vector<oc::Channel>& chls, size_t width, size_t hash_length_in_bytes);

public:
	void setSenderSet(const std::vector<oc::block>& sender_set, size_t receiver_size);
	void output(std::vector<oc::Channel>& chls);
	void setTimer(oc::Timer& timer);
};


