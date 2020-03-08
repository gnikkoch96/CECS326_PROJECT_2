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
	bool receivedBID = false;
	pid_t probeBID;

	while(isRunning){ //Always Running Until all Probes Terminate

        //Wait to receive ProbeB's ID
        if(!receivedBID){
            msgrcv(qid, (struct msgbuf*) &msg, greetingSize, 2, 0);

            int probeBPIDInt = atoi(msg.greetings);                                               //Converts to int
            probeBID = (pid_t) probeBPIDInt;
            receivedBID = true;
        }

        //Receive all messages sent to the DataHub
        msgrcv(qid, (struct msgbuf*) &msg, greetingSize, 1, 0);
        message_count++;


        //Check Termination of Probe A
        if(!strcmp(msg.greetings, "A_EXIT")){
            //(Debug) Probe A Terminated and is Disconnected from Message Queue
            cout << getpid() << "(DH): Probe A Disconnected" << endl;

            //Probe A Stops Running
            isARunning = false;

        }else if(!strcmp(msg.greetings, "Probe A: Hi")){                                         //Checks greetings for Probe A's Message (non-terminating)
            msg.mtype = 191;
            strncpy(msg.greetings, "DATAHUB: PROBE A ACKNOWLEDGED", greetingSize);               //Sends acknowledgment to Probe A
            msgsnd(qid, (struct msgbuf*)&msg, greetingSize, 0);
            message_count++;

            //(Debug) Display that DataHub Acknowledged Probe A
            cout << getpid() << "(DH): Acknowledged Probe A's Message" << endl;
        }

        //Check Termination of Probe B
        if(message_count >= 10000 && isBRunning){//Checks if the message_count >= 10000

            //Force Patch
            force_patch(probeBID);

             //(Debug) Probe B Terminated and is Disconnected from Message Queue
            cout << getpid() << "(DH): Probe B Disconnected" << endl;

            //Probe B Stops Running
            isBRunning = false;
        }

        //Terminate Probe C Conditions
        if(!strcmp(msg.greetings, "C_EXIT")){                                                   //Checks greetings for Probe C's Terminating Message

             //(Debug) Probe C Terminated and is Disconnected from Message Queue
            cout << getpid() << "(DH): Probe C Disconnected" << endl;

            //Probe C Stops Running
            isCRunning = false;
        }

        //(Update) Checks if All Probes are still running
        isRunning = isARunning || isBRunning|| isCRunning;
	}

	//Destroy Message Queue
    msgctl (qid, IPC_RMID, NULL);

    return 0;
}
