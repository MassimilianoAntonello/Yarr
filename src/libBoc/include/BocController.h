#ifndef BOCCONTROLLER_H
#define BOCCONTROLLER_H

// #################################
// # Author: Marius Wensing
// # Email: marius.wensing at cern.ch
// # Project: Yarr
// # Description: Boc Controller class
// # Comment:
// # Data: Mar 2017
// ################################

#include "HwController.h"
#include "BocTxCore.h"
#include "BocRxCore.h"


#include "storage.hpp"

class BocController : public HwController, public BocTxCore, public BocRxCore {
    public:
    	BocController() {};
    	~BocController();
        void loadConfig(json &j);

    private:
    	BocCom *m_com;
};

#endif
