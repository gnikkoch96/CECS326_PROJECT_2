#include <iostream>

/* Probe A:
    1. Terminates if Random Number Generated < 50
    2. Awaits for DataHub Signal before being able to send another message
*/
using namespace std;

int main(){
    //magic_seed alpha
    const int alpha = 997;

    //Gets the Queue ID that is currently on, if not then it returns a -1
    int qid = msgget(ftok(".",'u'), 0);

    // declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};

    return 0;
}
