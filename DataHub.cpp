#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

/*
    1. DataHub receives messages from Probe A, B, and C.
    2. DataHub must acknowledge Probe A messages
    3. Used the data stored in the greetings field to determine conditions
*/

int main()
{
    srand(time(NULL));

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	//Message Struct
	struct msgO{
        long mtype;
        char greetings[50];
	};


	//Generating message object
	msgO msg;

    //Size of Greeting
    int greetingSize = sizeof(msgO) - sizeof(long);


	//Queue is Terminated after all Probes Exit the Queue
	bool isARunning = true, isBRunning = true, isCRunning = true;
	bool isRunning = isARunning || isBRunning|| isCRunning;

	while(isRunning){ //Always Running Until all Probes Terminate

        //It will only receive messages with mtype = 1, we will use the greetings field to help distinguish objectives, this will prevent it from trying to get messages sent to the Probes from other probes
        msgrcv(qid, (struct msgbuf*) &msg, greetingSize, 1, 0);

        //Check Termination of Probe A
        if(!strcmp(msg.greetings, "A_EXIT")){
            //(Debug) Probe A Terminated and is Disconnected from Message Queue
            cout << getpid() << " : Probe A Disconnected" << endl;

            //Probe A Stops Running
            isARunning = false;

        }else if(!strcmp(msg.greetings, "Probe A: Hi")){                                         //Checks greetings for Probe A's Message (non-terminating)
            msg.mtype = 191;
            strncpy(msg.greetings, "DATAHUB: PROBE A ACKNOWLEDGED", greetingSize);               //Sends acknowledgment to Probe A
            msgsnd(qid, (struct msgbuf*)&msg, greetingSize, 0);

            //(Debug) Display that DataHub Acknowledged Probe A
            cout << getpid() << " : Acknowledged Probe A's Message" << endl;
        }

        //Check Termination of Probe B
        if(!strcmp(msg.greetings, "B_EXIT")){                                                   //Checks greetings for Probe B's Terminating Message
             //(Debug) Probe B Terminated and is Disconnected from Message Queue
            cout << getpid() << " : Probe B Disconnected" << endl;

            //Probe B Stops Running
            isBRunning = false;
        }

        //Terminate Probe C Conditions
        if(!strcmp(msg.greetings, "C_EXIT")){                                                   //Checks greetings for Probe C's Terminating Message
             //(Debug) Probe C Terminated and is Disconnected from Message Queue
            cout << getpid() << " : Probe C Disconnected" << endl;

            //Probe C Stops Running
            isCRunning = false;
        }
	}

	//Destroy Message Queue
	//(IMPORTANT): Make sure that the Queue is Empty before deleting
    msgctl (qid, IPC_RMID, NULL);

    return 0;
}
