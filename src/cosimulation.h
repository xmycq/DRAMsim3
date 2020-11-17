#ifndef __COSIMULATION_H
#define __COSIMULATION_H

#include <list>
#include <queue>
#include <string>

class CoDRAMRequest {
public:
    uint64_t address;
    bool is_write;
    void *meta;

    CoDRAMRequest() : CoDRAMRequest(0, false, NULL) { }
    CoDRAMRequest(uint64_t address, bool is_write)
        : CoDRAMRequest(address, is_write, meta) { }
    CoDRAMRequest(uint64_t address, bool is_write, void *meta)
        : address(address), is_write(is_write), meta(meta) { }
};


class CoDRAMResponse {
public:
    const CoDRAMRequest *req;
    uint64_t req_time;
    uint64_t finish_time;
    uint64_t resp_time;

    CoDRAMResponse(const CoDRAMRequest *req, uint64_t req_time)
        : req(req), req_time(req_time) { }
};


class CoDRAMsim3 {
public:
    // Initialize a DRAMsim3 model.
    CoDRAMsim3(const std::string &config_file, const std::string &output_dir);
    ~CoDRAMsim3();
    // Tick the DRAM model.
    void tick();
    // Returns true on success and false on failure.
    bool will_accept(uint64_t address, bool is_write);
    // Send request to CoDRAM model.
    bool add_request(const CoDRAMRequest *request);
    // Check whether there is some read response available. Returns NULL on failure.
    CoDRAMResponse *check_read_response();
    // Check whether there is some write response available. Returns NULL on failure.
    CoDRAMResponse *check_write_response();
    // Get DRAM ticks.
    inline uint64_t get_clock_ticks() { return dram_clock; }

private:
    uint64_t dram_clock;
    uint64_t transcation_id;
    std::list<CoDRAMResponse*> req_list;
    std::queue<CoDRAMResponse*> resp_read_queue;
    std::queue<CoDRAMResponse*> resp_write_queue;

    void callback(uint64_t addr, bool is_write);
    // Check whether there is some response in the queue. Returns NULL on failure.
    CoDRAMResponse *check_response(std::queue<CoDRAMResponse*> &resp_queue);
};





#endif
