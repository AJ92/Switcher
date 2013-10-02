#ifndef CKEYCAPTURER_H
#define CKEYCAPTURER_H

#include <w32std.h>
#include <apgwgnam.h> //CApaWindowGroupName

class MKeyCallBack
{
public:
    virtual TBool KeyCapturedL(TWsEvent aEvent) = 0;
};

class CKeyCapturer : public CActive
{
public:
    static CKeyCapturer* NewL(MKeyCallBack& aObserver);
    static CKeyCapturer* NewLC(MKeyCallBack& aObserver);
    virtual ~CKeyCapturer();
private:
    CKeyCapturer(MKeyCallBack& aObserver);
    void ConstructL();
    void RunL();
    void DoCancel();
    void Listen();
private:
    MKeyCallBack& 	iObserver;
    RWsSession     	iWsSession;
    RWindowGroup    iWg;
    TInt 			iHandle;
};

#endif // CKEYCAPTURER_H
