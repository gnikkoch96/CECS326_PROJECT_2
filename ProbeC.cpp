#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "kill_patch.h"

using namespace std;

/* Probe C:
    1. User has to use a kill command on a terminal to cancel Probe
    Note: Kill_Patch is provided by the Professor

    2. mtype value for A: 45, B:93
    3. mtype value for DataHub: 03, to let the DataHub know ProbeC is terminated
*/

class C_Messages{
    //This class will be used to calculate the # of messages sent from Probe A
    //This value will be accessed in Probe B only as it requires the knowledge of Messages sent for termination

    public:
        static int getTotalMessageSent(){
            return message_count;
        }
    private:
       	//Probe B Terminate Variable (Counts the # of Messages Sent
        static int message_count = 0;

};

int main(){
    //Used to Generate Random Numbers
    srand(time(NULL));

     //magic_seed rho
    const int magic_seed_rho = 251;

    //Gets the Queue ID that is currently on, if not then it returns a -1
    int qid = msgget(ftok(".",'u'), 0);


    // Declaring Message Buffer
	typdef struct buf {
		long mtype; // required
		char greeting[50];                                // mesg content
	}message_buf;

	//Generating Message Object
	message_buf msg;

	//Size of Greeting
	int greetingSize = sizeof(msg) - sizeof(long);

	//Kill Patch Inclusion
	msg.mtype = 3;
	strncpy(msg.greeting, "Probe C Exiting");
	kill_patch(qid, msg, greetingSize, 10);

	//Probe C Starts off Running
	bool isRunning = true;
	while(isRunning){
        //Generate Random Number (Bounded by Computer Limits)
        int randomNum = rand();

        //Valid Reading
		if(randomNum % magic_seed_rho == 0){                   		//Valid Reads Occur when the Random Number is Divisible by the Probe's Magic Seed
            //Generate and store mtype of either 45 or 93
            int chooseOne = (rand() > RAND_MAX/2) ? 45:93;
            msg.mtype = chooseOne;

            //Store Message in Greetings Field of msg
            strncpy(msg.greeting, "Probe C: Hi " + to_string(chooseOne));     //Sends "Hi 45/93"

            //Sending to Message Queue
            msgsnd(qid, (struct msgbuf*)&msg, greetingSize, 0);

            //(Debug) Outputs that Probe C has Sent a Message
            cout << getpid() << "(Probe C): Sent Message" << endl;

            //Increment Message_Count
            ++C_Messages.message_count;
        }


    return 0;
}
