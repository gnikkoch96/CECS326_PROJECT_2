#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>
#include "A_C_Message_Counts.h"

using namespace std;

/* Probe B:
    1. Terminates when the number of message count in DataHub (not strictly Probe B)
       reaches 10,000 messages
    Note: Use force_patch to force Probe B out of the DataHub (i.e. it terminates its
          its signal from the message queue
*/

//Initializing Global Message_Count Variable (From Header File)
int message_count;

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

        //Terminate Condition
        if(message_count >= 10000){
            //Display Probe B Termination
            cout << "Probe B Termination Condition Met" << endl;
            cout << "Probe B is Now Exiting" << endl;

            //Send Signal to Message Queue of Probe A Termination
            msg.mtype = 2;
            strncpy(msg.greetings, to_string(getpid()).c_str(), greetingSize);   //Send PID of Probe B for Force Patch (Used in DataHub.cpp)
            msgsnd(qid, (struct msgbuf*)&msg, greetingSize, 0);

            isRunning = false;
            break;                                                   //Break from the While Loop if Probe B Terminates
        }

        //Valid Reading
        else if(randomNum % magic_seed_beta == 0){                   //Valid Reads Occur when the Random Number is Divisible by the Probe's Magic Seed
            //Universal mtype for DataHub
            msg.mtype = 1;

            //Store Message in Greetings Field of msg
            strncpy(msg.greetings, "Probe B: Hi ", greetingSize);    //Sends "Hi

            //Sending to Message Queue
            msgsnd(qid, (struct msgbuf*) &msg, greetingSize, 0);

            //(Debug) Outputs that Probe A has Sent a Message
            cout << getpid() << "(Probe B) : Sent Message" << endl;

            //Increment Message_Count
            ++message_count;
        }


	}

    return 0;
}
