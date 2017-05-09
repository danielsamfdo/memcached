void MemcacheElement::update_expiration_time(uint64_t current_time, uint64_t val){
    if(val > 0){
        log_info << "RELATIVE EXP TIME IN SECONDS " << val << endl;
        log_info << "CURRENT TIME IN SECONDS " << current_time << endl;
        log_info << "EXPIRATION TIME IN SECONDS " << current_time + val << endl;
        exptime = current_time + val;
        infinite = false;
    }
    else if(val == 0){
        infinite = true;
    }
    else{
        exptime = 0;
        infinite = false;
    }
    return ;
}

bool MemcacheElement::expired(uint64_t current_time){
    if(infinite){
        return false;
    }
    if(current_time > exptime){
        log_info << "CURRENT TIME IN SECONDS " << current_time << endl;
        log_info << "EXPIRATION TIME IN SECONDS " << exptime << endl;
        return true;
    }
    return false;
}
