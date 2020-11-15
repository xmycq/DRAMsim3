#ifndef __COSIMULATION_H
#define __COSIMULATION_H

#include <list>
#include <queue>
#include "memory_system.h"

class CoDRAMRequest {
public:
    uint64_t address;
    bool is_write;

    CoDRAMRequest(uint64_t address, bool is_write)
        : address(address), is_write(is_write) { }
};


class CoDRAMResponse {
public:
    CoDRAMRequest req;
    uint64_t req_time;
    uint64_t finish_time;
    uint64_t resp_time;

    CoDRAMResponse(CoDRAMRequest req, uint64_t req_time)
        : req(req), req_time(req_time) { }
};


class CoDRAMsim3 {
public:
    // Initialize a DRAMsim3 model.
    CoDRAMsim3(const std::string &config_file, const std::string &output_dir);
    // Tick the DRAM model.
    void tick();
    // Send request to CoDRAM model. Returns true on success and false on failure.
    bool add_request(const CoDRAMRequest &request);
    // Check whether there is some response available. Returns NULL on failure.
    CoDRAMResponse *check_response();
    // Get DRAM ticks.
    inline uint64_t get_clock_ticks() { return dram_clock; }

private:
    dramsim3::MemorySystem memory;
    uint64_t dram_clock;
    uint64_t transcation_id;
    std::list<CoDRAMResponse*> req_list;
    std::queue<CoDRAMResponse*> resp_queue;

    void callback(uint64_t addr, bool is_write);
};





#endif
