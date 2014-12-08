// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Fei4 Preamp Scan
// # Comment: Nothing fancy
// ################################

#include "Fei4GlobalPreampTune.h"

Fei4GlobalPreampTune::Fei4GlobalPreampTune(Fei4 *fe, TxCore *tx, RxCore *rx, ClipBoard<RawData> *data) : ScanBase(fe, tx, rx, data) {
    mask = MASK_16;
    dcMode = QUAD_DC;
    numOfTriggers = 100;
    triggerFrequency = 10e3;
    triggerDelay = 50;
    minVcal = 10;
    maxVcal = 100;
    stepVcal = 1;

    useScap = true;
    useLcap = true;

    target = 16000;
    verbose = false;
}

// Initialize Loops
void Fei4GlobalPreampTune::init() {
    // Loop 0: Feedback
    std::shared_ptr<Fei4GlobalFeedbackBase> fbLoop(Fei4GlobalFeedbackBuilder(&Fei4::PrmpVbp));
    fbLoop->setStep(64);
    fbLoop->setMax(127);

    // Loop 1: Mask Staging
    std::shared_ptr<Fei4MaskLoop> maskStaging(new Fei4MaskLoop);
    maskStaging->setVerbose(verbose);
    maskStaging->setMaskStage(mask);
    maskStaging->setScap(useScap);
    maskStaging->setLcap(useLcap);
    //maskStaging->setStep(2);
    
    // Loop 2: Double Columns
    std::shared_ptr<Fei4DcLoop> dcLoop(new Fei4DcLoop);
    dcLoop->setVerbose(verbose);
    dcLoop->setMode(dcMode);
    //dcLoop->setStep(2);

    // Loop 3: Trigger
    std::shared_ptr<Fei4TriggerLoop> triggerLoop(new Fei4TriggerLoop);
    triggerLoop->setVerbose(verbose);
    triggerLoop->setTrigCnt(numOfTriggers);
    triggerLoop->setTrigFreq(triggerFrequency);
    triggerLoop->setTrigDelay(triggerDelay);

    // Loop 4: Data gatherer
    std::shared_ptr<StdDataLoop> dataLoop(new StdDataLoop);
    dataLoop->setVerbose(verbose);
    dataLoop->connect(g_data);

    this->addLoop(fbLoop);
    this->addLoop(maskStaging);
    this->addLoop(dcLoop);
    this->addLoop(triggerLoop);
    this->addLoop(dataLoop);

    engine.init();
}

// Do necessary pre-scan configuration
void Fei4GlobalPreampTune::preScan() {
    g_fe->writeRegister(&Fei4::Trig_Count, 12);
    // TODO VCAL and TDAC needs to be calculated per FE, not global
    g_fe->writeRegister(&Fei4::Trig_Lat, (255-triggerDelay)-4);
    for (unsigned col=1; col<81; col++)
        for (unsigned row=1; row<337; row++)
            g_fe->setFDAC(col, row, 8);
    g_fe->writeRegister(&Fei4::PlsrDAC, g_fe->toVcal(target, useScap, useLcap));
    g_fe->writeRegister(&Fei4::CalPulseWidth, 20); // Longer than max ToT 
    while(!g_tx->isCmdEmpty());
}