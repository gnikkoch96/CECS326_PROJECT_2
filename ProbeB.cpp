#include <iostream>

using namespace std;

/* Probe B:
    1. Terminates when the number of message count in DataHub (not strictly Probe B)
       reaches 10,000 messages
    Note: Use force_patch to force Probe B out of the DataHub (i.e. it terminates its
          its signal from the message queue

    2. mtype values for A: 103, C:102
    3. mtype for DataHub: 01, We strictly use these to either get acknowledge or get terminated
*/

int main(){
    //magic_seed beta
    const int beta = 257;

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
