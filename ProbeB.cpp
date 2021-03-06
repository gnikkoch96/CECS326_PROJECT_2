#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>

using namespace std;

/* Probe B:
    1. Terminates when the number of message count in DataHub (not strictly Probe B)
       reaches 10,000 messages
    Note: Use force_patch to force Probe B out of the DataHub (i.e. it terminates its
          its signal from the message queue
*/


int main(){
    //Used to Generate Random Numbers
    srand(time(NULL));

    //magic_seed beta
    const int magic_seed_beta = 257;

    //Gets the Queue ID that is currently on, if not then it returns a -1
    int qid = msgget(ftok(".",'u'), 0);

    // Declaring Message Buffer
	struct msgO {
		long mtype; // required
		char greetings[50];                                // mesg content
	};

	//Generating Message Object
	msgO msg;

    //Size of Greeting
	int greetingSize = sizeof(msgO) - sizeof(long);

	//Probe B Starts off Running
	bool isRunning = true;
	while(isRunning){
        //Generate Random Number (Bounded by Computer Limits)
        int randomNum = rand();

        //Valid Reading
        if(randomNum % magic_seed_beta == 0){                                       //Valid Reads Occur when the Random Number is Divisible by the Probe's Magic Seed
            //Universal mtype for DataHub
            msg.mtype = 1;

            //Store Message in Greetings Field of msg
            string bmessage = "ProbeB: " + to_string(getpid()) + " and " + to_string(randomNum);        //Stores "ProbeB: PIDX and Random Number"
            strncpy(msg.greetings, bmessage.c_str(), greetingSize);

            //Sending to Message Queue
            msgsnd(qid, (struct msgbuf*) &msg, greetingSize, 0);

            //(Debug) Outputs that Probe A has Sent a Message
            cout << getpid() << "(B): Sent Message" << endl;

        }
	}

	//(Debug) Terminate Probe B
	cout << getpid() << "(B): Probe B Terminated" << endl;

    return 0;
}
