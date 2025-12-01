/**
 * @file interrupts_EP_RR.cpp
 * @author Matthew Sanii
 * 
 */

#include<interrupts_101267993_Unknown.hpp>

void FCFS(std::vector<PCB> &ready_queue) {
    std::sort( 
                ready_queue.begin(),
                ready_queue.end(),
                []( const PCB &first, const PCB &second ){
                    return (first.arrival_time > second.arrival_time); 
                } 
            );
}

void RR(std::vector<PCB> &ready_queue) {
    std::sort( 
                ready_queue.begin(),
                ready_queue.end(),
                []( const PCB &first, const PCB &second ){
                    return (first.priority <= second.priority); 
                } 
            );
}

std::tuple<std::string /* add std::string for bonus mark */ > run_simulation(std::vector<PCB> list_processes) {

    std::vector<PCB> ready_queue;   //The ready queue of processes
    std::vector<PCB> wait_queue;    //The wait queue of processes
    std::vector<PCB> job_list;      //A list to keep track of all the processes. This is similar
                                    //to the "Process, Arrival time, Burst time" table that you
                                    //see in questions. You don't need to use it, I put it here
                                    //to make the code easier :).

    unsigned int current_time = 0;
    unsigned int max_single = 100;
    int selected = 0;
    std::vector<PCB> finished;
    PCB running;
    int next = -1;

    //Initialize an empty running process
    idle_CPU(running);

    std::string execution_status;

    //make the output table (the header row)
    execution_status = print_exec_header();

    //Loop while till there are no ready or waiting processes.
    //This is the main reason I have job_list, you don't have to use it.
    while(!all_process_terminated(job_list) || job_list.empty()) {

        //Inside this loop, there are three things you must do:
        // 1) Populate the ready queue with processes as they arrive
        // 2) Manage the wait queue
        // 3) Schedule processes from the ready queue

        //Population of ready queue is given to you as an example.
        //Go through the list of proceeses
        for(auto &process : list_processes) {
            if(process.arrival_time == current_time) {//check if the AT = current time
                //if so, assign memory and put the process into the ready queue
                if(assign_memory(process) == true){
                    process.state = READY;  //Set the process state to READY
                    ready_queue.push_back(process); //Add the process to the ready queue
                    job_list.push_back(process); //Add it to the list of processes
                    execution_status += print_exec_status(current_time, process.PID, NEW, READY);
                } else{
                    process.state = NOT_ASSIGNED;
                    wait_queue.push_back(process);
                    job_list.push_back(process); //Add it to the list of processes
}
            }
        }

        ///////////////////////MANAGE WAIT QUEUE/////////////////////////
        //This mainly involves keeping track of how long a process must remain in the ready queue

if(current_time - running.start_time == 100 && running.processing_time > 100 && !ready_queue.empty()){
            for(auto process : job_list){
            if(running == process && process.state == RUNNING){
                execution_status += print_exec_status(current_time, process.PID, process.state, READY);
                process.priority += 1;
                process.remaining_time -= 100;
                process.processing_time -= 100;
                if(running.io_freq > 100){
                process.io_freq -= 100;
}
                process.state = READY;
                sync_queue(job_list, process);
                sync_queue(ready_queue, process);
                idle_CPU(running);
                running.PID = -1;
                running.remaining_time = 0;
                running.io_saved = 0;
                running.state = READY;
}}}
            for(auto process : job_list){
            if(current_time - running.start_time == process.remaining_time && process == running){
                execution_status += print_exec_status(current_time, process.PID, process.state, TERMINATED);
                for(auto pro: ready_queue){pro.state = READY;
                sync_queue(ready_queue, pro);
}
                terminate_process(process, job_list);
                finished.push_back(running);
                idle_CPU(running);
                running.PID = -1;
                running.remaining_time = 0;
            }
            if(process.io_freq > 0){
            if(process == running){
                if(current_time == next && process.state == RUNNING) {
                    execution_status += print_exec_status(current_time, process.PID, process.state, WAITING);
                    next = -1;
                    process.remaining_time -= process.io_freq;
                    process.start_time = current_time;
                    idle_CPU(running);
                    running.start_time = 0;
                    process.state = WAITING;
                    process.io_saved = process.holder;
                    sync_queue(job_list, process);
                    wait_queue.push_back(process);
                }
}}}
            for(auto process : job_list){
            if((current_time - process.start_time) == process.io_duration && process.state == WAITING){
                execution_status += print_exec_status(current_time, process.PID, process.state, READY);
                process.state = READY;
                wait_queue.pop_back();
                sync_queue(job_list, process);
                ready_queue.push_back(process);
}}
        /////////////////////////////////////////////////////////////////

        //////////////////////////SCHEDULER//////////////////////////////

        if(!job_list.empty()){
            FCFS(ready_queue); //example of FCFS is shown here
            RR(ready_queue);
        }
        if(ready_queue.empty() && !all_process_terminated(job_list) && wait_queue.empty()){
        for(auto process: job_list){
            if(process.state != TERMINATED && process.state != RUNNING && process.state != WAITING){ready_queue.push_back(process);}
}}
        
        /////////////////////////////////////////////////////////////////
        
        if(running.PID == -1){
        for(auto &process:ready_queue){
            if(current_time >= process.arrival_time && process.state == READY && selected == 0){
                if(process.remaining_time > 0){                    
                    running.PID = process.PID;
                    running.remaining_time = process.remaining_time;
                    running.processing_time = process.processing_time;
                    running.io_duration = process.io_duration;
                    running.io_freq = process.io_freq;
                    execution_status += print_exec_status(current_time, process.PID, process.state, RUNNING);
                    run_process(process, job_list, ready_queue, current_time);
                    running.state = RUNNING;
                    process.state = RUNNING;
                    running.start_time = current_time;
                    sync_queue(job_list, running);
                    next = running.start_time + running.io_freq;
                    selected = 1;
} else{
                    execution_status += print_exec_status(current_time, process.PID, process.state, RUNNING);
                    run_process(process, job_list, ready_queue, current_time);
                    running.io_duration = process.io_duration;
                    sync_queue(job_list, running);
                    running.io_saved = process.io_saved;
                    running.PID = process.PID;
                    running.state = RUNNING;
                    running.io_freq = process.io_freq;
                    running.remaining_time = process.remaining_time;
                    running.start_time = current_time;
                    
                    next = running.start_time + running.io_freq;
                    selected = 1;
                }
            }}}
current_time += 1;
selected = 0;
if(current_time == 200){for(auto process: wait_queue){execution_status += print_exec_status(current_time, process.PID, process.state, RUNNING);}break;
}}
    //Close the output table
    execution_status += print_exec_footer();

    return std::make_tuple(execution_status);
}


int main(int argc, char** argv) {

    //Get the input file from the user
    if(argc != 2) {
        std::cout << "ERROR!\nExpected 1 argument, received " << argc - 1 << std::endl;
        std::cout << "To run the program, do: ./interrutps <your_input_file.txt>" << std::endl;
        return -1;
    }

    //Open the input file
    auto file_name = argv[1];
    std::ifstream input_file;
    input_file.open(file_name);

    //Ensure that the file actually opens
    if (!input_file.is_open()) {
        std::cerr << "Error: Unable to open file: " << file_name << std::endl;
        return -1;
    }

    //Parse the entire input file and populate a vector of PCBs.
    //To do so, the add_process() helper function is used (see include file).
    std::string line;
    std::vector<PCB> list_process;
    while(std::getline(input_file, line)) {
        auto input_tokens = split_delim(line, ", ");
        auto new_process = add_process(input_tokens);
        list_process.push_back(new_process);
    }
    input_file.close();

    //With the list of processes, run the simulation
    auto [exec] = run_simulation(list_process);

    write_output(exec, "execution_EP_RR.txt");

    return 0;
}
