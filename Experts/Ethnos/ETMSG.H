#ifndef	__ETMESSAGE_H__
#define	__ETMESSAGE_H__
#include <iostream.h>
#include <stdlib.h>

class ETMessage
{
	protected:
                short id;
                unsigned short Type; 
                char *data;
	        unsigned short dimension;
	public:

	ETMessage(unsigned short dim =0 ,unsigned short Type1 =0)
          {
            Type=Type1;
            dimension=dim;
            data=malloc(dim);
	    if (data==NULL) 
		{
		cerr << "\n**\n**\n** Errore in ETMessage!!\n**\n**\n**\n";
		exit(1); 	     
		}
          } 		
	~ETMessage() {free(data); }

		 unsigned short	ReadType()  { return Type;}
	
                char* GetData()
                    {
		     return data;
		    }
                const char* ReadData()
                    {
                     return data;

                    }
        	short	        GetID() const { return id; }
        	short	        GetKernelNum() const { return id; }
		void		SetID(unsigned short id1) { id=id1;}
	        void 		SetKernelNum(unsigned short id1) { id=id1; }
		int		ReadSize() { return dimension; }
};
#endif
