#include <iostream>

using namespace std;

int main()
{
	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	//Queue is Terminated after all Probes Exit the Queue
	bool isARunning = true, isBRunning = true;, isCrunning = true;          	                //Question: Is it right to assume all Probe are running correctly?
	bool isRunning = isARunning || isBRunning|| isCRunning;                                     //The Data Hub is Running as long as one Probe is still running

	while(isRunning){
        //Always Running Until all Probes Terminate


        //Terminate Probe A Conditions

        //Terminate Probe B Conditions

        //Terminate Probe C Conditions
	}

	//Destroy Message Queue

    return 0;
}
