#ifndef BLEA
#define BLEA TRUE

#define MaxRobots 100 
#define MaxType 10

class ETDispatch;

class Bigkernel
 {
 private:
   int imax;
  ETDispatch* Vect[MaxRobots];
 public:
  Bigkernel(){imax=0;}
  ~Bigkernel(){}
  void InsDispatcher(ETDispatch* a);
  void CommunicateRequest();
  void CommunicateMsg();
  void DoYourDuty(int=0);

};

#endif



