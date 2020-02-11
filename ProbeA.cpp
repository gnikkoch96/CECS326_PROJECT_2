#include <iostream>

/* Probe A:
    1. Terminates if Random Number Generated < 50
    2. Awaits for DataHub Signal before being able to send another message
    3. mtype values for B: 192 and C: 193, so this means whenever we are recieving we will know that mtypes of 192 will be from B and 193 will be from C
    4. mtype to send to the Hub Directly: 1. We strictly use these to either get acknowledge or get terminated. (Messages to send to the hub directly)
*/


using namespace std;

int main(){
    //Random
    srand(time(NULL));

    //magic_seed alpha
    const int alpha = 997;

    //Gets the Queue ID that is currently on, if not then it returns a -1
    int qid = msgget(ftok(".",'u'), 0);

    // Declaring Message Buffer
    int greetingSize = 50;                                          //Size of Message String
	typdef struct buf {
		long mtype; // required
		char greeting[greetingSize];                                // mesg content
	}message_buf;

	//Generating Message Object
	message_buf msg;

	//Probe A is Started Initially
	bool isRunning = true;
	while(isRunning){
        //Terminate Condition

        //Valid Reading

	}

    return 0;
}
