#include <iostream>

/* Probe A:
    1. Terminates if Random Number Generated < 50
    2. Awaits for DataHub Signal before being able to send another message (mtype for DataHub -> 35)
    3. mtype values for B: 192 and C: 193, so this means whenever we are recieving we will know that mtypes of 192 will be from B and 193 will be from C
    4. mtype to send to the Hub Directly: 1. We strictly use these to either get acknowledge or get terminated. (Messages to send to the hub directly)
    5
*/


using namespace std;

int main(){
    //Random
    srand(time(NULL));

    //magic_seed alpha
    const int magic_seed_alpha = 997;

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

	//Acknowledge Variable
	bool isAcknowledge = true;

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
            strncpy(msg.greeting, "EXIT");
            msgsnd(qid, (struct msgbuf*)msg, greetingSize, 0);

            //Probe A Shuts Down
            isRunning = false;
        }

        //Valid Reading
        else if(randomNum % magic_seed_alpha == 0 && isAcknowledge){ //Valid Reads Occur when the Random Number is Divisible by Probe's Magic Seed
            //Generate and store mtype of either 192 or 193
            int chooseOne = (rand() > RAND_MAX/2) ? 192:193;
            msg.mtype = chooseOne;

            //Store Message in Greetings Field of msg
            strncpy(msg.greeting, "Hi " + to_string(chooseOne));     //Sends "Hi 192/193"

            //Sending to Message Queue
            msgsnd(qid, (struct msgbuf*) msg, greetingSize, 0);

            //(Debug) Outputs that Probe A has Sent a Message
            cout << getpid() << " : Sent Message" << endl;

            //Change isAcknowledge to False to Wait for Signal, to be able to send another message
            isAcknowledge = false;

        }

        //Waiting for Acknowledgement from DataHub (mtype messages received from DataHub will be 35)
        msgrcv(qid, (struct msgbuf*) &msg, greetingSize, 35, 0);

        //Display to Console that Probe A Message has been acknowledge
        cout << getpid() << " : Received Acknowledge from Data Hub" << endl;
        isAcknowledge = true;

        //(Experimental) Probe A Reads Messages Sent From either Probe B(103) or C(45)
        int chooseOne = (rand() > RAND_MAX/2) ? 103:45;
        msgrcv(qid, (struct msgbuf *)&msg, greetingSize, chooseOne, 0)

        //(Debug) Outputs that Probe has Received a Message
        cout << getpid() << " : Received Message" << endl;


	}

    return 0;
}
