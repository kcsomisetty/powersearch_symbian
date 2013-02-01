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

#ifndef MAKECALL_H
#define MAKECALL_H

#include "MakeCall.h"
#include "model.h"

#include <Etel3rdParty.h>
// link to etel3rdparty.lib

class MDialObserver
{
    public:
        virtual void CallDialedL(TInt aError) = 0;
};

class CCallDialer : public CActive
{
    public:
        static CCallDialer* NewL(Model* aCallBack, const TDesC& aNumber);
        static CCallDialer* NewLC(Model* aCallBack, const TDesC& aNumber);
        ~CCallDialer();

    protected:
        CCallDialer(Model* aObserver);
        void ConstructL(const TDesC& aNumber);

    private:
        void RunL();
        void DoCancel();

    private:
        Model*                        iObserver;
        CTelephony*                   iTelephony;
        CTelephony::TCallId           iCallId;
        CTelephony::TCallParamsV1     iCallParams;
        CTelephony::TCallParamsV1Pckg iCallParamsPckg;
};
#endif // MAKECALL_H
