#ifndef KU040CONTROLLER_H
#define KU040CONTROLLER_H

// #################################
// # Author: Marius Wensing
// # Email: marius.wensing at cern.ch
// # Project: Yarr
// # Description: KU040 Controller class
// # Comment:
// # Data: Mar 2017
// ################################

#include "HwController.h"
#include "KU040TxCore.h"
#include "KU040RxCore.h"


#include "storage.hpp"

class KU040Controller : public HwController, public KU040TxCore, public KU040RxCore {
    public:
    	KU040Controller() {};
    	~KU040Controller();
        void loadConfig(json &j);

    private:
    	IPbus *m_com;
};

#endif
