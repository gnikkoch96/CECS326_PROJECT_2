#ifndef A_C_MESSAGE_COUNTS_H_INCLUDED
#define A_C_MESSAGE_COUNTS_H_INCLUDED

class Global_Message_Count{
    //This class will be used to calculate the # of messages sent from Probe A and C
    //This value will be accessed in Probe B only as it requires the knowledge of Messages sent for termination

    public:
       	//Probe B Terminate Variable (Counts the # of Messages Sent
        static int amessage_count;
        static int cmessage_count;
};



#endif // A_C_MESSAGE_COUNTS_H_INCLUDED
