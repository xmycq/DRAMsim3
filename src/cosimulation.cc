#include "cosimulation.h"

// using namespace dramsim3;

CoDRAMsim3::CoDRAMsim3(const std::string &config_file, const std::string &output_dir)
    : memory(config_file, output_dir,
        std::bind(&CoDRAMsim3::callback, this, std::placeholders::_1, false),
        std::bind(&CoDRAMsim3::callback, this, std::placeholders::_1, true)),
        dram_clock(0),
        transcation_id(0) {
    std::cout << "DRAMsim3 memory system initialized." << std::endl;
}

void CoDRAMsim3::tick() {
    memory.ClockTick();
    dram_clock++;
}

bool CoDRAMsim3::add_request(const CoDRAMRequest &request) {
    if (memory.WillAcceptTransaction(request.address, request.is_write)) {
        if (request.is_write) {
            std::cout << "send write request with addr 0x" << std::hex << request.address << " to DRAMsim3" << std::endl;
        }
        else {
            std::cout << "send read request with addr 0x" << std::hex << request.address << " to DRAMsim3" << std::endl;
        }
        memory.AddTransaction(request.address, request.is_write);
        req_list.push_back(new CoDRAMResponse(request, get_clock_ticks()));
        return true;
    }
    return false;
}

CoDRAMResponse *CoDRAMsim3::check_response() {
    if (resp_queue.empty())
        return NULL;
    auto resp = resp_queue.front();
    resp_queue.pop();
    resp->resp_time = get_clock_ticks();
    return resp;
}

void CoDRAMsim3::callback(uint64_t addr, bool is_write) {
    std::cout << "cycle " << std::dec << get_clock_ticks() << " callback "
              << "is_write " << std::dec << is_write << " addr " << std::hex << addr << std::endl;
    // search for the first matched request
    auto iter = req_list.begin();
    while (iter != req_list.end()) {
        auto resp = *iter;
        if (resp->req.address == addr && resp->req.is_write == is_write) {
            req_list.erase(iter);
            resp->finish_time = get_clock_ticks();
            resp_queue.push(resp);
            return;
        }
        iter++;
    }
    std ::cout << "INTERNAL ERROR: Do not find matched request for this response "
               << "(0x" << std::hex << addr << ", " << is_write << ")." << std::endl;
    abort();
}
