#include <iostream>

using namespace std;

/*
    1. Checks Termination by checking received messages by individual probes (01 - A, 02 - B, 03 - C)
    If the Queue receives any message with numbers mentioned previously, it is going to terminate their respective probe
*/

//Checks the Message for a Specific Word - Used for Looking for EXIT Messages
bool checkGreeting(char *greeting, char *word);                                                  //Returns (1) if Word Exists, (0) if Word doesn't exist

int main()
{
	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	//Message Struct
	int greetingSize = 50;
	typedef struct msg{
        long mtype;
        char greetings[greetingSize];
	}message_buf;

	//Generating message object
	message_buf msg;

	//Queue is Terminated after all Probes Exit the Queue
	bool isARunning = true, isBRunning = true;, isCrunning = true;          	                //Question: Is it right to assume all Probe are running correctly?
	bool isRunning = isARunning || isBRunning|| isCRunning;                                     //The Data Hub is Running as long as one Probe is still running

	while(isRunning){
        //Always Running Until all Probes Terminate

        //Check Termination of Probe A
        //Receive Message to Check for Termination
        if(msgrcv(qid, (struct msgbuf*)msg, greetingSize, 1, 0) != -1){
            //(Debug) Probe A Terminated and is Disconnected from Message Queue
            cout << getpid() << " : Probe A Disconnected" << endl;

            //Probe A Stops Running
            isARunning = false;
        }else{ //Probe A Acknowledgment
            msg.mtype = 35;
            strncpy(msg.greeting, "ACKNOWLEDGED");                                               //Sends "ACKNOWLEDGED" to Probe A
            msgsnd(qid, (struct msgbuf*) msg, greetingSize, 0);

            //(Debug) Display that DataHub Acknowledged Probe A
            cout << getpid() << " : Acknowledged Probe A's Message" << endl;
        }

        //Check Termination of Probe B
        //Receive Message to Check for Termination
        if(msgrcv(qid, (struct msgbuf*)msg, greetingSize, 2, 0) != -1){
             //(Debug) Probe B Terminated and is Disconnected from Message Queue
            cout << getpid() << " : Probe B Disconnected" << endl;

            //Probe B Stops Running
            isBRunning = false;
        }

        //Terminate Probe C Conditions
        //Receive Message to Check for Termination
        if(msgrcv(qid, (struct msgbuf*)msg, greetingSize, 3, 0) != -1){
             //(Debug) Probe C Terminated and is Disconnected from Message Queue
            cout << getpid() << " : Probe C Disconnected" << endl;

            //Probe C Stops Running
            isCRunning = false;
        }
	}

	//Destroy Message Queue

    return 0;
}
