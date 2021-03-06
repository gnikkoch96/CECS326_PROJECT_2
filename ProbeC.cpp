#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>
#include "kill_patch.h"


using namespace std;

/* Probe C:
    1. User has to use a kill command on a terminal to cancel Probe
    Note: Kill_Patch is provided by the Professor

*/

int main(){
    //Used to Generate Random Numbers
    srand(time(NULL));

     //magic_seed rho
    const int magic_seed_rho = 251;

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

	//Kill Patch
	msg.mtype = 1;
	strncpy(msg.greetings, "ProbeC: C_EXIT", greetingSize);
	kill_patch(qid, (struct msgbuf*) &msg, greetingSize, 1);

	//Probe C Starts off Running
	bool isRunning = true;
	while(isRunning){
        //Generate Random Number (Bounded by Computer Limits)
        int randomNum = rand();

        //Valid Reading
		if(randomNum % magic_seed_rho == 0){                   		//Valid Reads Occur when the Random Number is Divisible by the Probe's Magic Seed
            //Universal mtype for DataHub
            msg.mtype = 1;

            //Store Message in Greetings Field of msg
            string cmessage = "ProbeC: " + to_string(getpid()) + " and " + to_string(randomNum);
            strncpy(msg.greetings, cmessage.c_str(), greetingSize);   //Sends "Hi

            //Sending to Message Queue
            msgsnd(qid, (struct msgbuf*)&msg, greetingSize, 0);

            //(Debug) Outputs that Probe C has Sent a Message
            cout << getpid() << "(C): Sent Message" << endl;

        }

	}
    return 0;
}

