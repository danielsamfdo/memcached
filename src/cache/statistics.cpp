string Statistics::process_get_statistics(uint64_t ut, string version) {
    log_info << "Processing command to Get Statistics " << endl;
    string output = "";
    output += "Uptime: " + to_string(ut) + "\n";
    output += "Version: " + version + "\n";
    output += "Bytes: " + to_string(allocated) + "\n";
    output += "Curr_items: " + to_string(curr_items) + "\n";    
    output += "Limit MaxBytes: " + to_string(limit_maxbytes) + "\n";
    output += "Cmd_get: " + to_string(cmd_get) + "\n";
    output += "Cmd_set: " + to_string(cmd_set) + "\n";
    output += "Cmd_flush: " + to_string(cmd_flush) + "\n";
    output += "get_hits: " + to_string(get_hits) + "\n";
    output += "get_misses: " + to_string(get_misses) + "\n";
    output += "incr_hits: " + to_string(incr_hits) + "\n";
    output += "incr_misses: " + to_string(incr_misses) + "\n";
    output += "decr_hits: " + to_string(decr_hits) + "\n";
    output += "decr_misses: " + to_string(decr_misses) + "\n";
    output += "delete_hits: " + to_string(delete_hits) + "\n";
    output += "delete_misses: " + to_string(delete_misses) + "\n";
    output += "cas_hits: " + to_string(cas_hits) + "\n";
    output += "cas_misses: " + to_string(cas_misses) + "\n";

    return output;
}