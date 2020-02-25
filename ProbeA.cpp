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

/* Probe A:
    1. Terminates if Random Number Generated < 50
    2. Awaits for DataHub Signal before being able to send another message (mtype for DataHub -> 35)
    3. mtype values for B: 192 and C: 193, so this means whenever we are recieving we will know that mtypes of 192 will be from B and 193 will be from C
    4. mtype to send to the Hub Directly: 1. We strictly use these to either get acknowledge or get terminated. (Messages to send to the hub directly)
*/

class A_Messages{
    //This class will be used to calculate the # of messages sent from Probe A
    //This value will be accessed in Probe B only as it requires the knowledge of Messages sent for termination

    public:
       	//Probe B Terminate Variable (Counts the # of Messages Sent
        static int message_count;
};

	//Initializing A_Message Message_Count
	int A_Messages::message_count = 0;


int main(){
    //Used to Generate Random Numbers
    srand(time(NULL));

    //magic_seed alpha
    const int magic_seed_alpha = 997;

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

	//Used to Determine if Probe A has been Acknowledge by the DataHub
    bool isAcknowledge = true;

    //Used to Check if a Valid Reading was done, this is used for the Acknowledgment part
    bool validMessage = false;

	//Probe A Starts off Running
	bool isRunning = true;
	while(isRunning){
        //Generate Random Number (Bounded by Computer Limits)
        int randomNum = rand();

        //Terminate Condition
        if(randomNum < 50){
            //Display Probe A Termination
            cout << "Probe A Termination Condition Met: " << randomNum << endl;
            cout << "Probe A is Now Exiting" << endl;

            //Send Signal to Message Queue of Probe A Termination
            msg.mtype = 1;
            strncpy(msg.greetings, "A_EXIT", greetingSize);          //When this message is sent, we will look through message in DataHub for this String to Close A
            msgsnd(qid, (struct msgbuf*)&msg, greetingSize, 0);

            //Probe A Shuts Down
            isRunning = false;
            break;                                                   //Break from the While Loop if Probe A Terminates
        }

        //Valid Reading
        else if(randomNum % magic_seed_alpha == 0 && isAcknowledge){ //Valid Reads Occur when the Random Number is Divisible by the Probe's Magic Seed

            //Universal mtype to send to DataHub
            msg.mtype = 1;

            //Store Message in Greetings Field of msg
            strncpy(msg.greetings, "Probe A: Hi", greetingSize);     //Sends "Hi"

            //Sending to Message Queue
            msgsnd(qid, (struct msgbuf*) &msg, greetingSize, 0);

            //(Debug) Outputs that Probe A has Sent a Message
            cout << getpid() << "Probe A: Sent Message" << endl;

            //Change isAcknowledge to False to Wait for Signal, to be able to send another message
            isAcknowledge = false;

            //Count the Messages Being sent from Probe A to DataHub
            A_Messages::message_count++;

            //Changes Valid Message to True
            validMessage = true;

        }

        if(validMessage){

            do{
                //Waiting for Acknowledgment from DataHub (mtype messages received from DataHub will be 191)
                msgrcv(qid, (struct msgbuf*) &msg, greetingSize, 191, 0);

            }while(strcmp(&msg.greetings,"DATAHUB: PROBE A ACKNOWLEDGED"));                                     //While it isn't the message we are looking for, keep receiving until you get it


            //Display to Console that Probe A Message has been acknowledge
            cout << getpid() << "Probe A: Received Acknowledgment from Data Hub" << endl;
            isAcknowledge = true;

            //Change Valid Message to False to go through cycle again
            validMessage = false;
        }


	}

    return 0;
}
