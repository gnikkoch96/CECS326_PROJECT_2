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

/* Probe C:
    1. User has to use a kill command on a terminal to cancel Probe
    Note: Kill_Patch is provided by the Professor

*/

class C_Messages{
    //This class will be used to calculate the # of messages sent from Probe A
    //This value will be accessed in Probe B only as it requires the knowledge of Messages sent for termination

    public:
       	//Probe B Terminate Variable (Counts the # of Messages Sent
        static int message_count;
};

    //Initializing C_Message Message_Count
	C_Messages::message_count = 0;


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

	//Kill Patch Inclusion
	msg.mtype = 1;
	strncpy(msg.greeting, "C_EXIT");
	kill_patch(qid, msg, greetingSize, 1);

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
            strncpy(msg.greetings, "Probe C: Hi ");                 //Sends "Hi

            //Sending to Message Queue
            msgsnd(qid, (struct msgbuf*)&msg, greetingSize, 0);

            //(Debug) Outputs that Probe C has Sent a Message
            cout << getpid() << "(Probe C): Sent Message" << endl;

            //Increment Message_Count
            ++C_Messages::message_count;
        }


    return 0;
}
