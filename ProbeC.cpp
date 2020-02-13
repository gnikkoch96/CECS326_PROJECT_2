#include <iostream>

using namespace std;

/* Probe C:
    1. User has to use a kill command on a terminal to cancel Probe
    Note: Kill_Patch is provided by the Professor

    2. mtype value for A: 45, B:93
    3. mtype value for DataHub: 03, to let the DataHub know ProbeC is terminated
*/

int main(){
    //magic_seed rho
    const int magic_seed_rho = 251;

    //Gets the Queue ID that is currently on, if not then it returns a -1
    int qid = msgget(ftok(".",'u'), 0);

    // declare my message buffer
    int greetingSize = 50;
	typdef struct buf {
		long mtype; // required
		char greeting[greetingSize]; // mesg content
	}message_buf;


    return 0;
}
