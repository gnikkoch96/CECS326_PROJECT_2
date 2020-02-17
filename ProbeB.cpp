#include <iostream>

using namespace std;

/* Probe B:
    1. Terminates when the number of message count in DataHub (not strictly Probe B)
       reaches 10,000 messages
    Note: Use force_patch to force Probe B out of the DataHub (i.e. it terminates its
          its signal from the message queue

    2. mtype values for A: 103, C:102
    3. mtype for DataHub: 02 to let the DataHub know to terminate Probe B
*/

int main(){
    //Used to Generate Random Numbers
    srand(time(NULL));

    //magic_seed beta
    const int magic_seed_beta = 257;

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

    //Initialize Message Count - > Counts the # of Messages that is Sent to Message Queue (Used for testing Probe B Termination)
    int message_count = 0;

	//Probe B Starts off Running
	bool isRunning = true;
	while(isRunning){
        //Generate Random Number (Bounded by Computer Limits)
        int randomNum = rand();

        //Terminate Condition
        if(message_count >= 10000){
            //Display Probe B Termination
            cout << "Probe B Termination Condition Met: " << message_count << endl;
            cout << "Probe B is Now Exiting" << endl;

            //Send Signal to Message Queue of Probe A Termination
            msg.mtype = 2;
            strncpy(msg.greeting, "B_EXIT");                         //When this message is sent, we will look through message in DataHub for this String to Close B
            msgsnd(qid, (struct msgbuf*)msg, greetingSize, 0);

            //Probe B Shuts Down
            isRunning = false;
            break;                                                   //Break from the While Loop if Probe B Terminates
        }

        //Valid Reading
        else if(randomNum % magic_seed_beta == 0){                   //Valid Reads Occur when the Random Number is Divisible by the Probe's Magic Seed
            //Generate and store mtype of either 102 or 103
            int chooseOne = (rand() > RAND_MAX/2) ? 102:103;
            msg.mtype = chooseOne;

            //Store Message in Greetings Field of msg
            strncpy(msg.greeting, "Hi " + to_string(chooseOne));     //Sends "Hi 102/103"

            //Sending to Message Queue
            msgsnd(qid, (struct msgbuf*) msg, greetingSize, 0);

            //(Debug) Outputs that Probe A has Sent a Message
            cout << getpid() << " : Sent Message" << endl;

            //Increment Message_Count
            ++messageCount;
        }

        //(Experimental) Probe B Reads Messages Sent From either Probe A(103) or C(45)
        int chooseOne = (rand() > RAND_MAX/2) ? 103:45;
        msgrcv(qid, (struct msgbuf *)&msg, greetingSize, chooseOne, 0)

        //(Debug) Outputs that Probe has Received a Message
        cout << getpid() << " : Received Message" << endl;

	}

    return 0;
}
