/** @class  RemoteCodeTranslator */
#ifndef RemoteCodeTranslator_h
#define RemoteCodeTranslator_h

#include "../TennisConstants/TennisConstants.h"
#include "PinInterface.h"
#if defined _WIN32
#include "TranslateConstant.h"
#include "Logger.h"
#endif

struct remoteDataStructure { int pin_1 = 1; int pin_2 = 1; int pin_3 = 1; int pin_4 = 1; };
class RemoteCodeTranslator {
 public:
    RemoteCodeTranslator();
    ~RemoteCodeTranslator(); 
    int translateRemoteCode( remoteDataStructure remotePinData );
 private:
    #if defined _WIN32
    Logger* _logger;
    TranslateConstant* _translator;
    #endif
};

#endif
