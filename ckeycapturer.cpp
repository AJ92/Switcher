#include "ckeycapturer.h"

CKeyCapturer* CKeyCapturer::NewL(MKeyCallBack& aObserver)
    {
    CKeyCapturer* self = CKeyCapturer::NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
    }

CKeyCapturer* CKeyCapturer::NewLC(MKeyCallBack& aObserver)
    {
    CKeyCapturer* self = new (ELeave) CKeyCapturer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CKeyCapturer::CKeyCapturer(MKeyCallBack& aObserver)
:CActive(EPriorityStandard),iObserver(aObserver),iHandle(-1)
{
}

CKeyCapturer::~CKeyCapturer()
{
    if(iHandle > -1)
    {
        iWg.CancelCaptureKey(iHandle);
    }

    iHandle = -1;

    Cancel();

    iWg.Close();
    iWsSession.Close();
}

void CKeyCapturer::ConstructL()
{
    CActiveScheduler::Add(this);

    User::LeaveIfError(iWsSession.Connect());

    iWg=RWindowGroup(iWsSession);
    User::LeaveIfError(iWg.Construct((TUint32)&iWg, EFalse));
    iWg.SetOrdinalPosition(-1);
    iWg.EnableReceiptOfFocus(EFalse);

    CApaWindowGroupName* wn=CApaWindowGroupName::NewLC(iWsSession);
    wn->SetHidden(ETrue);
    wn->SetWindowGroupName(iWg);
    CleanupStack::PopAndDestroy();

    iHandle = iWg.CaptureKey(63570/*EKeyDevice0*/, 0,0);
    Listen();
}

void CKeyCapturer::RunL()
{
    if (iStatus == KErrNone)
    {
        TWsEvent e;
        iWsSession.GetEvent(e);

        if(iObserver.KeyCapturedL(e))
        {
            TInt wgId = iWsSession.GetFocusWindowGroup();
            iWsSession.SendEventToWindowGroup(wgId, e);
        }
    }

    if (iStatus != KErrCancel)
    {
        Listen();
    }
}

void CKeyCapturer::DoCancel()
{
    iWsSession.EventReadyCancel();
}

void CKeyCapturer::Listen()
{
    iWsSession.EventReady(&iStatus);
    SetActive();
}
