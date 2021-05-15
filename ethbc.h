#ifndef __BDEXPERTRX_H__
#define __BDEXPERTRX_H__ TRUE

#define ETPRIO_MIN	0
#define ETPRIO_SURE	255
#define MAXNTYPES	90

struct ETMsgHeader
{
        unsigned short  size;
        unsigned short  type;
        unsigned short  kernelnum;
        long            kerneldate;
        unsigned short  ID;
        unsigned char   priority;
        unsigned char   nrec;
};

struct ETMessage
{
         struct ETMsgHeader*    Header;
         char*                  Data;
         char*                  Rec;
         char*                  Msg;
};

int             ETCreateMsg(struct ETMessage *msg, int size, int type);
void            ETFreeMsg(struct ETMessage *msg);

void			InitBroadcast (const char *address, int port, int knum);
void			CloseBroadcast();
void			AddRequest(short);
void			RemoveRequest(short);
void			ReceiveMail();
void			TransmitMail();
void			ProcessAckList();
void			ShareMsg(struct ETMessage *, int);
int			MsgToBeRead();
struct ETMessage*	GetNextMsg();

#endif
