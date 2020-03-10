#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "force_patch.h"

using namespace std;

/*
    DataHub:
    1. DataHub receives messages from Probe A, B, and C.
    2. DataHub must acknowledge Probe A messages
*/


//Function Used to distinguish the Probe Messages sent to the DataHub
bool containsWord(char arr[], string word);

int main()
{
	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	//Message Struct
	struct msgO{
        long mtype;
        char greetings[50];
	};


	//Generating message object
	msgO msg;

    //Initialize Message_Count Variable (Used for Probe B Termination)
    int message_count = 0;

    //Size of Greeting
    int greetingSize = sizeof(msgO) - sizeof(long);

	//Queue is Terminated after all Probes Exit the Queue
	bool isARunning = true, isBRunning = true, isCRunning = true;
	bool isRunning = isARunning || isBRunning|| isCRunning;

	//Probe B PID
	pid_t probeBID;

	while(isRunning){ //Always Running Until all Probes Terminate

        //Receive all messages sent to the DataHub
        msgrcv(qid, (struct msgbuf*) &msg, greetingSize, 1, 0);
        message_count++;


        //Check Termination of Probe A
        if(msg.greetings[5] == 'A'){//Means that message is sent from Probe A
            if(msg.greetings[8] == 'A'){//Terminate A -> ProbeA: A_EXIT
                //(Debug) Probe A Terminated and is Disconnected from Message Queue
                cout << getpid() << "(DH): Probe A Disconnected" << endl;

                //Probe A Stops Running
                isARunning = false;
            }else{//Probe A Sent a Message with its PID and Random Number
                //Send Acknowledgment
                msg.mtype = 191;
                strncpy(msg.greetings, "DATAHUB: PROBE A ACKNOWLEDGED", greetingSize);               //Sends acknowledgment to Probe A
                msgsnd(qid, (struct msgbuf*)&msg, greetingSize, 0);
                message_count++;

                //(Debug) Display the Message Sent from ProbeA
                cout << getpid() << ": " << msg.greetings << endl;
            }
        }


        //Check Termination of Probe B
        if(msg.greetings[5] == 'B' && isBRunning){//Message was sent from Probe B
            if(message_count >= 10000){//Checks if the message_count >= 10000

                //Grab PID
                string BPID = "";
                for(int i = 8; i <= 11; i++){
                   BPID += msg.greetings[i];
                }

                //Convert BPID to int
                int probeBPIDInt = stoi(BPID);                                               //Converts to int
                probeBID = (pid_t) probeBPIDInt;

                //Force Patch
                force_patch(probeBID);

                 //(Debug) Probe B Terminated and is Disconnected from Message Queue
                cout << getpid() << "(DH): Probe B Disconnected" << endl;

                //Probe B Stops Running
                isBRunning = false;
            }else{
                cout << getpid() << ": " << msg.greetings << endl;
            }
        }

        //Terminate Probe C Conditions
        if(msg.greetings[5] == 'C'){//Message was sent from Probe C
            if(msg.greetings[8] == 'C'){ //Checks greetings for Probe C's Terminating Message

                 //(Debug) Probe C Terminated and is Disconnected from Message Queue
                cout << getpid() << "(DH): Probe C Disconnected" << endl;

                //Probe C Stops Running
                isCRunning = false;
            }else{
                cout << getpid() << ": " << msg.greetings << endl;
            }
        }

        //(Update) Checks if All Probes are still running
        isRunning = isARunning || isBRunning|| isCRunning;
	}

	//Destroy Message Queue
    msgctl (qid, IPC_RMID, NULL);

    return 0;
}

bool containsWord(char arr[], string word){
    //Index 5 is the Distinguishable Character
}
