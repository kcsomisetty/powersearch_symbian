//Powersearch is search app for symbian smartphones
//Copyright (C) 2010 Krishna Somisetty

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Powersearch is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "MakeCall.h"

CCallDialer* CCallDialer::NewL(Model* aCallBack, const TDesC& aNumber)
{
    LS("CCallDialer::NewL =>>");
    CCallDialer* self = CCallDialer::NewLC(aCallBack, aNumber);
    CleanupStack::Pop(self);
    LS("CCallDialer::NewL <<");
    return self;
}

CCallDialer* CCallDialer::NewLC(Model* aCallBack, const TDesC& aNumber)
{
    LS("CCallDialer::NewLC =>>");
    CCallDialer* self = new (ELeave) CCallDialer(aCallBack);
    CleanupStack::PushL(self);
    self->ConstructL(aNumber);
    LS("CCallDialer::NewL <<");
    return self;
}


CCallDialer::~CCallDialer()
{
    LS("CCallDialer::~CCallDialer =>>");
    Cancel();
    delete iTelephony;
    LS("CCallDialer::~CCallDialer <<");
}

void CCallDialer::ConstructL(const TDesC& aNumber)
{
    LS("CCallDialer::ConstructL =>>");
    iTelephony = CTelephony::NewL();
    CTelephony::TTelNumber telNumber(aNumber);

    iCallParams.iIdRestrict = CTelephony::ESendMyId;

    iTelephony->DialNewCall(iStatus, iCallParamsPckg, telNumber, iCallId);
    SetActive();
    LS("CCallDialer::ConstructL <<");
}

CCallDialer::CCallDialer(Model* aObserver)
: CActive(EPriorityNormal),iObserver(aObserver), iCallParamsPckg(iCallParams)
{
    CActiveScheduler::Add(this);
}

void CCallDialer::RunL()
{
    LS("CCallDialer::RunL =>>");
    iObserver->CallDialedL(iStatus.Int());
    LS("CCallDialer::RunL <<");
}

void CCallDialer::DoCancel()
{
    iTelephony->CancelAsync(CTelephony::EDialNewCallCancel);
}
